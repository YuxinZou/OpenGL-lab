#include "gl/ViewportFreeCameraControler.h"
#include <iostream>
#include "utility/Variant.h"
//#include "gl/FreeCameraControler.h"
ViewportFreeCameraControler::ViewportFreeCameraControler(glm::vec3 position, OGLViewport& assignedViewport, float theta, float phi, float moveSpeed, float mouseSpeed): 
FreeCameraControler(position, theta, phi, moveSpeed, mouseSpeed),
assignedViewport(assignedViewport)
{
  framebufferSizeCallback(assignedViewport.width, assignedViewport.height);
}

//void ViewportFreeCameraControler::keyboardEvent(unsigned char key, int x, int y)
void ViewportFreeCameraControler::keyboardEvent(int key, int scancode, int action, int mods)
{
  double x = currentx;
  double y = currenty;

  // Allow keyboardEvent handling by the FreeCameraControler if 
  // - current coordinates are inside the assignedViewport (see Viewport Header for convenience method for this check)
  // OR
  // - we are currently controlling the rotation with this camera controler (right mouse button was pressed inside the viewport and might have moved outside)
  // Review the FreeCameraControler for state variables required
  if ((rightMouseButton) || (assignedViewport.coordinatesInside(x,y))) {
    FreeCameraControler::keyboardEvent(key, scancode, action, mods);
  }
  
  
}

//void ViewportFreeCameraControler::mouseEvent(int button, int state, int x, int y)
void ViewportFreeCameraControler::mouseButtonEvent(int button, int action, int mods)
{
  double x = currentx;
  double y = currenty;

  //Allow mouseEvent handling by the FreeCameraControler if 
  // - current coordinates are inside the assignedViewport (see Viewport Header for convenience method for this check)
  // OR
  // - if the event was triggered by releasing the mouse button (prevents sticky mouse button if the cursor was moved outside the viewport during camera navigation)
  if ((action == GLFW_RELEASE) || (assignedViewport.coordinatesInside(x,y))) {
    FreeCameraControler::mouseButtonEvent(button, action, mods);
  }
  
  std::cout << theta << " " << phi << std::endl;
}


void ViewportFreeCameraControler::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  glm::mat4 projectionMatrix = getProjectionMat();

  for (OGLObject* obj: childNodes) {
    obj->render(projectionMatrix,relativeTransform,glm::mat4(1.0));
  }
}
