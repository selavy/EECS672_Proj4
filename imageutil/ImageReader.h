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

//  ImageReader.h -- read an Image file and allow it to be queried and passed
//                 to various OpenGL routines like glDrawPixels, glTexImage2D,
//                 gluBuild2DMipmaps, and gluScaleImage. Currently only the following
//                 image file types are supported:
//                     * JPEG format (.jpg; .jpeg) (Requires a system jpeg library)
//                     * TARGA format (.tga)
//                     * BMP format (.bmp)
//
//  Except as noted in the Acknowledgments below, this code was written by
//  James R. Miller (jrmiller@ku.edu)
//  Department of Electrical Engineering and Computer Science
//  The University of Kansas
//
// Quick synopsis of use:
// 1. Use ImageReader::create to open and read an image file
// 2. When ready to use glTexImage2D, use the various query methods to retrieve relevant
//    parameters for the call (including getPixels()->getData() to retrieve the texture).
//
// Acknowledgments:
// Various public domain and/or open source image reading utilities written by other
// people are used in files associated with the ImageReader class.  Appropriate
// attribution appears in those files obtained from other sources.

#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <GL/gl.h>

#include "Packed3DArray.h"

class ImageReader
{
public:
	// Methods to return appropriate values for various parameters to
	// OpenGL functions like glTexImage2D, glDrawPixels, etc.
	int		getBorder() const;
	GLenum	getFormat() const;
	GLint	getInternalFormat() const;
	GLenum	getType() const;
	int		getWidth() const;
	int		getHeight() const;

	// In the "getTexture" call, a pointer to the actual internal image
	// is returned. The caller should NEVER delete this pointer.
	const void* getTexture() const;

	// The actual number of logical channelts per pixel. Rarely needed in client code.
	int		getNumChannels() const;

	// convenience interface allowing you to record the textureID with the instance
	GLuint getTextureID() const { return textureID; }
	void setTextureID(GLuint tID);

	void	setDebug(bool b) { debug = b; }
	
	std::string getFileName() const { return fullFileName; }

	// public class methods
	// 1. create - dynamically allocates an ImageReader instance of the appropriate
	//             subtype if the type can be determined from the file name
	//             extension.
	//             It returns NULL if a type cannot be determined this way.
	//             If a non-NULL ImageReader pointer is returned, the caller is
	//             responsible for deleting it when it is done with it.
	static ImageReader*	create(std::string fileName,
						bool debug=ImageReader::defaultDebug);
	// 2. Miscellaneous
	static void setDefaultDebug(bool b) { defaultDebug = b; }

protected:
	// Since the class is abstract, you CANNOT use these constructors.
	// Use instead the class method "create" or create instances of
	// concrete subclasses (TGAImageReader, RGBImageReader, etc.)
	ImageReader(const std::string& fileName);

	ImageReader(const ImageReader& s); // cannot use the copy constructor

	virtual bool read() = 0;
	void	readImage();

	// The image read from the file
	cryph::Packed3DArray<GLubyte>*	theImage;

	int	border; // always '0' in OpenGL 3.3 and beyond
	std::string	fullFileName;
	bool	debug;
	bool	readFailed;

private:

	// Other private instance methods:
	static ImageReader* guessFileType(const std::string& fileName);

	GLuint textureID;
	bool	promoteSingleChannelToGray;

	static bool defaultDebug;
};

#endif
