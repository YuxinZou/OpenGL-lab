#include "gl/OGLIncludes.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "utility/OGLUtils.h"
#include <algorithm>
#include <iostream>
#include "utility/Variant.h"

void OGLMeshObject::unbindBuffers()
{
  // unbind buffers //
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int OGLMeshObject::prepareInternalBuffers(const std::vector< Vertex > vertices, const std::vector< Face > faces)
{
  vertexCount = vertices.size();
  faceCount = faces.size();
  
  
  // create a buffer with a suitable size  (use class member vertexData) 
  // put the vertex coordinates in the buffer. further attributs follow after the last vertex
  // | v_1 | v_2 | ... | v_n | [further attribs to be set by child classes]
  if (vertexData != nullptr) delete[] vertexData;
  vertexData = new GLfloat[vertexCount*vertexLength]; 
  for (size_t i = 0; i < vertices.size(); i++) {
    vertexData[i*attribLength+0] = vertices.at(i).x;
    vertexData[i*attribLength+1] = vertices.at(i).y;
    vertexData[i*attribLength+2] = vertices.at(i).z;
  }
  
  //create a buffer for the face indices (use class member faceData)
  if (faceData != nullptr) delete[] faceData;
  faceData = new GLint[faceCount*verticesPerFace];
  //copy indices to array (you can assume that only triangles are passed)
  for (size_t i = 0; i < faces.size(); i++) {
    for (int j  = 0; j < verticesPerFace; j++) {
      faceData[i*verticesPerFace+j] = faces.at(i).at(j);
    }
  }
  
  return vertexCount*attribLength;
}

void OGLMeshObject::prepareAndUploadGLBuffers()
{ 
  //create a Vertex Array Object
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);
 
  //create a Vertex Buffer Object
  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  
  //upload data to OpenGL
  glBufferData(GL_ARRAY_BUFFER,vertexCount*vertexLength*sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
  //define and enable the vertex attribute pointer for vertex coordinates
  glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0, (void*) 0);
  glEnableVertexAttribArray(0);
  
  
  //create an index buffer
  glGenBuffers(1,&ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  
  //upload the index data to OpenGL
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCount*verticesPerFace* sizeof(GLint), faceData, GL_STATIC_DRAW);
  

}

void OGLMeshObject::getShaderUniforms() {
  //Get Matrix Uniforms
  modelmatrix_location = glGetUniformLocation(shader->getId(), "modelmatrix");
  assert(!Utils::glError("Get modelmatrix uniform variable"));
  viewmatrix_location = glGetUniformLocation(shader->getId(), "viewmatrix");
  assert(!Utils::glError("Get viewmatrix uniform variable"));
  projection_location = glGetUniformLocation(shader->getId(), "projection");
  assert(!Utils::glError("Get projection uniform variable"));
}


OGLMeshObject::OGLMeshObject(OGLShader* shader, const std::vector<  Vertex > vertices, const std::vector<  Face > faces): 
shader(shader), vertexData(nullptr), faceData(nullptr), vertexLength(3), verticesPerFace(3)
{
  //If this constructor is called we assume a plain triangle mesh with no additional attributs
  // that does not use any connected properties (thus 3 floats for position and full 3 indices per face)
  prepareInternalBuffers(vertices,faces);
  prepareAndUploadGLBuffers();
  
  getShaderUniforms();
  //Unbind buffers to leave a clean state behind
  unbindBuffers();
}

OGLMeshObject::OGLMeshObject(OGLShader* shader, int vertexLength, int verticesPerFace): shader(shader), vertexData(nullptr), faceData(nullptr), vertexLength(vertexLength), verticesPerFace(verticesPerFace)
{
  //This constructor is called from child classes which will inject additional attributs (preparation and upload has to be initiated by child class)
  getShaderUniforms();
}

void OGLMeshObject::uploadUniforms(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  //bind the uniforms 
  glUniformMatrix4fv(modelmatrix_location, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewmatrix_location, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projection_location,1, GL_FALSE, glm::value_ptr(projection));

}


void OGLMeshObject::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
//   glDisable(GL_CULL_FACE);
    shader->bindShader();
    glBindVertexArray(vao);
    
    uploadUniforms(projection,view,model);
    
    glDrawElements(verticesPerFace < 3 ? GL_LINES : GL_TRIANGLES,faceCount*verticesPerFace,GL_UNSIGNED_INT, (GLvoid*) 0);
    glBindVertexArray(0);
    shader->unbindShader();
}

void OGLMeshObject::update()
{
//do nothing!
}


OGLMeshObject::~OGLMeshObject()
{
  if (vertexData!= nullptr) delete[] vertexData;
  if (faceData != nullptr) delete[] faceData;
}
