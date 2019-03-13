#include "gl/oglobject/OGLWireBox.h"
#include "gl/oglshader/SimpleFileShader.h"
#include <iostream>

OGLWireBox::OGLWireBox():
OGLColoredMeshObject(
  new SimpleFileShader("ColorShaderWireBox",SHADERPATH"modelView2.vert",SHADERPATH"verySimple.frag"),
		     {glm::vec3(-1.f,-1.f,-1.f), //Vertex coordinates
		      glm::vec3( 1.f,-1.f,-1.f),
		      glm::vec3( 1.f, 1.f,-1.f),
		      glm::vec3(-1.f, 1.f,-1.f),
		      glm::vec3(-1.f,-1.f, 1.f),
		      glm::vec3( 1.f,-1.f, 1.f),
		      glm::vec3( 1.f, 1.f, 1.f),
		      glm::vec3(-1.f, 1.f, 1.f)},

		     {glm::vec3(1.f,1.f,1.f), //Vertex colors
		      glm::vec3(1.f,1.f,1.f),
		      glm::vec3(1.f,1.f,1.f),
		      glm::vec3(1.f,1.f,1.f),
		      glm::vec3(1.f,1.f,1.f),
		      glm::vec3(1.f,1.f,1.f),
		      glm::vec3(1.f,1.f,1.f),
		      glm::vec3(1.f,1.f,1.f)},
		     
		      { std::vector<int> ({0, 1}), //Faces
			std::vector<int> ({1, 2}),
			std::vector<int> ({2, 3}),
			std::vector<int> ({3, 0}),
			std::vector<int> ({4, 5}),
			std::vector<int> ({5, 6}),
			std::vector<int> ({6, 7}),
			std::vector<int> ({7, 4}),
			std::vector<int> ({0, 4}),
			std::vector<int> ({1, 5}),
			std::vector<int> ({2, 6}),
			std::vector<int> ({3, 7}) },
			2  //2 vertices per face (we draw lines)
		    )
{}

OGLWireBox::~OGLWireBox() {
  delete shader; //we create this via new so we have to clean up
}

void OGLWireBox::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model){
    shader->bindShader();
    glBindVertexArray(vao);
    this->uploadUniforms(projection, view, model);
    glDrawElements(GL_LINES,faceCount*verticesPerFace,GL_UNSIGNED_INT, (GLvoid*) 0);
    glBindVertexArray(0);
    shader->unbindShader();
}