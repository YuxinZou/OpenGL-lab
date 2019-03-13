#ifndef OGLTEXTUREDMESHOBJECT2_H
#define OGLTEXTUREDMESHOBJECT2_H
#include "OGLTexturedMeshObject.h"
#include "gl/OGLShader.h"
#include "gl/OGLTexture.h"

class OGLTexturedMeshObject2: public OGLTexturedMeshObject {
private:
  size_t memOffset;
  size_t memOffset2;
public:
  typedef glm::vec3 Tangent;
  typedef glm::vec3 Binormal;
protected: 
  int prepareInternalBuffers(const std::vector< OGLMeshObject::Vertex > vertices,
			     const std::vector< OGLMeshObject::Normal > normals, 
			     const std::vector< TexCoord > textureCoordinates,
			     const std::vector< Tangent > tangents,
			     const std::vector< Binormal> binormals,
			     const std::vector< OGLMeshObject::Face > faces);
  void prepareAndUploadGLBuffers();
public:
    OGLTexturedMeshObject2(OGLShader* shader, 
			  const std::vector< Vertex > vertices, 
			  const std::vector< Normal > normals, 
			  const std::vector< TexCoord > textureCoordinates,
			  const std::vector< Tangent > tangents,
			  const std::vector< Binormal> binormals,
			  const std::vector< Face > faces, 
			  OGLMaterial material, 
			  std::vector< OGLLightSource* > lightSources,
			  std::vector< TextureBinding > textures);

//     void render(glm::mat4 transform) override;
    static const int attribLength = 3+3;
 
};

#endif