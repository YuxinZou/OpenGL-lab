#include "gl/OGLIncludes.h"

#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "utility/OGLUtils.h"
#include <iostream>


/** Realtime Computergraphics 
 *  Exercise 01 - a 
 *  
 *  Task: 
 *  
 * */

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 1 a");
    //2. Create a very simple shader 
    Utils::SimpleColorShader simpleColorShader("SimpleColorShader");
    //3. Create a triangle (use a simple example triangle)
    Utils::SimpleColoredTriangle triangle(&simpleColorShader);
    context.addObject(&triangle);
    
    //4. handoff to the glut renderloop
    context.renderLoop();
    
    //4. Cleanup 
    return EXIT_SUCCESS;

}
