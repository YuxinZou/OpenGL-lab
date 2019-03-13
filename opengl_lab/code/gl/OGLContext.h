#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H
#include <vector>
#include "math3d/GLMIncludes.h"

class OGLObject;

class OGLContext {
protected:
  std::vector<OGLObject*> objects;
  
//   glm::mat4 projectionMatrix;
public:
  virtual void addObject(OGLObject* object);
  virtual void removeObject(OGLObject* object);
  
//   virtual void setProjectionMatrix(glm::mat4 newProjectionMatrix);
//   virtual glm::mat4 getProjectionMatrix();
  
  //does a simple rendercall and returns
  virtual void renderCall() = 0;
  
  //starts a render loop - this function does not return until the render context was destroyed
  virtual void renderLoop() = 0;
  
};


#endif