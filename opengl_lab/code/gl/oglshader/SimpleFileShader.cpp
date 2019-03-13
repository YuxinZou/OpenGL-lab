#include "gl/oglshader/SimpleFileShader.h"

#include "utility/Variant.h"

#include <iostream>
#include <fstream>



std::string SimpleFileShader::loadCodeFromFile(std::string filename)
{
  std::string code;

  std::ifstream codeFile;
  codeFile.open(filename);
  
  if (!codeFile.is_open()) {
    throw std::runtime_error(filename+" not found");
  }
  
  
  std::string line;
  while (std::getline(codeFile,line)) {
        code+=line+"\n";
  }
  codeFile.close();

  return code;  
}


SimpleFileShader::SimpleFileShader(std::string shaderName, std::string vertexShaderFilename, std::string fragmentShaderFilename): OGLShader(shaderName)
{
  //Load the vertex/fragment shader source code from file and initialize the OGLShader with them
  loadVertexShaderFromCode(loadCodeFromFile(vertexShaderFilename));
  loadFragmentShaderFromCode(loadCodeFromFile(fragmentShaderFilename));

  //Link the shader  
  linkShader();
  // set address of fragment color output //
  glBindFragDataLocation(shaderProgram, 0, "color");
}

SimpleFileShader::SimpleFileShader(std::string shaderName, std::string vertexShaderFilename, std::string fragmentShaderFilename, 
std::string geometryShaderFilename): OGLShader(shaderName)
{
  loadVertexShaderFromCode(loadCodeFromFile(vertexShaderFilename));
  loadFragmentShaderFromCode(loadCodeFromFile(fragmentShaderFilename));
  loadGeometryShaderFromCode(loadCodeFromFile(geometryShaderFilename));
  
  //Link the shader  
  linkShader();
  // set address of fragment color output //
  glBindFragDataLocation(shaderProgram, 0, "color");
}
