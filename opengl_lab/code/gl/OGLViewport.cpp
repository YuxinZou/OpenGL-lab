#include "gl/OGLViewport.h"
#include "gl/OGLIncludes.h"

OGLViewport::OGLViewport(int left, int top, int width, int height, bool update): 
OGLSceneGraphNode(glm::mat4(1.f)), left(left),top(top),width(width),height(height),doUpdate(update)
{}

void OGLViewport::update()
{
  if (doUpdate)
  OGLSceneGraphNode::update();
}


void OGLViewport::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  glViewport(left,top,width,height);
  OGLSceneGraphNode::render(projection,view,model);
}


bool OGLViewport::coordinatesInside(int x, int y)
{
  return (x>= left) && (y>=top) && (x<left+width) && (y<top+height);
}
