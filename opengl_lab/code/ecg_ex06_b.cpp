#include "gl/OGLIncludes.h"


#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "gl/oglobject/OGLSGRotationalAnimator.h"
#include "gl/oglobject/OGLWireBox.h"
#include "gl/oglobject/OGLRelativeTransform.h"
#include "gl/oglobject/OGLTexturedMeshObject.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "gl/oglshader/SimpleFileShader.h"

#include "gl/FreeCameraControler.h"
#include "gl/OGLViewport.h"
#include "gl/ViewportFreeCameraControler.h"
#include <iostream>

#include "gl/OGLTexture.h"

#include "gl/oglobject/OGLGrid.h"



int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 6 b");
    
    glEnable(GL_DEPTH_TEST);



    FreeCameraControler camControler(glm::vec3(1.f,2.f,4.f),0.42f,0.2f);
    context.addObject(&camControler);
    context.addInputEventHandler(&camControler);
    context.addReshapeEventHandler(&camControler);
    
    //Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"texture.vert",SHADERPATH"texture.frag");
    
    //Load OBJ Mesh data
    Utils::OBJWavefrontFile objfile_trashbin(MODELPATH"trashbin.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_trashbin(objfile_trashbin);
    
    Utils::OBJWavefrontFile objfile_ball(MODELPATH"ball.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_ball(objfile_ball);
    
    //Generate Material and a Lightsource
    OGLMaterial material;
    
    OGLLightSource lightsource;
    camControler.addChild(&lightsource);
    
    
    //Create the thrashbin object
    OGLTexture trashbin_texture(TEXTUREPATH"trashbin.png");
    OGLTexturedMeshObject trashbin(&shader,
                                   meshgen_trashbin.vertices,meshgen_trashbin.normals,meshgen_trashbin.texCoords,meshgen_trashbin.faces,
                                   material,{&lightsource},{OGLTexturedMeshObject::TextureBinding(&trashbin_texture,"texture")});
    OGLSceneGraphNode trashbin_node(glm::scale(glm::vec3(0.2,0.2,0.2)),&camControler,&trashbin);
    
    
    //Create the ball objects
    OGLTexture ball_texture(TEXTUREPATH"ball.jpg");
    OGLTexturedMeshObject ball(&shader,
                                   meshgen_ball.vertices,meshgen_ball.normals,meshgen_ball.texCoords,meshgen_ball.faces,
                                   material,{&lightsource},{OGLTexturedMeshObject::TextureBinding(&ball_texture,"texture")});
    OGLSceneGraphNode ball_node(glm::translate(glm::vec3(-1.0f,0.3f,0.f))*glm::scale(glm::vec3(0.15,0.15,0.15)),&camControler,&ball);
    
    
    //Create a grid 
    OGLGrid grid(1000.f,1000.f,2000,2000,OGLColoredMeshObject::Color(0.5f,0.5f,0.5f));
    OGLSceneGraphNode grid_node(glm::rotate(((float)M_PI/2),glm::vec3(1.f,0.f,0.f)),&camControler,&grid);
    
    
    //Handoff to the glut renderloop
    context.renderLoop();
    
    //Cleanup 
    return EXIT_SUCCESS;

}
