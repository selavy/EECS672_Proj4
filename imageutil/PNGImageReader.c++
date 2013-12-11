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

//  PNGImageReader.c++ -- subclass of ImageReader for reading png image files
//  Written by James R. Miller (jrmiller@ku.edu)
//  Department of Electrical Engineering and Computer Science
//  The University of Kansas
//  Documentation on use:
//      http://people.eecs.ku.edu/~miller/Courses/ToolDoc/ImageReader.html

#include <stdio.h>

// Use standard png library
#include "png.h"

#include "PNGImageReader.h"

using namespace std;

// The copy constructor cannot be used.

PNGImageReader::PNGImageReader(const PNGImageReader& j) : ImageReader(j)
{
}

// Here are the supported constructors

PNGImageReader::PNGImageReader(std::string fileName) :
	ImageReader(fileName)
{
	readImage();
}

int PNGImageReader::numChannelsFromColorType(int cType)
{
	if (debug)
		cout << "PNGImageReader::numChannelsFromColorType: ";
	switch (cType)
	{
		case PNG_COLOR_TYPE_GRAY:
			if (debug)
				cout << "PNG_COLOR_TYPE_GRAY ==> 1\n";
			return 1;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			cerr << getFileName() << " has unsupported PNG_COLOR_TYPE_GRAY_ALPHA\n";
			return 0;
		case PNG_COLOR_TYPE_PALETTE:
			cerr << getFileName() << " has unsupported PNG_COLOR_TYPE_PALETTE\n";
			return 0;
		case PNG_COLOR_TYPE_RGB:
			if (debug)
				cout << "PNG_COLOR_TYPE_RGB ==> 3\n";
			return 3;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			if (debug)
				cout << "PNG_COLOR_TYPE_RGB_ALPHA ==> 4\n";
			return 4;
		case PNG_COLOR_MASK_PALETTE:
			cerr << getFileName() << " has unsupported PNG_COLOR_MASK_PALETTE\n";
			return 0;
//		case PNG_COLOR_MASK_COLOR: Same as PNG_COLOR_TYPE_RGB
//			cout << "PNG_COLOR_MASK_COLOR\n"; break;
//		case PNG_COLOR_MASK_ALPHA: Same as PNG_COLOR_TYPE_GRAY_ALPHA
//			cout << "PNG_COLOR_MASK_ALPHA\n"; break;
		default:
			cerr << getFileName() << " has UNKNOWN PNG color type: " << cType << "\n";
			return 0;
	}
}

bool PNGImageReader::read()
{
    FILE *fp = fopen(fullFileName.c_str(), "rb");
    if (fp == NULL)
	{
		cerr << "PNGImageReader::read - could not open: '" << fullFileName
		     << "'\n";
        return false;
	}
	// check signature
	const int NUM_HEADER_BYTES_TO_CHECK = 8;
	unsigned char header[NUM_HEADER_BYTES_TO_CHECK];
	fread(header, 1, NUM_HEADER_BYTES_TO_CHECK, fp);
	if (png_sig_cmp(header, 0, NUM_HEADER_BYTES_TO_CHECK) != 0)
	{
		cerr << "PNGImageReader::read - bad signature\n";
        return false;
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		cerr << "PNGImageReader::read - could not allocate png_struct\n";
		return false;
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		cerr << "PNGImageReader::read - could not allocate png_info\n";
		return false;
	}
	// set up input reading code
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, NUM_HEADER_BYTES_TO_CHECK); // tell it we read the signature
	png_read_info(png_ptr, info_ptr);

	// get dimensions and such so we can
	png_uint_32 width, height;
	int bit_depth, color_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
				 NULL, NULL, NULL); // don't care about interlace, compression, or filter types
	if (debug)
	{
		cout << "PNGImageReader::read - Image specs: "
		     << width << " x " << height << "; bit_depth: " << bit_depth
		     << "; color_type: " << color_type << "\n";
	}
	int nChannels = numChannelsFromColorType(color_type);
	if (nChannels == 0)
		return false;
	if (debug)
		cout << "PNGImageReader::read - nChannels = " << nChannels << '\n';
	theImage = new cryph::Packed3DArray<GLubyte>(height, width, nChannels);
	GLubyte* p = theImage->getModifiableData();
	png_bytep* row_pointers = new png_byte*[height];
	// need to flip order of the rows:
	int rpi = height;
	for (int i=0 ; i<height ; i++)
		row_pointers[--rpi] = p + i*width*nChannels*sizeof(GLubyte);
	png_read_image(png_ptr, row_pointers);
	delete [] row_pointers;
	png_read_end(png_ptr, (png_infop)NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	return true;
}
