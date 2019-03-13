#include "gl/oglobject/OGLTexturedMeshObject2.h"

OGLTexturedMeshObject2::OGLTexturedMeshObject2(OGLShader* shader, 
					       const std::vector< OGLMeshObject::Vertex > vertices, 
					       const std::vector< OGLNormalMeshObject::Normal > normals, 
					       const std::vector< OGLMeshObject::TexCoord > textureCoordinates, 
					       const std::vector< OGLTexturedMeshObject2::Tangent > tangents, 
					       const std::vector< OGLTexturedMeshObject2::Binormal > binormals, 
					       const std::vector< OGLMeshObject::Face > faces, 
					       OGLMaterial material, std::vector< OGLLightSource* > lightSources, 
					       std::vector< OGLTexturedMeshObject::TextureBinding > textures): 
					       
					       OGLTexturedMeshObject(shader, vertices, normals, textureCoordinates, faces, material, lightSources, textures, 
                                                      OGLMeshObject::attribLength+OGLNormalMeshObject::attribLength+
                                                      OGLTexturedMeshObject::attribLength+OGLTexturedMeshObject2::attribLength)
{
  prepareInternalBuffers(vertices,normals,textureCoordinates,tangents,binormals,faces);
  prepareAndUploadGLBuffers();
  
  //Unbind buffers to leave a clean state behind
  unbindBuffers();
  
  getShaderUniforms();

}

// void OGLTexturedMeshObject2::render(glm::mat4 transform)
// {
//   uploadUniforms(transform);
//     OGLTexturedMeshObject::render(transform);
// }



int OGLTexturedMeshObject2::prepareInternalBuffers(const std::vector< OGLMeshObject::Vertex > vertices, 
						   const std::vector< OGLMeshObject::Normal > normals, 
						   const std::vector< OGLMeshObject::TexCoord > textureCoordinates, 
						   const std::vector< OGLTexturedMeshObject2::Tangent > tangents, 
						   const std::vector< OGLTexturedMeshObject2::Binormal > binormals, 
						   const std::vector< OGLMeshObject::Face > faces)
{
  memOffset = OGLTexturedMeshObject::prepareInternalBuffers(vertices,normals,textureCoordinates,faces);
  memOffset2= memOffset+(tangents.size()*3);
  
  assert(vertices.size() == tangents.size());
  assert(vertices.size() == binormals.size());
  
  //memory has been allocated by parentclass - copy data to the prepared location (behind vertex normales)
  for (size_t i = 0; i < tangents.size(); i++) {
    vertexData[memOffset+i*3+0] = tangents.at(i).x;
    vertexData[memOffset+i*3+1] = tangents.at(i).y;
    vertexData[memOffset+i*3+2] = tangents.at(i).z;
    vertexData[memOffset2+i*3+0] = binormals.at(i).x;
    vertexData[memOffset2+i*3+1] = binormals.at(i).y;
    vertexData[memOffset2+i*3+2] = binormals.at(i).z;
  }
  return memOffset+tangents.size()*attribLength; //return for future child classes
}

void OGLTexturedMeshObject2::prepareAndUploadGLBuffers()
{
    //Upload data to OpenGL
  OGLTexturedMeshObject::prepareAndUploadGLBuffers();
  
  glVertexAttribPointer(3,3,GL_FLOAT, GL_FALSE,0, (void*)(memOffset*sizeof(GLfloat)));
  glVertexAttribPointer(4,3,GL_FLOAT, GL_FALSE,0, (void*)(memOffset2*sizeof(GLfloat)));
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
}
