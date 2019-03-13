#include "gl/oglobject/OGLTexturedMeshObject.h"
#include "utility/Variant.h"

OGLTexturedMeshObject::OGLTexturedMeshObject(OGLShader* shader, const std::vector< OGLMeshObject::Vertex > vertices, const std::vector< OGLNormalMeshObject::Normal > 
normals, const std::vector< OGLMeshObject::TexCoord > textureCoordinates, const std::vector< OGLMeshObject::Face > faces, OGLMaterial material, std::vector< 
OGLLightSource* > lightSources, std::vector< OGLTexturedMeshObject::TextureBinding > textures): 
					     OGLNormalMeshObject(shader, 
								 vertices, normals, faces,
			                                         material, lightSources, 
                                                                 OGLMeshObject::attribLength+OGLNormalMeshObject::attribLength+OGLTexturedMeshObject::attribLength),
					     textures(textures)
{
  prepareInternalBuffers(vertices,normals,textureCoordinates,faces);
  prepareAndUploadGLBuffers();
  
  //Unbind buffers to leave a clean state behind
  unbindBuffers();
  
  getShaderUniforms();
}

OGLTexturedMeshObject::OGLTexturedMeshObject(OGLShader* shader, 
					     const std::vector< OGLMeshObject::Vertex > vertices, 
					     const std::vector< OGLNormalMeshObject::Normal > normals, 
					     const std::vector< OGLMeshObject::TexCoord > textureCoordinates, 
                                             const std::vector< OGLMeshObject::Face > faces, 
					     OGLMaterial material, std::vector< OGLLightSource* > lightSources, 
                                             std::vector< OGLTexturedMeshObject::TextureBinding > textures, int vertexLength):
                                                   OGLNormalMeshObject(shader, 
								       vertices, normals, faces, 
								       material, lightSources, vertexLength),
					           textures(textures)
{

}


void OGLTexturedMeshObject::prepareAndUploadGLBuffers()
{
    //Upload data to OpenGL
  OGLNormalMeshObject::prepareAndUploadGLBuffers();
  
  //define and enable the vertex attribute pointer for vertex color
  //use memOffset as offset in the data (remember to multiply with the size of the datatype)
  glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE,0, (void*)(memOffset*sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

}

int OGLTexturedMeshObject::prepareInternalBuffers(const std::vector< OGLMeshObject::Vertex > vertices, 
						  const std::vector< OGLMeshObject::Normal > normals, 
						  const std::vector< OGLMeshObject::TexCoord > textureCoordinates, 
						  const std::vector< OGLMeshObject::Face > faces)
{
  memOffset = OGLNormalMeshObject::prepareInternalBuffers(vertices,normals,faces);
  //here the Buffer has vertices and normal
  assert(vertices.size() == textureCoordinates.size());
  
  //memory has been allocated by parentclass - copy data to the prepared location (behind vertex normales)
  for (size_t i = 0; i < textureCoordinates.size(); i++) {
    vertexData[memOffset+i*attribLength+0] = textureCoordinates.at(i).x;
    vertexData[memOffset+i*attribLength+1] = textureCoordinates.at(i).y;
  }
  return memOffset+textureCoordinates.size()*attribLength; //return for future child classes
  //add the VertexData to the buffer and updata the offest, teture has only  2 coordinates
}


void OGLTexturedMeshObject::getShaderUniforms()
{
    OGLNormalMeshObject::getShaderUniforms();
    // go through the textures vector and get the uniform location for every texture 
    // store the uniforms in textureUniformLocations
    // clear the textureUniformLocations before filling 
    textureUniformLocations.clear();
    for (int i = 0; i < textures.size(); i++) {
      textureUniformLocations.push_back(glGetUniformLocation(shader->getId(),textures.at(i).second));
    }    
}


void OGLTexturedMeshObject::uploadUniforms(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
    OGLNormalMeshObject::uploadUniforms(projection, view, model);
    //upload the texture unit ids to the uniform locations in textureUniformLocations
    for (int i = 0; i < textures.size(); i++) {
      glUniform1i(textureUniformLocations.at(i), i); 
    }
    
}

void OGLTexturedMeshObject::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  for (int i = 0 ; i < textures.size(); i++) {
    // activate the texture unit (note that the i-th texture unit can be addressed as GL_TEXTURE0+i [guaranteed by definition, see khronos.org])
    // then bind the texture
    glActiveTexture(GL_TEXTURE0+i);
    textures.at(i).first->bindTexture();
  }
  //render the object
  OGLNormalMeshObject::render(projection, view, model);
}
