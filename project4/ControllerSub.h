#ifndef __CONTROLLERSUB_H__
#define __CONTROLLERSUB_H__

#include "Controller.h"
#include <GL/gl.h>
#include <GL/freeglut.h>
#include "GeneralMV.h"

class ControllerSub : public Controller
{
 public:
  enum {
    ScreenWidth = 360,
    ScreenHeight = 360
  };

 public:
  explicit ControllerSub( const std::string& name, int glutRCFlags = 0 );
  virtual ~ControllerSub();
  bool drawingOpaque() const;
  virtual void handleDisplay();

 private:
  virtual void drawAllOpaqueObjects();
  virtual void drawAllTranslucentObjects();
  
  virtual void establishInitialCallbacksForRC();
  virtual void handleMouseFunc( int button, int state, int x, int y );
  virtual void handleMouseMotion( int x, int y );
  virtual void handleMousePassiveMotion( int x, int y );
  static void mouseFuncCB( int button, int state, int x, int y );
  static void mouseMotionCB( int x, int y );
  static void mousePassiveMotionCB( int x, int y );

  static ControllerSub * curControllerSub;
  bool bDrawOpaque;
};

#endif
