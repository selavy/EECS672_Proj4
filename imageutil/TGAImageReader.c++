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

//  TGAImageReader.c++ -- subclass of ImageReader for reading image files in
//                        targa (.tga) format

//  Written by James R. Miller (jrmiller@ku.edu)
//  Department of Electrical Engineering and Computer Science
//  The University of Kansas
//  Documentation on use:
//      http://people.eecs.ku.edu/~miller/Courses/ToolDoc/ImageReader.html

#include <iomanip>

#include "TGAImageReader.h"

// The copy constructor cannot be used.

TGAImageReader::TGAImageReader(const TGAImageReader& t) : ImageReader(t)
{
}

// Here are the supported constructors

TGAImageReader::TGAImageReader(std::string fileName) :
	ImageReader(fileName)
{
	readImage();
}

#include <iostream>

#include <stdio.h>

using namespace std;

/* =============
getRGBA

Reads in RGBA data for a 32bit image.
============= */
void getRGBA (FILE *s, int size, unsigned char* buf)
{
    int bread = fread (buf, sizeof (unsigned char), size * 4, s);

    /* TGA is stored in BGRA, make it RGBA */
    for (int i = 0; i < size * 4; i += 4 )
    {
        unsigned char temp = buf[i];
        buf[i] = buf[i + 2];
        buf[i + 2] = temp;
    }
}
/* =============
getRGB

Reads in RGB data for a 24bit image.
============= */
void getRGB (FILE *s, int size, unsigned char* buf)
{
    int bread = fread (buf, sizeof (unsigned char), size * 3, s);

    /* TGA is stored in BGR, make it RGB */
    for (int i = 0; i < size * 3; i += 3)
    {
        unsigned char temp = buf[i];
        buf[i] = buf[i + 2];
        buf[i + 2] = temp;
    }
}
/* =============
getGray

Gets the grayscale image data.  Used as an alpha channel.
============= */
void getGray (FILE *s, int size, unsigned char* buf)
{
    int bread = fread (buf, sizeof (unsigned char), size, s);
}
/* =============
getData

Gets the image data for the specified bit depth.
============= */
void getData (FILE *s, int sz, int iBits, unsigned char* buf)
{
    if (iBits == 32)
        getRGBA (s, sz,  buf);
    else if (iBits == 24)
        getRGB (s, sz, buf);
    else if (iBits == 8)
        getGray (s, sz, buf);
}
/* =============
loadTGA

Loads up a targa file.  Supported types are 8,24 and 32 uncompressed images.
id is the texture ID to bind too.
============= */
bool TGAImageReader::read()
{
    FILE *fp = fopen(fullFileName.c_str(), "rb");
    if (fp == NULL)
	{
		cerr << "TGAImageReader:: read - could not open: '" << fullFileName
		     << "'\n";
        return false;
	}
	else if (debug)
		cout << "Reading: " << fullFileName << '\n';

    unsigned char header[12];
	// Header (12 bytes)
	// byte   0: ignored
	// byte 1-2: color map information and image type
	// byte 3-12: ? (original implementation just fseek-ed over them...)
    fread (header, sizeof (char), 12, fp);
	if (debug)
	{
		cout << "Header: ";
		for (int ii=0 ; ii<12 ; ii++)
			cout << setw(4) << (int)header[ii];
		cout << '\n';
	}

    if (header[1] != 0 || (header[2] != 2 && header[2] != 3))
	{
		fclose(fp);
		return false;
	}

    unsigned char info[6];
    fread (info, sizeof (char), 6, fp);
	if (debug)
	{
		cout << "  info: ";
		for (int jj=0 ; jj<12 ; jj++)
			cout << setw(4) << (int)info[jj];
		cout << '\n';
	}

    int nCols = info[0] + info[1] * 256;
    int nRows = info[2] + info[3] * 256;
    int imageBits =	info[4];
	int nChannels = imageBits/8;

    int size = nCols * nRows;

    /* make sure we are loading a supported type */
    if (imageBits != 32 && imageBits != 24 && imageBits != 8)
	{
		fclose(fp);
		return false;
	}

	theImage = new cryph::Packed3DArray<GLubyte>
								(nRows, nCols, nChannels);
	unsigned char* imageData = theImage->getModifiableData();
	getData (fp, size, imageBits, imageData);
    fclose (fp);

	if (info[5] != 0)
	{
		// rows are inverted
		int nRowsToSwap = nRows / 2;
		int m = nRows - 1;
		for (int i=0 ; i<nRowsToSwap ; i++)
		{
			// swap row 'i' with row 'm'
			for (int j=0 ; j<nCols ; j++)
				for (int k=0 ; k<nChannels ; k++)
				{
					GLubyte from_i = theImage->getDataElement(i,j,k);
					GLubyte from_m = theImage->getDataElement(m,j,k);
					theImage->setDataElement(i,j,k,from_m);
					theImage->setDataElement(m,j,k,from_i);
				}
			m--;
		}
	}

	return true;
}
