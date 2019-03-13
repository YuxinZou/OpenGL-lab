#ifndef OGLTEXTUREDMESHOBJECT_H
#define OGLTEXTUREDMESHOBJECT_H
#include "OGLNormalMeshObject.h"
#include "gl/OGLShader.h"
#include "gl/OGLTexture.h"

class OGLTexturedMeshObject: public OGLNormalMeshObject {
private:
  size_t memOffset;
public:
  typedef std::pair<OGLTexture*,const char*> TextureBinding;
protected: 
  std::vector < TextureBinding> textures;
  std::vector < GLint > textureUniformLocations;
  
  int prepareInternalBuffers(const std::vector< OGLMeshObject::Vertex > vertices,
			     const std::vector< OGLMeshObject::Normal > normals, 
			     const std::vector< TexCoord > textureCoordinates,
			     const std::vector< OGLMeshObject::Face > faces);
  void prepareAndUploadGLBuffers();
  OGLTexturedMeshObject(OGLShader* shader, 
			  const std::vector< Vertex > vertices, 
			  const std::vector< Normal > normals, 
			  const std::vector< TexCoord > textureCoordinates,
			  const std::vector< Face > faces, 
			  OGLMaterial material, 
			  std::vector< OGLLightSource* > lightSources,
			  std::vector< TextureBinding > textures,
			  int vertexLength);
public:
    OGLTexturedMeshObject(OGLShader* shader, 
			  const std::vector< Vertex > vertices, 
			  const std::vector< Normal > normals, 
			  const std::vector< TexCoord > textureCoordinates,
			  const std::vector< Face > faces, 
			  OGLMaterial material, 
			  std::vector< OGLLightSource* > lightSources,
			  std::vector< TextureBinding > textures);
    void uploadUniforms(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override ;
    void getShaderUniforms() override;
    virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
    
    static const int attribLength = 2;
 
};

#endif