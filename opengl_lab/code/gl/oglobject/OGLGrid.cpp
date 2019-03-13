#include "gl/oglobject/OGLGrid.h"
#include "gl/oglshader/SimpleFileShader.h"
#include <iostream>

OGLGrid::OGLGrid(float maxX, float maxY, int subdivsX, int subdivsY, Color color): 
        OGLColoredMeshObject(new SimpleFileShader("ColorShaderWireBox",SHADERPATH"modelView2.vert",SHADERPATH"verySimple.frag")
	   , generateVertices(maxX,maxY,subdivsX,subdivsY) , generateColors(maxX,maxY,subdivsX,subdivsY,color) , generateFaces(maxX,maxY,subdivsX,subdivsY) , 2)
{

}

std::vector< OGLColoredMeshObject::Color > OGLGrid::generateColors(float maxX, float maxY, int subdivsX, int subdivsY, OGLColoredMeshObject::Color color)
{
  std::vector< OGLColoredMeshObject::Color > result;
  
  for (float x = -maxX; x < maxX; x+= (maxX*2/subdivsX) ) {
    result.push_back(color);
    result.push_back(color);
  }
  for (float y = -maxY; y < maxY; y+= (maxY*2/subdivsY) ) {
    result.push_back(color);
    result.push_back(color);
  }
  
  return result;
}

std::vector< OGLMeshObject::Face > OGLGrid::generateFaces(float maxX, float maxY, int subdivsX, int subdivsY)
{
  std::vector< OGLMeshObject::Face > result;
  
  int idx= 0;
  
  for (float x = -maxX; x < maxX; x+= (maxX*2/subdivsX) ) {
    result.push_back(Face({idx,idx+1}));
    idx+=2;
  }
  for (float y = -maxY; y < maxY; y+= (maxY*2/subdivsY) ) {
    result.push_back(Face({idx,idx+1}));
    idx+=2;
  }
  
  return result;
}

std::vector< OGLMeshObject::Vertex > OGLGrid::generateVertices(float maxX, float maxY, int subdivsX, int subdivsY)
{
  std::vector< OGLMeshObject::Vertex > result;
  
  for (float x = -maxX; x < maxX; x+= (maxX*2/subdivsX) ) {
    result.push_back(Vertex(x,-maxY,0));
    result.push_back(Vertex(x, maxY,0));
  }
  for (float y = -maxY; y < maxY; y+= (maxY*2/subdivsY) ) {
    result.push_back(Vertex(-maxX,y,0));
    result.push_back(Vertex( maxX,y,0));
  }
  
  return result;
}

OGLGrid::~OGLGrid()
{
  delete shader; //cleanup
}



void OGLGrid::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
    shader->bindShader();
    glBindVertexArray(vao);
    this->uploadUniforms(projection, view, model);
    glDrawElements(GL_LINES,faceCount*verticesPerFace,GL_UNSIGNED_INT, (GLvoid*) 0);
    glBindVertexArray(0);
    shader->unbindShader();
}
