#include "gl/OGLIncludes.h"

#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "gl/oglobject/OGLSceneGraphNode.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "utility/Variant.h"
#include "gl/oglshader/SimpleFileShader.h"
#include <iostream>

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 3 a");
    
    glEnable(GL_DEPTH_TEST);
    
    //2. Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"modelView.vert",SHADERPATH"verySimple.frag");
    //3. Load the monkey
    Utils::OBJWavefrontFile objfile(MODELPATH"monkey.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen(objfile);
    
    glm::mat4 transform;
    //4. TODO: compute a transformation (45° arround y axis) (you are allowed to use the full functionality of glm)
    transform = glm::rotate(45.f/180.f*((float)M_PI),glm::vec3(0.f,1.f,0.f));
    
    //5. create a root node in the scenegraph 
    OGLSceneGraphNode node(transform);
    //5.1 set the context
    node.setContext(&context);
    //5.2 add the node to the context
    context.addObject(&node);
    
    //5. Create a MeshObject from the obj  data (use normals as color)
    OGLColoredMeshObject monkey (&shader, meshgen.vertices, meshgen.normals, meshgen.faces);
    node.addChild(&monkey);
    
    //6. Handoff to the glut renderloop
    context.renderLoop();
    
    //7. Cleanup 
    return EXIT_SUCCESS;

}
