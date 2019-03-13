#ifndef OGLSGROTATIONALANIMATOR_H
#define OGLSGROTATIONALANIMATOR_H
#include <gl/OGLObject.h>
#include "OGLSceneGraphNode.h"

class OGLSGRotationalAnimator: public OGLSceneGraphNode {
protected:
  void updateRotation();
  
  glm::vec3 rotationCenter;
  glm::vec3 rotationAxis;
  float rotSpeed;
  
  float currentRotation;
  
public: 
  OGLSGRotationalAnimator(glm::vec3 rotationCenter, glm::vec3 rotationAxis, float rotationSpeed, OGLSceneGraphNode* parent = nullptr);
  
  virtual void update() override;
};


#endif