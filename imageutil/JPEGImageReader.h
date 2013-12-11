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

// JPEGImageReader.h -- subclass of ImageReader for reading jpeg image files
//
//  Written by James R. Miller (jrmiller@ku.edu)
//  Department of Electrical Engineering and Computer Science
//  The University of Kansas
//  Documentation on use:
//      http://people.eecs.ku.edu/~miller/Courses/ToolDoc/ImageReader.html
//

#ifndef JPEGIMAGEREADER_H
#define JPEGIMAGEREADER_H

#include "ImageReader.h"

class JPEGImageReader : public ImageReader
{
public:
	JPEGImageReader(std::string fileName);

protected:
	JPEGImageReader(const JPEGImageReader& s); // cannot use the copy constructor

	virtual bool read();
};

#endif
