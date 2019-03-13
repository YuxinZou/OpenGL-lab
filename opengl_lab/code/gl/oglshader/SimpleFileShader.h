#ifndef SIMPLEFILESHADER_H
#define SIMPLEFILESHADER_H
#include <gl/OGLShader.h>

class SimpleFileShader: public OGLShader {
protected:
  static std::string loadCodeFromFile(std::string filename);
public:
    SimpleFileShader(std::string shaderName, std::string vertexShaderFilename, std::string fragmentShaderFilename);
    SimpleFileShader(std::string shaderName, std::string vertexShaderFilename, std::string fragmentShaderFilename, std::string geometryShaderFilename); 
};

#endif