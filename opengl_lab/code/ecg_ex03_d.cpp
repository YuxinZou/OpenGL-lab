#include "gl/OGLIncludes.h"

#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "gl/oglobject/OGLSGRotationalAnimator.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "utility/Variant.h"
#include "gl/oglshader/SimpleFileShader.h"

#include "gl/FreeCameraControler.h"
#include <iostream>

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 3 d");
    
    glEnable(GL_DEPTH_TEST);
    
    //2. Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"modelView.vert",SHADERPATH"verySimple.frag");
    //3. Load the models
    //3.1 Load monkey
    Utils::OBJWavefrontFile objfile_monkey(MODELPATH"monkey.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_monkey(objfile_monkey);
    OGLColoredMeshObject monkey (&shader, meshgen_monkey.vertices, meshgen_monkey.normals, meshgen_monkey.faces);
    //3.2  Load armadillo
    Utils::OBJWavefrontFile objfile_armadillo(MODELPATH"armadillo.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_armadillo(objfile_armadillo);
    OGLColoredMeshObject armadillo (&shader, meshgen_armadillo.vertices, meshgen_armadillo.normals, meshgen_armadillo.faces);
    //3.3  Load bunny
    Utils::OBJWavefrontFile objfile_bunny(MODELPATH"bunny.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_bunny(objfile_bunny);
    OGLColoredMeshObject bunny (&shader, meshgen_bunny.vertices, meshgen_bunny.normals, meshgen_bunny.faces);
    
    //4. create a camera controler
    FreeCameraControler camControler(glm::vec3(-2.f,4.f,-2.f),M_PI+M_PI/4,M_PI/4,0.1f,0.01f);

    //4.2 register the camControler as an Input Event Handler
    context.addInputEventHandler(&camControler);
    context.addReshapeEventHandler(&camControler);
    //4.3 add the camControler as a scenegraph root node to the context
    context.addObject(&camControler);


    //5 create a rotation animation (remember to set the camControler as parent)
    OGLSGRotationalAnimator anim(glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,1.f,0.f),0.01f,&camControler);
    
    
    //5. Create a MeshObjects 
    //5.1 put a monkey head in the middle of the scene
    glm::mat4 transform  = glm::translate(glm::vec3(0.f,1.f,0.f));
    OGLSceneGraphNode monkey_node(transform,&camControler,&monkey);
    //5.2 create 2 bunnys and 2 armadillos nodes facing the monkey rotating around it  (suggested radius 2)
    transform  = glm::rotate((float)(M_PI/2),glm::vec3(0.f,1.f,0.f))*glm::translate(glm::vec3(0.f,0.f,-2.f));
    OGLSceneGraphNode bunny1(transform,&anim,&bunny);
    
    
    transform  = glm::rotate((float)(-M_PI/2),glm::vec3(0.f,1.f,0.f))*glm::translate(glm::vec3(0.f,0.f,-2.f));
    OGLSceneGraphNode bunny2(transform,&anim,&bunny);


    transform  = glm::rotate((float)(M_PI),glm::vec3(0.f,1.f,0.f))*glm::translate(glm::vec3(0.f,0.f,2.f));
    OGLSceneGraphNode armadillo1(transform,&anim,&armadillo);
    
    
    transform  = glm::translate(glm::vec3(0.f,0.f,2.f));
    OGLSceneGraphNode armadillo2(transform,&anim,&armadillo);
    
    //6. Handoff to the glut renderloop
    context.renderLoop();
    
    //7. Cleanup 
    return EXIT_SUCCESS;

}
