// Square.c++
#include "Square.h"
#include "ControllerSub.h"

#include <iostream>
using namespace std;

/* static */ GLint Square::pvaLoc_vTexCoord = -1;

Square::Square( 
	       int color,
	       vec3 corner,           //!> location of bottom left corner of square
	       float width,           //!> width of square
	       float thickness        //!> thickness of square
		)
  :
_color( color )
{
  _vertices = new vec3[SQ_CORNERS];

  _points = new vec3[ SQ_VERTICES ];
  _normals = new vec3[ SQ_VERTICES ];

  float xmin = corner[0];
  float xmax = corner[0] + width;
  float ymin = corner[1];
  float ymax = corner[1] + thickness;
  float zmin = corner[2];
  float zmax = corner[2] + width;

  _limits[0] = xmin;
  _limits[1] = xmax;
  _limits[2] = ymin;
  _limits[3] = ymax;
  _limits[4] = zmin;
  _limits[5] = zmax;

  vec3 tmp[SQ_CORNERS] = {
    { xmin, ymin, zmin },
    { xmin, ymax, zmin },
    { xmax, ymax, zmin },
    { xmax, ymin, zmin },
    { xmin, ymin, zmax },
    { xmin, ymax, zmax },
    { xmax, ymax, zmax },
    { xmax, ymin, zmax }
  };

  memcpy( _vertices, tmp, SQ_CORNERS * sizeof( vec3 ) );

  defineModel();
} /* end Square::Square() */

Square::~Square()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  delete [] _points;
  delete [] _normals;
  delete [] _vertices;
} /* end Square::~Square */

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Square::getWCBoundingBox(double* xyzLimits) const
{
  memcpy( xyzLimits, _limits, 6 * sizeof( double ) );
} /* end Square::getWCBoundingBox() */

void Square::defineTex( int Index )
{
  tex_coords[Index - 6][0] = 0.0;
  tex_coords[Index - 6][1] = 0.0;  
  tex_coords[Index - 5][0] = 0.0;
  tex_coords[Index - 5][1] = 1.0;
  tex_coords[Index - 4][0] = 1.0;
  tex_coords[Index - 4][1] = 1.0;
  tex_coords[Index - 3][0] = 0.0;
  tex_coords[Index - 3][1] = 0.0;
  tex_coords[Index - 2][0] = 1.0;
  tex_coords[Index - 2][1] = 1.0;
  tex_coords[Index - 1][0] = 1.0;
  tex_coords[Index - 1][1] = 1.0;
}

void Square::defineModel()
{
  int index = 0;

  quad( 1, 0, 3, 2, index );
  defineTex( index );
  quad( 3, 0, 4, 7, index );
  defineTex( index );
  quad( 2, 3, 7, 6, index );
  defineTex( index );
  quad( 5, 4, 0, 1, index );
  defineTex( index );
  quad( 6, 5, 1, 2, index );
  defineTex( index );
  quad( 4, 5, 6, 7, index );
  defineTex( index );

  /* create VAO */  
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 3, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SQ_VERTICES, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SQ_VERTICES, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcNormal );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec2 ) * SQ_VERTICES, tex_coords, GL_STATIC_DRAW );
  pvaLoc_vTexCoord = pvAttribLocation( shaderProgram, "vTexCoord" );
  glVertexAttribPointer( pvaLoc_vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( pvaLoc_vTexCoord );

  /* load image */
  ImageReader * texImage = ImageReader::create("../images/light_square.jpg");
  if( texImage == NULL )
      cout << "PROBLEM!" << endl;

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glGenTextures( 2, mytex );
  glBindTexture( GL_TEXTURE_2D, mytex[0] );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  GLint level = 0;
  int pw = texImage->getWidth(), ph = texImage->getHeight();
  GLint iFormat = texImage->getInternalFormat();
  GLenum format = texImage->getFormat();
  GLenum type = texImage->getType();
  const GLint border = 0;
  const void* pixelData = texImage->getTexture();
  glTexImage2D(GL_TEXTURE_2D, level, iFormat, pw, ph, border, format, type, pixelData );
  delete texImage;

  ImageReader * texImage2 = ImageReader::create("../images/dark_square.jpg");
  if( texImage2 == NULL )
      cout << "PROBLEM!" << endl;

  glBindTexture( GL_TEXTURE_2D, mytex[1] );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  level = 0;
  pw = texImage2->getWidth();
  ph = texImage2->getHeight();
  iFormat = texImage2->getInternalFormat();
  format = texImage2->getFormat();
  type = texImage2->getType();
  const GLint border2 = 0;
  const void* pixelData2 = texImage2->getTexture();
  glTexImage2D(GL_TEXTURE_2D, level, iFormat, pw, ph, border2, format, type, pixelData2 );
  delete texImage2;
} /* end Square::defineModel() */

void Square::render()
{
  ControllerSub * c = dynamic_cast<ControllerSub*>(Controller::getCurrentController());
  if(! c->drawingOpaque() )
    return;

  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  
  glUseProgram( shaderProgram );
  
  getMatrices( _limits );

  vec4 kd;
  kd[3] = 1.0f;

  if( _color == WHITE )
    {
      kd[0] = RGB( 245.0f );
      kd[1] = RGB( 255.0f );
      kd[2] = RGB( 250.0f );
    }
  else if( _color == BLACK )
    {
      kd[0] = RGB( 49 );
      kd[1] = RGB( 79 );
      kd[2] = RGB( 79 );
    }
  else if( _color == BORDER )
    {

      kd[0] = RGB( 49 );
      kd[1] = RGB( 79 );
      kd[2] = RGB( 79 );
    }
  else if( _color == TEST )
    {
      kd[0] = 1.0f;
      kd[1] = 0.0f;
      kd[2] = 0.0f;
    }
  else
    {
      kd[0] = 0.0f;
      kd[1] = 0.0f;
      kd[2] = 1.0f;
    }

  //vec4 ks = { 0.3, 0.3, 0.3, 1.0 };
  vec4 ks = { 1.0, 1.0, 1.0, 1.0 };
  vec4 ka;
  memcpy( ka, kd, sizeof( vec4 ) );

  usingTexture();
  sendPhongLightModel( ka, kd, ks, 10 );
  if( _color == WHITE )
    glBindTexture( GL_TEXTURE_2D, mytex[0] );
  else
    {
      glBindTexture( GL_TEXTURE_2D, mytex[1] );
    }
  glUniform1i( ppuLoc_texture, 0 );
  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLES, 0, SQ_VERTICES );

  glUseProgram( pgm );
} /* end Square::render() */
