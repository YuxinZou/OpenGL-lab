#include "gl/OGLIncludes.h"

#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "gl/oglobject/OGLSceneGraphNode.h"
#include "gl/oglobject/OGLSGRotationalAnimator.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "gl/oglshader/SimpleFileShader.h"
#include <iostream>

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 3 b");
    
    glEnable(GL_DEPTH_TEST);
    
    //2. Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"modelView.vert",SHADERPATH"verySimple.frag");
    //3. Load the monkey
    Utils::OBJWavefrontFile objfile(MODELPATH"monkey.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen(objfile);
    
    //4. create a rotating node in the scenegraph
    OGLSGRotationalAnimator anim(glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,1.f,0.f),0.01f);
    context.addObject(&anim);
    
    //5. Create a MeshObject from the obj  data (use normals as color)
    OGLColoredMeshObject monkey (&shader, meshgen.vertices, meshgen.normals, meshgen.faces);
    anim.addChild(&monkey);
    
    //6. Handoff to the glut renderloop
    context.renderLoop();
    
    //7. Cleanup 
    return EXIT_SUCCESS;

}
