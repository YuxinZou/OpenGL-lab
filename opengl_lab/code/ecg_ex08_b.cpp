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


class SetGravityNode: public OGLSceneGraphNode{
protected:
  OGLShader* shader;
  GLint gravityLocation;
public:
SetGravityNode(OGLShader* shader, OGLSceneGraphNode* parent = nullptr, OGLObject* child = nullptr): OGLSceneGraphNode(glm::mat4(1.f),parent,child),shader(shader) {
  gravityLocation = glGetUniformLocation(shader->getId(),"gravity");
}
virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override{
  glm::vec3 gravity(0,-0.25,0);
  shader->bindShader();
  glUniform3fv(gravityLocation, 1, glm::value_ptr((getTransform()*glm::vec4(gravity,0.f)).xyz()));
  shader->unbindShader();
  OGLSceneGraphNode::render(projection, view, model);
}
};


int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 8 b");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);



    FreeCameraControler camControler(glm::vec3(0.f,1.f,5.f),0.f,0.f);
    context.addObject(&camControler);
    context.addInputEventHandler(&camControler);
    context.addReshapeEventHandler(&camControler);
    
    //Create a very simple shader 
    SimpleFileShader hairShader("hairShader",SHADERPATH"hair.vert",SHADERPATH"hair.frag",SHADERPATH"hair.geom");
    SimpleFileShader shader("phongShader",SHADERPATH"multiLight.vert",SHADERPATH"multiLight.frag");
    
    OGLMaterial material(glm::vec3(128.f/255.f,64.f/255.f,0.f),glm::vec3(128.f/255.f,64.f/255.f,0.f),glm::vec3(128.f/255.f,64.f/255.f,0.f));
    OGLLightSource lightSource1(glm::vec3(0.3f,0.3f,0.3f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.f,0.f,0.f),glm::translate(glm::vec3(5.f,0.f,-2.f)),&camControler);
    
        //3.3  Load bunny
    Utils::OBJWavefrontFile objfile_bunny(MODELPATH"bunny.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_bunny(objfile_bunny);
    OGLNormalMeshObject bunny_hair (&hairShader, meshgen_bunny.vertices, meshgen_bunny.normals, meshgen_bunny.faces,material);
    OGLNormalMeshObject bunny (&shader, meshgen_bunny.vertices, meshgen_bunny.normals, meshgen_bunny.faces,material,{&lightSource1});
    
    SetGravityNode gravNode(&hairShader,&camControler);
    
    OGLSceneGraphNode bunnyHairNode(glm::translate(glm::vec3(0.f,1.05f,0.f)),&gravNode,&bunny_hair);
//     OGLSceneGraphNode bunnyNode(glm::translate(glm::vec3(0.f,1.05f,0.f)),&camControler,&bunny);
    
    //Create a grid 
    OGLGrid grid(1000.f,1000.f,2000,2000,OGLColoredMeshObject::Color(0.0f,0.0f,0.0f));
    OGLSceneGraphNode grid_node(glm::rotate(((float)M_PI/2),glm::vec3(1.f,0.f,0.f)),&camControler,&grid);
    
    //Handoff to the glut renderloop
    context.renderLoop();
    
    //Cleanup 
    return EXIT_SUCCESS;

}
