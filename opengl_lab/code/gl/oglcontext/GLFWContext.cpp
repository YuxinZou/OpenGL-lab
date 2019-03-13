#include "GLFWContext.h"
#include "utility/OGLUtils.h"
#include "gl/OGLObject.h"

#include <assert.h>

GLFWContext* GLFWContext::context = nullptr;

GLFWContext::GLFWContext(int& argc, char** argv, int width, int height, std::string windowTitle, int windowPosX, int windowPosY)
{ 
  glfwSetErrorCallback(error_callback);
  
  if (!glfwInit()) {
    throw std::runtime_error("Failed to init glfw");
  }
  
  // TODO: unhardcode
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  //FIXME should not be hardcoded  
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  glfwWindowHint(GLFW_SAMPLES, 4); //TODO make this setable from outside
  
  window = glfwCreateWindow(width, height, windowTitle.c_str() , NULL, NULL);

  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create a glfw window");
  }

  glfwMakeContextCurrent(window);
  
  //Setting up GLEW
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cout << "(glewInit) - Error: " << glewGetErrorString(err) << std::endl;
    assert(false);
  }
  std::cout << "(glewInit) - Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
  
  //Here we can experience a OpenGl Error which apparently can be ignored
  Utils::glError("GLEW initialization",false);

  GLFWContext::context = this;
  
  glfwSetKeyCallback(window, GLFWContext::key_callback);
  glfwSetCursorPosCallback(window, GLFWContext::mouseMoveEventCallback);
  glfwSetMouseButtonCallback(window, GLFWContext::mouseEventCallback);
  glfwSetFramebufferSizeCallback(window, GLFWContext::framebuffer_size_callback);

 
  //Some Info
  std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
}

void GLFWContext::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  context->key_callback(key,scancode,action,mods);
}

void GLFWContext::key_callback(int key, int scancode, int action, int mods)
{
  for (GLFWInputEventHandler* handler: inputEventHandler) {
    handler->keyboardEvent(key,scancode,action,mods);
  }
}

void GLFWContext::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  context->framebuffer_size_callback(width,height);
 //   std::cout << "[Static] FB resize event forwarded\n";
}
 
void GLFWContext::framebuffer_size_callback(int width, int height)
{
  for (GLFWReshapeEventHandler* handler: reshapeEventHandler) {
    handler->framebufferSizeCallback(width,height);
 //     std::cout << "[internally] FB resize event forwarded\n";
  }
}


void GLFWContext::mouseEventCallback(GLFWwindow* window, int button, int state, int mods)
{
  context->mouseEventCallbackm(button,state,mods);
}

void GLFWContext::mouseMoveEventCallback(GLFWwindow* window, double x, double y)
{
  context->mouseMoveEventCallbackm(x,y);
}

void GLFWContext::mouseWheelEventCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  context->mouseWheelEventCallbackm(xoffset, yoffset);
}

void GLFWContext::renderCall()
{
  //FIXME: implement
  assert(false);
}

void GLFWContext::renderLoop()
{
  while(!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      displayCallbackm();
  }
}

void GLFWContext::displayCallbackm()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (OGLObject* obj: objects) {
    obj->update(); 
  }

  for (OGLObject* obj: objects) {
    obj->render(glm::mat4(1.0),glm::mat4(1.0), glm::mat4(1.0)); 
  }
  
  // swap renderbuffers for smooth rendering //
  glfwSwapBuffers(window);
}

void GLFWContext::mouseEventCallbackm(int button, int state, int mods)
{
  for (GLFWInputEventHandler* handler: inputEventHandler) {
     handler->mouseButtonEvent(button,state,mods);
  }
}

void GLFWContext::mouseMoveEventCallbackm(double x, double y)
{
   for (GLFWInputEventHandler* handler: inputEventHandler) {
     handler->mouseMoveEvent(x,y);
   }
}

void GLFWContext::mouseWheelEventCallbackm(double xoffset, double yoffset)
{
  for (GLFWInputEventHandler* handler: inputEventHandler) {
     handler->mouseScroll(xoffset, yoffset);
   }
}

void GLFWContext::error_callback(int error, const char* description)
{
  std::cerr << "GLFW Error: " << description << "(Code " << std::to_string(error) << ")\n";
}

GLFWContext::~GLFWContext()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}


void GLFWContext::addInputEventHandler(GLFWInputEventHandler* handler)
{
  inputEventHandler.insert(handler);
}

void GLFWContext::removeInputEventHandler(GLFWInputEventHandler* handler)
{
  inputEventHandler.erase(inputEventHandler.find(handler));
}

void GLFWContext::addReshapeEventHandler(GLFWReshapeEventHandler* reshapeHandler)
{
  reshapeEventHandler.insert(reshapeHandler);
  //tell the reshapeEventHandler once about the initial configuration
  int fbwidth, fbheight;
  glfwGetFramebufferSize(window,&fbwidth,&fbheight);
  reshapeHandler->framebufferSizeCallback(fbwidth,fbheight);

  int winwidth, winheight;
  glfwGetWindowSize(window, &winwidth, &winheight);
  reshapeHandler->windowSizeCallback(winwidth, winheight);
}

void GLFWContext::removeReshapeEventHandler(GLFWReshapeEventHandler* reshapeHandler)
{
  reshapeEventHandler.erase(reshapeEventHandler.find(reshapeHandler));
}

