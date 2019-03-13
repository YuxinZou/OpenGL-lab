#ifndef OGLUTILS_H
#define OGLUTILS_H
#include <GL/gl.h>
#include <iostream>


#include "math3d/GLMIncludes.h"

#include "gl/OGLObject.h"
#include "gl/OGLShader.h"

#define UNIFORM_CHECK(UNIFORM) if (UNIFORM< 0) std::cerr << "WARNING: Uniform location >"#UNIFORM"< was not found or setup correctly. (Optimized out?)"<<std::endl;

namespace Utils {

/// Checks for OpenGL Errors and prints given message if an error was detected
bool glError(std::string msg, bool haltOnError = true);

//////////////////////////////////////////////////////////////////////////////
//           SimpleColorShader                                              //
//////////////////////////////////////////////////////////////////////////////


class SimpleColorShader: public OGLShader {
public:
  SimpleColorShader (std::string shaderName);  
};


//////////////////////////////////////////////////////////////////////////////
//           SimpleColoredTriangle                                          //
//////////////////////////////////////////////////////////////////////////////

class SimpleColoredTriangle: public OGLObject {
protected:
  GLuint vao;
  GLuint vbo;
  GLuint ibo;
  OGLShader* shader; 
public:
  SimpleColoredTriangle(OGLShader* shader);
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
  void update() override;
    
};

}
#endif