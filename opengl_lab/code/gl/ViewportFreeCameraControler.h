#ifndef VIEWPORTFREECAMERACONTROLER_H
#define VIEWPORTFREECAMERACONTROLER_H

#include "gl/FreeCameraControler.h"
#include "gl/OGLViewport.h"

class ViewportFreeCameraControler: public FreeCameraControler {

protected:
  OGLViewport& assignedViewport;
  bool viewportSelected;
  
public:
  ViewportFreeCameraControler(glm::vec3 position, OGLViewport& assignedViewport, float theta = 0, float phi = 0, float moveSpeed = 1.f, float mouseSpeed = 0.01f);
  //void keyboardEvent(unsigned char key, int x, int y) override;
  //void mouseEvent(int button, int state, int x ,int y) override;
  virtual void keyboardEvent(int key, int scancode, int action, int mods) override;
  virtual void mouseButtonEvent(int button, int action, int mods) override;
  
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
};

#endif
