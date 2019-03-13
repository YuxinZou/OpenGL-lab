#ifndef OGLVIEWPORT_H
#define OGLVIEWPORT_H

#include "gl/oglobject/OGLSceneGraphNode.h"

class OGLViewport: public OGLSceneGraphNode {
protected:
  bool doUpdate;
public:
  int top;
  int left;
  int width;
  int height; 
  
  

  OGLViewport(int left, int top, int width, int height, bool update = true);
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
  virtual void update() override;
  bool coordinatesInside(int x, int y);
  

};

#endif