/*
Copyright (c) 1998-2012, cryph Component Library and Tools
James R. Miller, University of Kansas, Lawrence, Kansas, U.S.A.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the cryph Component Library and Tools
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS OPEN SOURCE. IT IS NOT IN THE PUBLIC DOMAIN.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

//  ImageReader.c++ -- read an Image file and allow it to be queried and passed
//                 to various OpenGL routines like glDrawPixels, glTexImage2D,
//                 gluBuild2DMipmaps, and gluScaleImage. Currently only the
//                 following image file types are supported:
//                     * JPEG format (.jpg; .jpeg) (Requires a system jpeg
//                       library)
//                     * TARGA format (.tga)
//                     * BMP format (.bmp)
//                     * a verbose uncompressed ascii format (.tex; .text)
//
//  Written by James R. Miller (jrmiller@ku.edu)
//  Department of Electrical Engineering and Computer Science
//  The University of Kansas
//  Documentation on use:
//      http://people.eecs.ku.edu/~miller/Courses/ToolDoc/ImageReader.html
//
// Acknowledgments:
// Various public domain and/or open source image reading utilities written by
// other people are used in files associated with the ImageReader class.
// Appropriate attribution appears in those files obtained from other sources.

/*********
IF YOUR SYSTEM DOES NOT HAVE THE STANDARD JPEG LIBRARIES, COMMENT OUT THE
FOLLOWING TWO LINES WHERE THEY APPEAR BELOW:

#include "JPEGImageReader.h"
...
p = new JPEGImageReader(fileName); -- appears TWICE below...

NOTHING ELSE NEEDS TO CHANGE. (And then don't compile JPEGImageReader.c++)
*********/
 
#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "ImageReader.h"

// Known subclasses (needed by factory method "create"):
#include "BMPImageReader.h"
#include "JPEGImageReader.h"
#include "PNGImageReader.h"
#include "TGAImageReader.h"

using namespace std;
using namespace cryph;

// class variables

bool ImageReader::defaultDebug = false;

// The copy constructor cannot be used.

ImageReader::ImageReader(const ImageReader&)
{
	std::cerr << "***** ERROR: ImageReader copy constructor called!!\n";
}

// Here are the supported constructors

ImageReader::ImageReader(const std::string& fileName) :
	theImage(NULL), border(0),
	fullFileName(fileName), debug(ImageReader::defaultDebug),
	readFailed(false), textureID(0),
	promoteSingleChannelToGray(true)
{
}

// Methods....

ImageReader* ImageReader::create(std::string fileName, bool debug) // CLASS METHOD
{
	ImageReader* p = guessFileType(fileName);
	if (p == NULL)
		return NULL;

	if (p->readFailed)
	{
		delete p;
		return NULL;
	}
	p->setDebug(debug);

	if (p->theImage->getDim3() == 1)
	{
		if (p->promoteSingleChannelToGray)
		{
			int nRows = p->theImage->getDim1();
			int nCols = p->theImage->getDim2();
			cryph::Packed3DArray<GLubyte>* grayImage = new cryph::Packed3DArray<GLubyte>(nRows, nCols, 3);
			for (int i=0 ; i<nRows ; i++)
				for (int j=0 ; j<nCols ; j++)
				{
					GLubyte b = p->theImage->getDataElement(i,j,0);
					for (int k=0 ; k<3 ; k++)
						grayImage->setDataElement(i,j,k,b);
				}
			delete p->theImage;
			p->theImage = grayImage;
		}
	}
	return p;
}

int ImageReader::getBorder() const
{
	return border;
}

GLenum ImageReader::getFormat() const
{
	int nChannels = theImage->getDim3();
	if (nChannels == 1)
		return GL_RED;
	if (nChannels == 3)
		return GL_RGB;
	if (nChannels == 4)
		return GL_RGBA;

	// probably an error:
	std::cout << "ImageReader::getFormat(), nChannels = " << nChannels << endl;
	return GL_RGB;
}

GLint ImageReader::getInternalFormat() const
{
	int nChannels = theImage->getDim3();
	if (nChannels == 1)
		return GL_RED;
	if (nChannels == 3)
		return GL_RGB;
	if (nChannels == 4)
		return GL_RGBA;
	
	// probably an error:
	std::cout << "ImageReader::getInternalFormat(), nChannels = " << nChannels << endl;
	return GL_RGB;
}

int ImageReader::getNumChannels() const
{
	return theImage->getDim3();
}

int ImageReader::getHeight() const
{
	return theImage->getDim1();
}

int ImageReader::getWidth() const
{
	return theImage->getDim2();
}

const void* ImageReader::getTexture() const
{
	return theImage->getData();
}

GLenum ImageReader::getType() const
{
	return GL_UNSIGNED_BYTE;
}

ImageReader* ImageReader::guessFileType(const std::string& fileName) // CLASS METHOD
{
	int dotLoc = fileName.find_last_of('.');
	if (dotLoc != std::string::npos)
	{
		std::string extension = fileName.substr(dotLoc+1);
		if ((extension.compare("bmp") == 0) || (extension.compare("BMP") == 0))
			return new BMPImageReader(fileName);
		if ((extension.compare("jpg") == 0) || (extension.compare("JPG") == 0))
			return new JPEGImageReader(fileName);
		if ((extension.compare("jpeg") == 0) || (extension.compare("JPEG") == 0))
			return new JPEGImageReader(fileName);
		if ((extension.compare("png") == 0) || (extension.compare("PNG") == 0))
			return new PNGImageReader(fileName);
		if ((extension.compare("tga") == 0) || (extension.compare("TGA") == 0))
			return new TGAImageReader(fileName);
	}

	cerr << "ImageReader::guessFileType cannot determine file type of: "
		<< fileName << endl;
	return NULL;
}

void ImageReader::readImage()
{
	readFailed = !read();
}

void ImageReader::setTextureID(GLuint tID)
{
	if (debug)
		std::cout << "Setting ImageReader textureID to " << tID << std::endl;
	textureID = tID;
}