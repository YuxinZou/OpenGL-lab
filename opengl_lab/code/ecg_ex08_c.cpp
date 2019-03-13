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

#include "gl/OGLFramebufferRenderPass.h"


/*class NormalMapping2ndRenderPass: public OGLFramebufferRenderPass::ScreenFillingQuad {
protected:
  OGLMaterial material;
public:
NormalMapping2ndRenderPass(OGLShader* shader, std::vector< OGLLightSource* > lightSources, std::vector< TextureBinding > textures, OGLMaterial material);
};*/


int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 8 b");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    OGLFramebufferRenderPass renderpass1(800,600,
					 {OGLFramebufferRenderPass::DefaultColorAttachment(GL_COLOR_ATTACHMENT0),
					  OGLFramebufferRenderPass::DefaultColorAttachment(GL_COLOR_ATTACHMENT1),
					  OGLFramebufferRenderPass::DefaultColorAttachment(GL_COLOR_ATTACHMENT2)
					},true);
    context.addObject(&renderpass1);
    
    
    FreeCameraControler camControler(glm::vec3(0.f,1.f,0.f),0.f,0.f);
    renderpass1.addObject(&camControler);
    context.addInputEventHandler(&camControler);
    context.addReshapeEventHandler(&camControler);
    
    //Create a very simple shader 
    //     SimpleFileShader shader("FileShader",SHADERPATH"normalmapping.vert",SHADERPATH"normalmapping.frag");
    SimpleFileShader deferredPass1("FileShader",SHADERPATH"deferred/pass1.vert",SHADERPATH"deferred/pass1.frag");
    glBindFragDataLocation(deferredPass1.getId(),0,"deferred_vertex");
    glBindFragDataLocation(deferredPass1.getId(),1,"deferred_normal");
    glBindFragDataLocation(deferredPass1.getId(),2,"deferred_texCoord");
        
    
//     SimpleFileShader simpleScreenFillingShader("FileShader",SHADERPATH"deferred/simple.vert",SHADERPATH"deferred/simpleShow.frag");
    SimpleFileShader deferredPass2("FileShader",SHADERPATH"deferred/pass2.vert",SHADERPATH"deferred/pass2.frag");
    
    OGLMaterial material;
    //(glm::vec3(128.f/255.f,64.f/255.f,0.f),glm::vec3(128.f/255.f,64.f/255.f,0.f),glm::vec3(128.f/255.f,64.f/255.f,0.f));
    OGLLightSource lightSource1(glm::vec3(0.3f,0.3f,0.3f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.f,1.f,1.f),glm::translate(glm::vec3(5.f,0.f,-2.f)),&camControler);
    
    
    //Load head
    OGLTexture diffuseTexture(TEXTUREPATH"diffuse.jpg");
    OGLTexture normalTexture(TEXTUREPATH"normals.jpg");
    
    Utils::OBJWavefrontFile objfile_head(MODELPATH"head.obj");
    objfile_head.computeTangentSpace();
    Utils::OBJWavefrontMeshDataGenerator meshgen_head(objfile_head);
    OGLTexturedMeshObject2 head(&deferredPass1, meshgen_head.vertices,meshgen_head.normals, 
meshgen_head.texCoords,meshgen_head.tangents,meshgen_head.binormals,meshgen_head.faces, 
material,{&lightSource1},{OGLTexturedMeshObject::TextureBinding(&diffuseTexture,"colorTex"),OGLTexturedMeshObject::TextureBinding(&normalTexture,"normalMap")});
    
    //Add the head to the scene (20*20)
    #define noX 50
    #define noZ 50
    #define distX .5f
    #define distZ .5f
    std::vector<OGLSceneGraphNode*> head_nodes;
    for (float x = - noX/2.0f*distX; x < noX/2.0f*distX; x+= distX) {
      for (float z = - noZ/2.0f*distZ; z < noZ/2.0f*distZ; z+= distZ) {
	OGLSceneGraphNode* node = new OGLSceneGraphNode(
					  glm::translate(glm::vec3(x,0.f,z))* /*glm::scale(glm::vec3(10.f,10.f,10.f))*/glm::translate(glm::vec3(0.f,0.31f,0.f)),
					  &camControler,
					  &head);
	head_nodes.push_back(node);
      }
    }
    
    
    
    
            
    //Create a grid 
    OGLGrid grid(1000.f,1000.f,2000,2000,OGLColoredMeshObject::Color(0.0f,0.0f,0.0f));
    OGLSceneGraphNode grid_node(glm::rotate(((float)M_PI/2),glm::vec3(1.f,0.f,0.f)),&camControler,&grid);
    
    
    //Get a screen filling quad and render it
    OGLFramebufferRenderPass::ScreenFillingQuad fillQuad(
      &deferredPass2,
      {&lightSource1},
      {OGLTexturedMeshObject::TextureBinding(&diffuseTexture,"diffuseTexture"),
	OGLTexturedMeshObject::TextureBinding(renderpass1.getTextureHandleForAttachment(GL_COLOR_ATTACHMENT0),"def_vertexMap"),
        OGLTexturedMeshObject::TextureBinding(renderpass1.getTextureHandleForAttachment(GL_COLOR_ATTACHMENT1),"def_normalMap"),
        OGLTexturedMeshObject::TextureBinding(renderpass1.getTextureHandleForAttachment(GL_COLOR_ATTACHMENT2),"def_texCoordMap")});
    context.addObject(&fillQuad);
        
    //Handoff to the glut renderloop
    context.renderLoop();
    
    //Cleanup 
    for (OGLSceneGraphNode* node: head_nodes) {
      delete node; 
    }
    return EXIT_SUCCESS;
}
