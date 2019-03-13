#ifndef GLFWCONTEXT_H
#define GLFWCONTEXT_H

#include <string>
#include <map>
#include <set>
#include "gl/OGLIncludes.h"
#include "gl/OGLContext.h"

class GLFWInputEventHandler {
 protected:
   double currentx;
   double currenty;
 public:
   virtual void keyboardEvent(int key, int scancode, int action, int mods) {}
   virtual void mouseMoveEvent(double x, double y) {currentx=x; currenty=y;}
   virtual void mouseButtonEvent(int button, int state, int mods) {}
   virtual void mouseEnter (int entered) {}
   virtual void mouseScroll (double dx, double dy) {}
   virtual void charCallback(unsigned int codepoint) {}
};

class GLFWReshapeEventHandler {
public:
  virtual void framebufferSizeCallback(int width, int height) {}
  virtual void windowSizeCallback(int width, int height) {}
};

class GLFWContext: public OGLContext {
protected:
  static std::map<GLFWwindow*,GLFWContext*> contextByWindow;

  void mouseEventCallbackm(int button, int state, int mods);
  void mouseMoveEventCallbackm(double x, double y);
  void mouseWheelEventCallbackm(double xoffset, double yoffset);
  void displayCallbackm();
  void idleCallbackm();

  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  virtual void key_callback(int key, int scancode, int action, int mods);
  static void mouseMoveEventCallback(GLFWwindow* window, double x, double y);
  static void mouseEventCallback(GLFWwindow* window, int button, int state, int mods);
  static void mouseWheelEventCallback(GLFWwindow* window, double xoffset, double yoffset);
  
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  virtual void framebuffer_size_callback(int width, int height);

  static GLFWContext* context;
  GLFWwindow* window;
  std::set<GLFWInputEventHandler*> inputEventHandler;
  std::set<GLFWReshapeEventHandler*> reshapeEventHandler;

public:
  GLFWContext(int& argc, char** argv, int windowWidth, int windowHeight, std::string windowTitle, int windowPosX = 0, int windowPosY = 0);
  
  void renderCall() override;
  void renderLoop() override;
  
  static void error_callback(int error, const char* description);
  virtual ~GLFWContext();
  void addInputEventHandler(GLFWInputEventHandler* handler);
  void removeInputEventHandler(GLFWInputEventHandler* handler);

  virtual void addReshapeEventHandler(GLFWReshapeEventHandler* reshapeHandler);
  virtual void removeReshapeEventHandler(GLFWReshapeEventHandler* reshapeHandler);

};

#endif
