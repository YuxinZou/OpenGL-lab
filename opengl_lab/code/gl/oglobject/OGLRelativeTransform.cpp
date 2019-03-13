#include "gl/oglobject/OGLRelativeTransform.h"
#include <iostream>

OGLRelativeTransform::OGLRelativeTransform(OGLSceneGraphNode* reference, OGLSceneGraphNode* parent, bool invertTransform): 
OGLSceneGraphNode(glm::mat4(1.f), parent), reference(reference), inverted(invertTransform)
{}

glm::mat4 OGLRelativeTransform::getRelativeTransform()
{
  glm::mat4 relativeTransform = reference->getTransform();
  if (inverted) {
    relativeTransform = glm::affineInverse(relativeTransform); 
  }
  
  return relativeTransform;
}


glm::mat4 OGLRelativeTransform::getTransform()
{
  if (parent == nullptr) {
    return getRelativeTransform();
  }
  else {
    glm::mat4 parentT = parent->getTransform();
    return parentT*getRelativeTransform();
  }
}



void OGLRelativeTransform::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  glm::mat4 localTransform = model*getRelativeTransform();
  for (OGLObject* object: childNodes) {
    object->render(projection, view, localTransform);
  }
}
