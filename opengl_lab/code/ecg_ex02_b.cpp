#include "gl/OGLIncludes.h"

#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "gl/oglshader/SimpleFileShader.h"
#include <iostream>

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 2 b");
    
    glEnable(GL_DEPTH_TEST);
    
    //2. Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"verySimple.vert",SHADERPATH"verySimple.frag");
    //4. Load the monkey
    Utils::OBJWavefrontFile objfile(MODELPATH"monkey.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen(objfile);
    
    //5. Create a MeshObject from the obj  data (use normals as color)
    OGLColoredMeshObject monkey (&shader, meshgen.vertices, meshgen.normals, meshgen.faces);
    context.addObject(&monkey);
    
    //4. Handoff to the glut renderloop
    context.renderLoop();
    
    //4. Cleanup 
    return EXIT_SUCCESS;
}
