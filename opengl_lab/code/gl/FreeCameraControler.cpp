#include "gl/FreeCameraControler.h"
#include <iostream>
#include "utility/Variant.h"

FreeCameraControler::FreeCameraControler(glm::vec3 position, float theta, float phi, float moveSpeed, float mouseSpeed): 
OGLSceneGraphNode(glm::mat4(1.f),nullptr),cameraPosition(position), theta(theta), phi(phi), lastTheta(theta), lastPhi(phi), rightMouseButton(false), speed(moveSpeed), mouseSpeed(mouseSpeed)
{
  updateModelView();
  resetIntrinsics();
}

void FreeCameraControler::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  glViewport(0,0,m_width, m_height);  
  
  glm::mat4 projectionMatrix = getProjectionMat();

  for (OGLObject* obj: childNodes) {
    obj->render(projectionMatrix,relativeTransform,glm::mat4(1.0));
  }
}

void FreeCameraControler::resetIntrinsics(float angle, float aspect, float near, float far)
{
  openAngle = angle;
  this->aspect = aspect;
  this->near = near;
  this->far = far;
}

void FreeCameraControler::resetOrientation(float theta, float phi)
{
  this->theta = theta;
  this->phi = phi;
  updateModelView();
}

void FreeCameraControler::mouseButtonEvent(int button, int state, int mods)
{
   double x = currentx;
   double y = currenty;
  // If the Right Button (GLUT_RIGHT_BUTTON) is pressed (GLUT_DOWN) store the mouse start position.
  // On release (GLUT_UP) store last angles phi and theta
  // Store the information about the current button state in the member variable rightMouseButton
  switch(button) {
    case GLFW_MOUSE_BUTTON_RIGHT: { //Right Button pressed
      if (state == GLFW_PRESS) {
        startX = x;
        startY = y;
        rightMouseButton = true;
      }
      else {
        lastPhi = phi;
        lastTheta = theta;
        rightMouseButton= false;
      }
      break;
    }
    default: break;
  }
}

void FreeCameraControler::updateModelView()
{
  //compute the new model view matrix and set as relativeTransform
  glm::vec3 lookDir(sin(theta) * cos(phi), sin(phi), cos(theta) * cos(phi));
  glm::vec3 upDir (sin(theta)* sin(phi)*-1,cos(phi),cos(theta)*sin(phi)*-1);
  setRelativeTransform(glm::lookAt(cameraPosition, cameraPosition - lookDir, upDir));
}


void FreeCameraControler::mouseMoveEvent(double x, double y)
{
  GLFWInputEventHandler::mouseMoveEvent(x,y);

  if (rightMouseButton) {
    // compute the angles theta (around screen y-axis) and phi(arround screen x-axis) from mouse positions
    // use the mouseSpeed to convert pixel difference to angle
    // make sure you restrict phi to [-PI/2,PI/2] to prevent upside down camera
    theta = lastTheta + (startX -x)*  mouseSpeed;
    
    phi = lastPhi - (startY- y) * mouseSpeed;
    phi = std::max(std::min((float)(M_PI/2),phi),(float)(-M_PI/2));
    

    //call for an update of the modelView matrix so we get live feedback while moving the mouse
    updateModelView();
  }
}

void FreeCameraControler::keyboardEvent(int key, int scancode, int action, int mods)
{
  bool positionChanged = false;
  //TODO react to keyboard event (implement wasd cotrol + rf for up and down)
  // calculate the correct axis to move along - use the speed value to control movement speed
  switch (key) {
    case GLFW_KEY_W: {
      //move the camera
      cameraPosition -= glm::vec3(sin(theta)*cos(phi), sin(phi), cos(theta)*cos(phi)) * speed; 
      positionChanged=true;
      break;
    }
    case GLFW_KEY_S: {
      //move the camera
      cameraPosition += glm::vec3(sin(theta)*cos(phi), sin(phi), cos(theta)*cos(phi)) * speed; 
      positionChanged=true;
      break;
    }
    case GLFW_KEY_D: {
      // move the camera - note that phi does not influence movement to the side
      cameraPosition += glm::vec3(cos(theta), 0, -sin(theta)) * speed;
      positionChanged=true;
      break;
    }
    case GLFW_KEY_A: {
      // move the camera - note that phi does not influence movement to the side
      cameraPosition -= glm::vec3(cos(theta), 0, -sin(theta)) * speed;
      positionChanged=true;
      break;
    }
    case GLFW_KEY_R: {
      //move the camera - note that up/down movement is not influenced by the camera rotation
      cameraPosition += glm::vec3(0,speed,0);
      positionChanged=true;
      break;
    }
    case GLFW_KEY_F: {
      //move the camera - note that up/down movement is not influenced by the camera rotation
      cameraPosition += glm::vec3(0,-speed,0);
      positionChanged=true;
      break;
    }
    default: break; //other keys are okay but we do not use them  
  }

  if (positionChanged) updateModelView();
}

void FreeCameraControler::framebufferSizeCallback(int width, int height)
{
  m_width = width;
  m_height = height;
  if (width > 0 && height > 0)
    aspect = (float) width / height;
  else
    aspect = 1;
  projectionMatrix = glm::perspective(openAngle, aspect, near, far);
}

glm::mat4 FreeCameraControler::getProjectionMat(void)
{
  return glm::perspective(openAngle, aspect, near, far);
}

glm::vec3 FreeCameraControler::getCameraPosition(void)
{
  return cameraPosition;
}

glm::mat4 FreeCameraControler::getModelViewMat(void)
{
  return relativeTransform;
}


glm::mat4 FreeCameraControler::getModelTransform() {
  return glm::mat4(1.0);
}

