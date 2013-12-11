// Square.h
#ifndef SQUARE_H
#define SQUARE_H

#include "GeneralMV.h"
#include "ImageReader.h"

#ifndef WHITE
#define WHITE 1
#endif

#ifndef BLACK
#define BLACK 0
#endif

#ifndef BORDER
#define BORDER 2
#endif

#ifndef TEST
#define TEST 3
#endif

#define SQ_VERTICES 36
#define SQ_CORNERS 8
#define SQ_FACES 6

#define _V( x ) _vertices[ (x) ]

class Square : public GeneralMV
{
 public:
  Square( int color, vec3 corner, float width, float thickness );
	virtual ~Square();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void defineModel();
	void render();
	void defineTex( int Index );
	
private:
	GLuint vao;
	GLuint vbo[3]; //!> 0: coordinate data, 1: normals, 2: texture
	GLuint mytex[2]; //!> texture mapping buffer
	static GLint pvaLoc_vTexCoord;
	
	vec2 tex_coords[SQ_VERTICES];
	double _limits[6];
	int _color;

};

#endif
