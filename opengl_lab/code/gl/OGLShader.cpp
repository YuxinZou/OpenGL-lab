#include "gl/OGLShader.h"
#include "gl/OGLIncludes.h"
#include <iostream>

OGLShader::OGLShader(std::string shaderName):shaderName(shaderName)
{
  shaderProgram = glCreateProgram();
  vertexShader  = 0;
  fragmentShader= 0;
  geometryShader= 0;
}

bool OGLShader::compileShaderAndPrintErrors(GLuint shader)
{
  glCompileShader(shader);
  
  int logMaxLength;
  int logLength;
  
   
  GLint isCompiled = 0;
  glGetShaderiv(shader,GL_COMPILE_STATUS,&isCompiled);
  
  
  bool hasErrors = (isCompiled==GL_FALSE);
  
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logMaxLength);
  char* log = new char[logMaxLength]; 
  glGetShaderInfoLog(shader,logMaxLength, &logLength, log);
  if (logLength> 0) {
    std::cout << "Shader Compilation - Compiler log:\n------------------\n" << log <<  "==================" << std::endl;
  }
  delete[] log;
  
  
  return !hasErrors;
}

bool OGLShader::setupShaderFromCodeString(GLuint shader, std::string code)
{
  const char* shader_src = code.c_str();
  glShaderSource(shader,1,(const char**)&shader_src, NULL);
  
  bool compiled = compileShaderAndPrintErrors(shader);
  if (!compiled) {
    std::cerr << "An error occured during shader compilation - code was:\n" << code << std::endl;
  }
  
  return compiled;
}


bool OGLShader::loadFragmentShaderFromCode(std::string code)
{
  if (fragmentShader==0) {
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  }
  //Compile Shader
  bool result = setupShaderFromCodeString(fragmentShader,code);
  
  //Attach shader to the program 
  glAttachShader(shaderProgram,fragmentShader);
  
  return result;
}


bool OGLShader::loadGeometryShaderFromCode(std::string code)
{
  if (geometryShader==0) {
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  }
  //Compile Shader
  bool result = setupShaderFromCodeString(geometryShader,code);
  
  //Attach shader to the program 
  glAttachShader(shaderProgram,geometryShader);

  
  return result;
}

bool OGLShader::loadVertexShaderFromCode(std::string code)
{
  if (vertexShader==0) {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
  }
  //Compile Shader
  bool result = setupShaderFromCodeString(vertexShader,code);
  
  //Attach shader to the program 
  glAttachShader(shaderProgram,vertexShader);
  
  return result;
}

bool OGLShader::linkShader()
{
  glLinkProgram(shaderProgram);
  
  GLint isLinked = 0;
  glGetProgramiv(shaderProgram,GL_LINK_STATUS,&isLinked);
  
  
  bool hasErrors = (isLinked==GL_FALSE);
  
  // get log //
  int logMaxLength = 0;
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logMaxLength);
  char* log = new char[logMaxLength]();
  int logLength = 0;
  glGetProgramInfoLog(shaderProgram, logMaxLength, &logLength, log);
  if (logLength > 0) {
    std::cout << "Shader Compilation - Linker log:\n------------------\n" << log << "\n------------------" << std::endl;
  }
  delete[] log;
  
  return !hasErrors;
}


bool OGLShader::bindShader()
{
  glUseProgram(shaderProgram);  
  return true;
}

bool OGLShader::unbindShader()
{
  glUseProgram(0);
  return true;
}


GLuint OGLShader::getId()
{
  return shaderProgram;
}


