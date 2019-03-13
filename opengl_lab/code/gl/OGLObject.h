#ifndef OGLOBJECT_H
#define OGLOBJECT_H
#include "gl/OGLContext.h"

class OGLObject {
protected:
  OGLContext* context = nullptr;
public:
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) = 0;
  virtual void update() = 0;
  
  virtual void setContext(OGLContext* context) {
    this->context= context;
  }

  virtual OGLContext* getContext() {
      return this->context;
  }
};


#endif 
