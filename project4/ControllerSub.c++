#include "ControllerSub.h"
#include "Matrix4x4.h"

using namespace std;

ControllerSub * ControllerSub::curControllerSub = NULL;

ControllerSub::ControllerSub( const std::string& name, int glutRCFlags )
  :
  Controller( name, glutRCFlags )
{
  ControllerSub::curControllerSub = this;
  establishInitialCallbacksForRC();
} /* end ControllerSub::ControllerSub() */

ControllerSub::~ControllerSub()
{
  if( this == curControllerSub )
    curControllerSub = NULL;
} /* end ControllerSub::~ControllerSub() */

void ControllerSub::establishInitialCallbacksForRC() {
  Controller::establishInitialCallbacksForRC();
  
  glutMouseFunc( ControllerSub::mouseFuncCB );
  glutMotionFunc( ControllerSub::mouseMotionCB );
  //glutPassiveMotionFunc( ControllerSub::mousePassiveMotionCB );
} /* end Controller::establishInitialCallbacksForRC() */

void ControllerSub::handleMouseFunc( int button, int state, int x, int y )
{
  double ldsX, ldsY;
  screenXYToLDS( x, y, ldsX, ldsY );

  GeneralMV::handleMouseFunc( button, state, ldsX, ldsY );

  glutPostRedisplay();
} /* end ControllerSub::handleMouseFunc() */

void ControllerSub::handleMouseMotion( int x, int y )
{
  GeneralMV::handleMouseMotion( x, y );
  glutPostRedisplay();
} /* end ControllerSub::handleMouseMotion() */

void ControllerSub::handleMousePassiveMotion( int x, int y )
{
  double ldsX, ldsY;
  screenXYToLDS( x, y, ldsX, ldsY );
  GeneralMV::handleMousePassiveMotion( ldsX, ldsY );
} /* end ControllerSub::handleMousePassiveMotion() */

/* class member function */
void ControllerSub::mouseFuncCB( int button, int state, int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMouseFunc( button, state, x, y );
} /* end ControllerSub::mouseFuncCB() */

/* class member function */
void ControllerSub::mouseMotionCB( int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMouseMotion( x, y );
} /* end ControllerSub::mouseMotionCB() */

/* class member function */
void ControllerSub::mousePassiveMotionCB( int x, int y )
{
  if( curControllerSub != NULL )
    curControllerSub->handleMousePassiveMotion( x, y );
} /* end ControllerSub::mousePassiveMotionCB() */

bool ControllerSub::drawingOpaque() const
{
  return bDrawOpaque;
} /* end ControllerSub::drawingOpaque() */

void ControllerSub::handleDisplay()
{
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_BLEND);
  drawAllOpaqueObjects();

  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  drawAllTranslucentObjects();

  glutSwapBuffers();
} /* end ControllerSub::handleDisplay() */

void ControllerSub::drawAllOpaqueObjects()
{
  bDrawOpaque = true;
  for( std::vector<ModelView*>::iterator it = models.begin(); it != models.end(); ++it )
    (*it)->render();
  //Controller::handleDisplay();
} /* end ControllerSub::drawAllOpaqueObjects() */

void ControllerSub::drawAllTranslucentObjects()
{
  bDrawOpaque = false;
  for( std::vector<ModelView*>::iterator it = models.begin(); it != models.end(); ++it )
    (*it)->render();
  //Controller::handleDisplay();
} /* end ControllerSub::drawAllTranslucentObjects() */
