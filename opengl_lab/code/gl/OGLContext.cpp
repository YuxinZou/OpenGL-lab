#include "gl/OGLContext.h"
#include "gl/OGLObject.h"
#include <algorithm>

void OGLContext::addObject(OGLObject* object)
{
  objects.push_back(object);
  object->setContext(this);
}

void OGLContext::removeObject(OGLObject* object)
{
  objects.erase(std::remove(objects.begin(), objects.end(), object));
  object->setContext(nullptr);
}


// glm::mat4 OGLContext::getProjectionMatrix()
// {
//   return projectionMatrix;
// }
// 
// void OGLContext::setProjectionMatrix(glm::mat4 newProjectionMatrix)
// {
//   projectionMatrix = newProjectionMatrix;
// }
