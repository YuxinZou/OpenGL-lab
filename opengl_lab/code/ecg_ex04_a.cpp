#include "gl/OGLIncludes.h"


#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLNormalMeshObject.h"
#include "gl/oglobject/OGLSGRotationalAnimator.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "gl/oglshader/SimpleFileShader.h"

#include "gl/FreeCameraControler.h"
#include <iostream>

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 4 a");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //2. Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"normal.vert",SHADERPATH"normal.frag");
    
    //3. Load the monkey
    Utils::OBJWavefrontFile objfile(MODELPATH"smoothMonkey.obj");
//     Utils::OBJWavefrontFile objfile(MODELPATH"sphere.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen(objfile);
    

    
    
    //4. create a camera controler
    FreeCameraControler camControler(glm::vec3(0.f,0.f,-2.f),M_PI,0.f,0.1f,0.01f);
    
    //4.2 register the camControler as an Input Event Handler
    context.addInputEventHandler(&camControler);
    context.addReshapeEventHandler(&camControler);
    //4.3 add the camControler as a scenegraph root node to the context
    context.addObject(&camControler);
    
    OGLMaterial material;
    
    //5. Create a MeshObject from the obj  data (use normals as color) (use the camControler as parent node to use its transform)
    OGLNormalMeshObject monkey (&shader, meshgen.vertices, meshgen.normals, meshgen.faces,material,{});
    camControler.addChild(&monkey);
    
    //6. Handoff to the glut renderloop
    context.renderLoop();
    
    //7. Cleanup 
    return EXIT_SUCCESS;

}
