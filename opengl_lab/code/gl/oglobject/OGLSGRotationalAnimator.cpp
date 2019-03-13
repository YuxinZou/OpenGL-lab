#include "gl/oglobject/OGLSGRotationalAnimator.h"
#include "utility/Variant.h"

OGLSGRotationalAnimator::OGLSGRotationalAnimator(glm::vec3 rotationCenter, glm::vec3 rotationAxis, float rotationSpeed,OGLSceneGraphNode* parent): 
OGLSceneGraphNode(glm::mat4(1.0f),parent), rotationCenter(rotationCenter), rotationAxis(rotationAxis), rotSpeed(rotationSpeed), currentRotation(0.f)
{}




void OGLSGRotationalAnimator::update()
{
  //update the rotation 
  updateRotation();

  OGLSceneGraphNode::update();
}

void OGLSGRotationalAnimator::updateRotation()
{ 
  //compute an updated rotation  - First increase the rotation value (make sure to prevent float overflow)
  currentRotation += rotSpeed;
  if (currentRotation > 2*M_PI) currentRotation-= 2*M_PI;
  if (currentRotation < -2*M_PI) currentRotation+= 2*M_PI;
  
  //compute a matrix transform
  glm::mat4 newrelativeTransform = glm::translate(rotationCenter) * glm::rotate(currentRotation,rotationAxis) * glm::translate(-rotationCenter);
  //set the new transform (make sure all internal data is updated)
  setRelativeTransform(newrelativeTransform);
  
}


