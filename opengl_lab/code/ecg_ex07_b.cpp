#include "gl/OGLIncludes.h"


#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "gl/oglobject/OGLSGRotationalAnimator.h"
#include "gl/oglobject/OGLWireBox.h"
#include "gl/oglobject/OGLRelativeTransform.h"
#include "gl/oglobject/OGLTexturedMeshObject2.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "gl/oglshader/SimpleFileShader.h"

#include "gl/FreeCameraControler.h"
#include "gl/OGLViewport.h"
#include "gl/ViewportFreeCameraControler.h"
#include <iostream>

#include "gl/OGLTexture.h"

#include "gl/oglobject/OGLGrid.h"

#include "utility/Variant.h"

int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 7 b");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);



    FreeCameraControler camControler(glm::vec3(5.f,12.f,11.f),0.42f,0.2f,0.1f);
    context.addObject(&camControler);
    context.addInputEventHandler(&camControler);
    context.addReshapeEventHandler(&camControler);
    
    //Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"normalmapping.vert",SHADERPATH"normalmapping.frag");
    
    //Load OBJ Mesh data
    Utils::OBJWavefrontFile objfile_planet(MODELPATH"planet.obj");
//     Utils::OBJWavefrontFile objfile_planet(MODELPATH"quad.obj");
    objfile_planet.computeTangentSpace();
    Utils::OBJWavefrontMeshDataGenerator meshgen_planet(objfile_planet);
    
    //Generate Material and a Lightsource
    OGLMaterial material(glm::vec3(1.f,1.f,1.f),glm::vec3(1.f,1.f,1.f),glm::vec3(0.1f,0.1f,0.1f),50.f);

    OGLLightSource sun;
    OGLSGRotationalAnimator sunnode(glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,1.f,0.f),-0.01f,&camControler);
    OGLSceneGraphNode sunNode2(glm::translate(glm::vec3(-50.0f,10.0f,0.0f)),&sunnode,&sun);


    // Load color and normal texture for mars (feel free to also use the moon)
    OGLTexture colorTexture(TEXTUREPATH"mars.png");
    OGLTexture normalMap(TEXTUREPATH"mars_normal.png");

//     OGLTexture colorTexture(TEXTUREPATH"moon.png");
//     OGLTexture normalMap(TEXTUREPATH"moon_normal.png");
    
    
    //Create a planet object 
    OGLTexturedMeshObject2 planet (&shader,meshgen_planet.vertices,meshgen_planet.normals, meshgen_planet.texCoords, meshgen_planet.tangents, meshgen_planet.binormals 
,meshgen_planet.faces, material, {&sun},
			  {OGLTexturedMeshObject::TextureBinding(&colorTexture,"colorTex"),
			   OGLTexturedMeshObject::TextureBinding(&normalMap,"normalTex")});
    OGLSceneGraphNode planet_node(glm::translate(glm::vec3(0.0f,10.0f,0.0f)) * glm::scale(glm::vec3(20.0f,20.0f,20.0f)),&camControler,&planet);

    
    
    
    //Create a grid 
    OGLGrid grid(1000.f,1000.f,2000,2000,OGLColoredMeshObject::Color(0.0f,0.0f,0.0f));
    OGLSceneGraphNode grid_node(glm::rotate(((float)M_PI/2),glm::vec3(1.f,0.f,0.f)),&camControler,&grid);
    
    //Handoff to the glut renderloop
    context.renderLoop();
    
    //Cleanup 
    return EXIT_SUCCESS;

}
