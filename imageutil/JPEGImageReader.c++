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

//  JPEGImageReader.c++ -- subclass of ImageReader for reading jpeg image files
//  Written by James R. Miller (jrmiller@ku.edu)
//  Department of Electrical Engineering and Computer Science
//  The University of Kansas
//  Documentation on use:
//      http://people.eecs.ku.edu/~miller/Courses/ToolDoc/ImageReader.html

#include <stdio.h>

// Use standard jpeg library
#include "jpeglib.h"

#include "JPEGImageReader.h"

using namespace std;

// The copy constructor cannot be used.

JPEGImageReader::JPEGImageReader(const JPEGImageReader& j) : ImageReader(j)
{
}

// Here are the supported constructors

JPEGImageReader::JPEGImageReader(std::string fileName) :
	ImageReader(fileName)
{
	readImage();
}

bool JPEGImageReader::read()
{
    FILE *fp = fopen(fullFileName.c_str(), "rb");
    if (fp == NULL)
	{
		cerr << "JPEGImageReader:: read - could not open: '" << fullFileName
		     << "'\n";
        return false;
	}

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, fp);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);

	if (debug)
	{
		cout << "Reading: " << fullFileName << "; cinfo fields:\n";
		cout << "(image_width,image_height) = (" << cinfo.image_width << ','
		     << cinfo.image_height
		     << "), num_components = " << cinfo.num_components
		     << ", jpeg_color_space: " << cinfo.jpeg_color_space
		     << "\n\nout_color_space: " << cinfo.out_color_space
		     << ", scale_num = " << cinfo.scale_num
		     << ", scale_denom = " << cinfo.scale_denom
		     << "\n\n(output_width,output_height) = (" << cinfo.output_width
		     << ',' << cinfo.output_height
		     << ")\nout_color_components = "
		     << cinfo.out_color_components
		     << "; output_components = " << cinfo.output_components
		     << "\n\nrec_outbuf_height = "
		     << cinfo.rec_outbuf_height
		     << '\n';
	}

	// storage for OpenGL image
	theImage = new cryph::Packed3DArray<GLubyte>
				(cinfo.image_height, cinfo.image_width, cinfo.out_color_components);

	// JDIMENSION is unsigned int
	// JSAMPLE is short

	// JSAMPROW   is JSAMPLE*    { one image row of pixel samples }
	// JSAMPARRAY is JSAMPROW*   { array of sample rows }
	// JSAMPIMAGE is JSAMPARRAY* { 3D array: [row][col][color] }

	// JSAMPLE - one channel (r, g, b; or gray)
	// JSAMPLE* (JSAMPROW) - one scan line
	// JSAMPLE** (JSAMPARRAY or JSAMPROW*) -- one channel of an image

	// Main read routine is:
	// JDIMENSION jpeg_read_scanlines (j_decompress_ptr cinfo,
    //                 JSAMPARRAY scanlines, JDIMENSION max_lines);

	JSAMPARRAY scanlines = new JSAMPROW[cinfo.rec_outbuf_height];
	int nCols = theImage->getDim2();
	int nChannels = theImage->getDim3();
	for (int i=0 ; i<cinfo.rec_outbuf_height ; i++)
		scanlines[i] = new JSAMPLE[nCols*nChannels];

	JDIMENSION totalLinesRead = 0;
	int setRow = theImage->getDim1() - 1;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		JDIMENSION res = jpeg_read_scanlines(&cinfo,scanlines,
			cinfo.rec_outbuf_height);
		for (int ii=0 ; ii<res ; ii++)
		{
			for (int jj=0 ; jj<nChannels*nCols ; jj+=nChannels)
			{
				int setCol = jj / nChannels;
				for (int kk=0 ; kk<nChannels ; kk++)
					theImage->setDataElement(setRow,setCol,kk,
							scanlines[ii][jj+kk]);
			}
			setRow--;
		}
		totalLinesRead += res;
	}
	if (debug)
		cout << "Read " << totalLinesRead << " lines.\n";

	jpeg_finish_decompress(&cinfo);
	fclose(fp);
	jpeg_destroy_decompress(&cinfo);

	for (int i=0 ; i<cinfo.rec_outbuf_height ; i++)
		delete [] scanlines[i];
	delete [] scanlines;

	return true;
}
