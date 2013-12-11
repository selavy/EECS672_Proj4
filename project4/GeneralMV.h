// GeneralMV.h

#ifndef GENERAL_MV_H
#define GENERAL_MV_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Matrix4x4.h"
#include "AffVector.h"
#include "AffPoint.h"
#include <cmath>
#include "Controller.h"
#include "MatrixIF.h"
#include <cstring>
#include <iostream>

#define RGB(x) ( (x) / 255.0 )

typedef float mat4[16]; // specify in row major, and will always transpose with openGL
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

class GeneralMV : public ModelView
{
 public:
  /* types */
  typedef enum PROJECTION_TYPE {
    ORTHOGONAL = 0,
    OBLIQUE,
    PERSPECTIVE
  } PROJECTION_TYPE;

  enum { ScreenHeight = 50, ScreenWidth = 50 };
  enum { numLights = 3 }; // enum hack to avoid using a #define statement

 public:
	GeneralMV();
	virtual ~GeneralMV();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	virtual void getWCBoundingBox(double* xyzLimitsF) const = 0;
	virtual void render() = 0;
	virtual void handleCommand( unsigned char key, double ldsX, double ldsY );

	static void setProjectionType( PROJECTION_TYPE proj_type );

	static void handleMouseFunc( int button, int state, int x, int y );
	static void handleMouseMotion( int x, int y );
	static void handleMousePassiveMotion( int x, int y );
	
 protected:
	void calcBoundingSphere();
	void setECMinMax();
	void getMatrices( double limits[6] );

	void sendPhongLightModel( const vec4& ka, const vec4& kd, const vec4& ks, const float m );

	void tri( const vec3& veca, const vec3& vecb, const vec3& vecc, int& Index );
	void quad( int a, int b, int c, int d, int& Index );
	void printBox();
	void printEyeLoc();
	void notUsingTexture();

	// TODO: dynamic rotations (HIGH)
	// common 3D global dynamic viewing requests
	static void addToGlobalRotationDegrees( double rx, double ry, double rz );
	static void addToGlobalZoom( double increment );  //!> scale += multiplier
	static void scaleGlobalZoom( double multiplier ); //!> scale *= multiplier
	static double scale;                              //!> for global zoom
	static cryph::Matrix4x4 M4x4_dynamic;             //!> mouse-based dynamic 3D rotations

	vec3 * _points;
	vec3 * _normals;
	vec3 * _vertices;

	static GLint pvaLoc_wcPosition;
	static GLint pvaLoc_wcNormal;

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint ppuLoc_M4x4_wc_ec;
	static GLint ppuLoc_M4x4_ec_lds;
	static GLint ppuLoc_M4x4_dynamic;

	static GLint ppuLoc_lightPosition;
	static GLint ppuLoc_lightStrength;
	static GLint ppuLoc_actualNumLights;
	static GLint ppuLoc_globalAmbient;
	static GLint ppuLoc_ka;
	static GLint ppuLoc_kd;
	static GLint ppuLoc_ks;
	static GLint ppuLoc_m;
	static GLint ppuLoc_texture;
	static GLint ppuLoc_usingTexture;

	static void fetchGLSLVariableLocations();

	static vec3 _eye;                             //!> The location of the camera in World Coordinates
	static vec3 _center;                          //!> Where the camera is pointed in World Coordinates
	static vec3 _up;                              //!> Vector to define which direction is up
	/*********** PHONG LIGHT MODEL VARS **************/
	static vec4 _lightPosition[numLights];        //!> light source positions
	static float _lightStrength[3 * numLights];   //!> strength of light sources
	static vec4 _ambientStrength;                 //!> stength of assumed ambient light
	/********** END PHONG LIGHT MODEL VARS **********/

	static vec3 _ecmin;                           //!> The model's bounding box mins in eye coordinates
	static vec3 _ecmax;                           //!> The model's bounding box maxs in eye coordinates
	static float _r;                              //!> Bounding sphere radius
	static float _zpp;                            //!> projection plane for perspective projection

	static bool mouseIsDown;
	static int lastMousePosition[2];
	static cryph::AffVector _E;
	static float _EX;
	static float _EY;
	static float _EZ;
	static float _D;
	static float _F;
	static float _frustum;
	static PROJECTION_TYPE _proj_type;
	mat4 _model_view;                            //!> The matrix to convert from eye coordinates to LDS
	mat4 _projection;                            //!> The orthogonal projection matrix
	static bool useWheel;

};

#endif
