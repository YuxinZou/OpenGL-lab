#ifndef OGLNORMALMESHOBJECT_H
#define OGLNORMALMESHOBJECT_H
#include "OGLMeshObject.h"
#include <GL/gl.h>
#include "gl/OGLLightSource.h"
#include "gl/OGLMaterial.h"

class OGLNormalMeshObject: public OGLMeshObject {
private:
  size_t memOffset;
protected:
  void getShaderUniforms() override;
  void uploadUniforms(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
  
  int prepareInternalBuffers(const std::vector< OGLMeshObject::Vertex > vertices,
			     const std::vector< OGLMeshObject::Normal > normals, 
			     const std::vector< OGLMeshObject::Face > faces);
  void prepareAndUploadGLBuffers();
  
  std::vector<OGLLightSource*> lightSources;
  std::vector<OGLLightSource::LightSourceLocation> lightSourceLocations;
  OGLMaterial material;
  OGLMaterial::OGLMaterialLocation materialLocation;
  
  GLuint lightsInUseLocation;
  GLuint viewMatrixLocation;
  
  OGLNormalMeshObject(OGLShader* shader, const std::vector< OGLMeshObject::Vertex > vertices, 
		      const std::vector< OGLNormalMeshObject::Normal > normals, 
		      const std::vector< OGLMeshObject::Face > faces, OGLMaterial material, std::vector< OGLLightSource* > lightSources, int vertexLength);
  
public:
  typedef glm::vec3 Normal;
  OGLNormalMeshObject(OGLShader* shader, const std::vector< OGLMeshObject::Vertex > vertices, const std::vector< OGLNormalMeshObject::Normal > normals, const std::vector< 
OGLMeshObject::Face > faces, OGLMaterial material, std::vector< OGLLightSource* > lightSources = {});
  static const int attribLength = 3;
};

#endif