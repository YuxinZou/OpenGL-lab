#ifndef OGLShader_H
#define OGLShader_H

#include "gl/OGLIncludes.h"
#include <string>

class OGLShader {
protected:
  //A Name for the shader to make debug easier
  std::string shaderName;
  
  GLuint shaderProgram;
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint geometryShader;
  
  static bool compileShaderAndPrintErrors(GLuint shader);
  static bool setupShaderFromCodeString(GLuint shader, std::string code);
public:
  OGLShader(std::string shaderName);

  bool loadVertexShaderFromCode(std::string code);
  bool loadFragmentShaderFromCode(std::string code);
  bool loadGeometryShaderFromCode(std::string code);
  
  bool linkShader();
  
  virtual bool bindShader();
  virtual bool unbindShader();
  
  GLuint getId();
  
  
};

#endif 