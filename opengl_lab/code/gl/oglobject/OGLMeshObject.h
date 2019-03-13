#ifndef OGLMESHOBJECT_H
#define OGLMESHOBJECT_H
#include "gl/OGLObject.h"
#include "gl/OGLShader.h"
#include "math3d/GLMIncludes.h"
#include <GL/gl.h>
#include <vector>

class OGLMeshObject : public OGLObject {
public:
  typedef std::vector<int> Face;  
  typedef glm::vec3 Vertex;
  typedef glm::vec2 TexCoord;
  typedef glm::vec3 Normal;
  
  static const size_t attribLength = 3;
protected:
  int prepareInternalBuffers(const std::vector< OGLMeshObject::Vertex > vertices, const std::vector< OGLMeshObject::Face > faces);
  void prepareAndUploadGLBuffers();
  void unbindBuffers();    
  virtual void getShaderUniforms();
  
  virtual void uploadUniforms(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
  //
  OGLMeshObject(OGLShader* shader, int vertexLength , int verticesPerFace=3); 

  GLuint vao;
  GLuint vbo;
  GLuint ibo;
  OGLShader* shader; 
  
  GLfloat* vertexData;
  size_t vertexCount;
  GLint* faceData;
  size_t faceCount;
  
  int vertexLength;
  int verticesPerFace;
  
  GLuint viewmatrix_location;
  GLuint modelmatrix_location;
  GLuint projection_location;
public:
  OGLMeshObject(OGLShader* shader, const std::vector<Vertex> vertices, const std::vector<Face> faces);
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
  void update() override;
  virtual ~OGLMeshObject();
  
};


#endif

