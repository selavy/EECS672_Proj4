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

//  BMPImageReader.c++ -- subclass of ImageReader for reading image files in
//                        BMP format

//  Written by James R. Miller (jrmiller@ku.edu)
//  Department of Electrical Engineering and Computer Science
//  The University of Kansas
//  Documentation on use:
//      http://people.eecs.ku.edu/~miller/Courses/ToolDoc/ImageReader.html

#include "BMPLoader.h"
#include "BMPImageReader.h"

// The copy constructor cannot be used.

BMPImageReader::BMPImageReader(const BMPImageReader& b) : ImageReader(b)
{
}

// Here are the supported constructors

BMPImageReader::BMPImageReader(std::string fileName) :
	ImageReader(fileName)
{
	readImage();
}

bool BMPImageReader::read()
{
	int			widthOut, heightOut, nChannelsOut;
	GLubyte*	pixels;

	LOAD_TEXTUREBMP_RESULT res = loadBMPData(fullFileName.c_str(),&pixels,
										widthOut, heightOut, nChannelsOut);
	if ( (pixels == NULL) || (res != LOAD_TEXTUREBMP_SUCCESS) )
		return false;

	theImage = new cryph::Packed3DArray<GLubyte>(
							heightOut, widthOut, nChannelsOut);
	int nextPixel = 0;
	for (int i=0 ; i<heightOut ; i++)
		for (int j=0 ; j<widthOut ; j++)
			for (int k=0 ; k<nChannelsOut ; k++)
				theImage->setDataElement(i,j,k,pixels[nextPixel++]);
	delete [] pixels;

    return true;
}
