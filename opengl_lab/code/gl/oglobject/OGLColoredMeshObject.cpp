#include "gl/oglobject/OGLColoredMeshObject.h"
#include "utility/Variant.h"

OGLColoredMeshObject::OGLColoredMeshObject(OGLShader* shader, const std::vector<  Vertex> vertices, const std::vector<Color > colors, const std::vector<  Face > faces, const int verticesPerFace): 
OGLMeshObject(shader, OGLMeshObject::attribLength+OGLColoredMeshObject::attribLength,verticesPerFace)
{
  size_t memOffset = prepareInternalBuffers(vertices,faces);
  assert (vertices.size() == colors.size());
  
  //memory has been allocated by parentclass - copy data to the prepared location (behind vertex coordinates)
  for (size_t i = 0; i < colors.size(); i++) {
    vertexData[memOffset+i*attribLength+0] = colors.at(i).x;
    vertexData[memOffset+i*attribLength+1] = colors.at(i).y;
    vertexData[memOffset+i*attribLength+2] = colors.at(i).z;
  }
  
  //Upload data to OpenGL
  prepareAndUploadGLBuffers();
  
  //define and enable the vertex attribute pointer for vertex color
  glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,0, (void*)(memOffset*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  //Unbind buffers to leave a clean state behind
  unbindBuffers();
}
