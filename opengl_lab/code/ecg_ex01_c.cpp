#include "gl/OGLIncludes.h"

#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "utility/OGLUtils.h"
#include "gl/oglshader/SimpleFileShader.h"
#include <iostream>

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 1 c");
    //2. Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"verySimple.vert",SHADERPATH"verySimple.frag");
    //3. Create a triangle (use a simple example triangle)
    Utils::SimpleColoredTriangle triangle(&shader);
    context.addObject(&triangle);
    
    //4. Handoff to the glut renderloop
    context.renderLoop();
    
    //4. Cleanup 
    return EXIT_SUCCESS;

}
