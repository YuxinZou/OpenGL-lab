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

#include <chrono>

#include "utility/Variant.h"

//TODO: Write an animator the simulates a pendulum. Use std::chrono to make this time based rather than framerate based. 
// maximum angle should be 15 degrees
// for the rotation axis (1,0,0) can be used (feel free to experiment)
class OGLSGPendulumAnimator: public OGLSceneGraphNode {
protected:
    std::chrono::steady_clock::time_point start;
  
public: 
  OGLSGPendulumAnimator(OGLSceneGraphNode* parent, OGLSceneGraphNode* child): OGLSceneGraphNode(glm::mat4(1.0), parent,child), start(std::chrono::steady_clock::now()) {

  }
  
  virtual void update() override {
    static const glm::vec3 rotationAxis(1, 0, 0);
    static const float maxAngle = glm::radians(15.);
    static const std::chrono::seconds cycleTime(2);
    static const float friction = .1;

    auto timeDiff = std::chrono::steady_clock::now() - start;
    float progress = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff % cycleTime).count()) / std::chrono::milliseconds(cycleTime).count();
    float prevCycles = timeDiff / cycleTime + progress;
    float angle = (cos(progress * 2 * M_PI) * maxAngle) / (1 + prevCycles * friction);

    relativeTransform = glm::rotate(glm::mat4(), angle, rotationAxis);
  }
};



class CullFrontWrapper: public OGLSceneGraphNode {
  
public:
    CullFrontWrapper(OGLSceneGraphNode* parent, OGLSceneGraphNode* child ) : 
      OGLSceneGraphNode(glm::mat4(1.0),parent,child) {}
      
    CullFrontWrapper(OGLSceneGraphNode* parent): 
      OGLSceneGraphNode(glm::mat4(1.0),parent) {}
    
    
    virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override{
      //TODO switch the face culling mode to FRONT, render, switch back to BACKface culling
      glCullFace(GL_FRONT);
      OGLSceneGraphNode::render(projection, view, model);
      glCullFace(GL_BACK);
    }
};

class ControlDummy: public OGLSceneGraphNode, public GLFWInputEventHandler {
protected:
  
public:
  bool lampOn = false;
  OGLShader* lampShader = nullptr;
  OGLShader* shadowMappingShader = nullptr;
  
  ControlDummy():OGLSceneGraphNode(glm::mat4(1.0)) {}
  
  
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override {
    if (lampShader)  {
      lampShader->bindShader();
      GLint location = glGetUniformLocation(lampShader->getId(), "lampOn");
      glUniform1i(location, lampOn?1:0);
      lampShader->unbindShader();
    }
    
    if (shadowMappingShader) {
      shadowMappingShader->bindShader();
      GLint location = glGetUniformLocation(shadowMappingShader->getId(), "baseIntensity");
      glUniform1f(location, lampOn?1.f:0.f);
      shadowMappingShader->unbindShader();
    }
    
    
    //ensure desired state
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND); 
    glEnable(GL_CULL_FACE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  
  virtual void keyboardEvent(int key, int scancode, int action, int mods) override {
    if ((key == GLFW_KEY_L)) {
      if (action == GLFW_RELEASE) {
	lampOn = !lampOn;
      }
    }
  }
};


class FixedCameraNode: public OGLSceneGraphNode {
protected:
  glm::mat4 m_projection;
  
public:
  OGLSceneGraphNode* transformationNode = nullptr;
  
  OGLShader* shadowMappingShader = nullptr;
  
  
  FixedCameraNode(float openingAngleDegrees = 60, 
		  float aspectRatio = 1.7, 
		  float nearPlane = 0.1f, 
		  float farPlane = 50.f,
		  glm::mat4 relativeNodeTransform = glm::mat4(1.0)):
    OGLSceneGraphNode(relativeNodeTransform) 
  {
    m_projection = glm::perspective(glm::radians(openingAngleDegrees), aspectRatio, nearPlane, farPlane);
  }
  

  
  
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override {
    if (!visible) return;
    
    assert (transformationNode);
    assert (shadowMappingShader);

    glm::mat4 newtransform;
    
    //TODO move the camera into the model space of transformationNode
    newtransform = glm::inverse(view * transformationNode->getModelTransform());
    
    //Relative transform is applied afterwards
    newtransform = relativeTransform*newtransform;

    //Upload the transformations to the shader
    shadowMappingShader->bindShader();
    glUniformMatrix4fv(glGetUniformLocation(shadowMappingShader->getId(), "lightTransform"),
			1, GL_FALSE, glm::value_ptr(newtransform));
    glUniformMatrix4fv(glGetUniformLocation(shadowMappingShader->getId(), "lightProjection"),
		        1, GL_FALSE, glm::value_ptr(m_projection));
    shadowMappingShader->unbindShader();
    
    
    for (OGLObject* object: childNodes) {
      object->render(m_projection, view, newtransform);
    }

  }
};


int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,800,600,"Echtzeitcomputergraphik - Aufgabe 9 a");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    FreeCameraControler camControler(glm::vec3(0.f,1.f,5.f),0.f,0.f,0.1f);
    context.addInputEventHandler(&camControler);
    context.addReshapeEventHandler(&camControler);

    
    //mirror node for the renderpass of the shadowmap
    FixedCameraNode shadowParent(150.f, 1.f, 0.1f, 50.f, glm::rotate(glm::radians(90.f),glm::vec3(1.f,0.f,0.f)));
    
    // =============== Setup light renderpass ================
    
    OGLFramebufferRenderPass::Attachment depthAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT32F,GL_DEPTH_COMPONENT,GL_FLOAT);
    OGLFramebufferRenderPass lightPass(2048,2048, {OGLFramebufferRenderPass::DefaultColorAttachment(GL_COLOR_ATTACHMENT0),depthAttachment},false);
    OGLViewport lightPassView(0,0,2048,2048);
    lightPass.addObject(&lightPassView);
    lightPass.addObject(&shadowParent);//*/
    
    
    OGLLightSource lightsource;
    
    //Simple Texture Shader with support for shadow maps
    SimpleFileShader simpleTextureShaderSML("simpleShadowMapLighting", SHADERPATH"simpleTextureSML.vert", SHADERPATH"simpleTextureSML.frag");
//     SimpleFileShader simpleTextureShaderSML("dbgNormalShader", "shader/normal.vert", "shader/normal.frag");

    SimpleFileShader fresnelBulbShader("fresnelBulbShader", SCENEPATH"room/fresnelBulb.vert", SCENEPATH"room/fresnelBulb.frag");


    //Create test geometry
    OGLSceneGraphNode boxesNode(glm::mat4(1.0), &camControler);
    
    //TODO Create sub scenegraph to contain the shadow geometry (attach it to shadowParent).
    // You will require a node that activates front face culling.
    CullFrontWrapper cullNode(&shadowParent);
    
    
    std::vector<glm::vec3> positions ({glm::vec3(1.f,0.2f,-3.f),
				       glm::vec3(1.5f,0.2f,-3.1f),
				       glm::vec3(-1.0f,0.2f,-2.7f),
				       glm::vec3(-1.0f,0.2f,-2.3f),
				       glm::vec3(1.25f,0.6f,-3.0f)});
    std::vector<float> rotations ({0.f,0.f,0.f,0.f,0.f});
    int boxCount =5;
    
    // ======== BOX =============
    OGLTexture boxTexture(SCENEPATH"room/box.png");
    Utils::OBJWavefrontFile box_obj (SCENEPATH"room/box.obj");
    Utils::OBJWavefrontMeshDataGenerator box_meshgen(box_obj);
//     OGLTexturedMeshObject box (box_meshgen.vertices,box_meshgen.faces, {OGLTexturedMeshObject::TextureBinding(&boxTexture,"tex")});
    
    OGLMaterial box_material;
    
    OGLTexturedMeshObject box(&simpleTextureShaderSML,
			       box_meshgen.vertices,box_meshgen.normals,box_meshgen.texCoords, box_meshgen.faces,
			       box_material, 
			       {&lightsource},  
			       {OGLTexturedMeshObject::TextureBinding(&boxTexture,"tex"),
				OGLTexturedMeshObject::TextureBinding(lightPass.getTextureHandleForAttachment(GL_DEPTH_ATTACHMENT),"shadowMap")});
    
    for (int i = 0; i < boxCount; i++) {
      //Create a scenegraph to connect the cam controler and the triangle
      //we leak the scenegraph objects here (not great style but will work)
      new OGLSceneGraphNode(glm::translate(positions.at(i))*
			    glm::rotate(glm::radians(rotations.at(i)),glm::vec3(0.f,1.f,0.f))*
			    glm::scale(glm::vec3(0.2f,0.2f,0.2f)),
			    &boxesNode,&box);
      
      //TODO Add your boxes to the sub scenegraph for shadow geometry (DO NOT duplicate the geometry)
        new OGLSceneGraphNode(glm::translate(positions.at(i))*
                  glm::rotate(glm::radians(rotations.at(i)),glm::vec3(0.f,1.f,0.f))*
                  glm::scale(glm::vec3(0.2f,0.2f,0.2f)),
                  &cullNode,&box);
      
    }
    
    // =============== Floor ===================
    OGLTexture floorTexture(SCENEPATH"room/wood.png");
    floorTexture.bindTexture();
    floorTexture.setWrapSMode(GL_REPEAT);
    floorTexture.setWrapTMode(GL_REPEAT);
    floorTexture.ensureWrappingMode();
    floorTexture.setFilterMode(GL_LINEAR, GL_LINEAR);
    floorTexture.ensureFilterMode();
    floorTexture.unbindTexture();

    
    OGLMaterial floor_material;



    OGLTexturedMeshObject floor (&simpleTextureShaderSML,
				 {glm::vec3(-15, 0, 15), glm::vec3(15, 0, 15), glm::vec3(-15, 0, -15), glm::vec3(15, 0, -15)},
			         {glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)},
			         {glm::vec2(-15, 15), glm::vec2(-15, -15), glm::vec2(15, 15), glm::vec2(15, -15)}, 
				 {{0,1,2},{2,1,3}},
			         floor_material, 
			         {&lightsource},  
			         {OGLTexturedMeshObject::TextureBinding(&floorTexture,"tex"),
				  OGLTexturedMeshObject::TextureBinding(lightPass.getTextureHandleForAttachment(GL_DEPTH_ATTACHMENT),"shadowMap")
				});
    

    //Create a scenegraph to connect the cam controler and the triangle
    OGLSceneGraphNode floorNode(glm::mat4(1),&camControler,&floor);
    
    //TODO technically we should add the floor to the shadow pass but there is nothing it can cast a shadow on 
    // so after considering it for a few seconds actively decide not to add it and continue


    
    // =============== Lamp ===============
    //NOTE: no parts of the lamp are added to the shadow map 
    // there a two reasons:
    //  1. the bulb actually emits the light and should thus not cast a shadow 
    //  2. we do render a spot light and all other parts are behind our light source
    
    //lamp meta node
    OGLSceneGraphNode lampNode(glm::translate(glm::vec3(0.f,2.2f,-3.f)), &camControler);
    OGLSceneGraphNode lampScaleNode(glm::scale(glm::vec3(0.02f,0.02f,0.02f)), nullptr);
    OGLSGPendulumAnimator animNode(&lampNode, &lampScaleNode);
    OGLSceneGraphNode virtualLightNode(glm::translate(glm::vec3(0.f,-1.4f,0.f)), &animNode); //used for light position
    
    // =============== Bulb ===============
    Utils::OBJWavefrontFile bulb_obj (SCENEPATH"room/bulb.obj");
    Utils::OBJWavefrontMeshDataGenerator bulb_meshgen(bulb_obj);
    OGLNormalMeshObject bulb (&fresnelBulbShader, bulb_meshgen.vertices, bulb_meshgen.normals, bulb_meshgen.faces,OGLMaterial(),{});
    OGLSceneGraphNode bulbNode(glm::mat4(1.0), &lampScaleNode, &bulb);

    // =============== Socket ===============
    Utils::OBJWavefrontFile socket_obj (SCENEPATH"room/socket.obj");
    Utils::OBJWavefrontMeshDataGenerator socket_meshgen(socket_obj);
    OGLNormalMeshObject socket (&simpleTextureShaderSML, socket_meshgen.vertices, socket_meshgen.normals, socket_meshgen.faces,OGLMaterial(),{});
    OGLSceneGraphNode socketNode(glm::mat4(1.0),&lampScaleNode, &socket);

    // ============== Dummy Controler ========
    // listens to keyboard events and "turns on" our lightbulb (by setting the correct uniforms inside the shaders)
    ControlDummy ctrlDummy;
    ctrlDummy.lampShader = &fresnelBulbShader;
    ctrlDummy.shadowMappingShader = &simpleTextureShaderSML;
    context.addInputEventHandler(&ctrlDummy);
    

    //Create a grid 
    OGLGrid grid(1000.f,1000.f,2000,2000,OGLColoredMeshObject::Color(0.0f,0.0f,0.0f));
    OGLSceneGraphNode grid_node(glm::translate(glm::vec3(0.f,-0.01f,0.f))*glm::rotate(((float)M_PI/2),glm::vec3(1.f,0.f,0.f)),&camControler,&grid);

    
    //Configure our shadow parent
    // The light source is moved into the same space as virtualLightNode
    // the simpleTextureShaderSML shader is configured to render with the shadowmap 
    shadowParent.transformationNode = &virtualLightNode;
    shadowParent.shadowMappingShader = &simpleTextureShaderSML;
    
    
    //======================================
    
    // Render Depth map for debug (show 2 overlays from the shadow space)
    SimpleFileShader simpleTextureShader ("simpleTextureShader",SHADERPATH"noOp.vert",SHADERPATH"simpleTexture.frag");
    SimpleFileShader simpleDepthTextureShader ("simpleTextureShader",SHADERPATH"noOp.vert",SHADERPATH"simpleDepthTexture.frag");

    OGLFramebufferRenderPass::ScreenFillingQuad shadowTex(&simpleTextureShader,{},
    {OGLTexturedMeshObject::TextureBinding(lightPass.getTextureHandleForAttachment(GL_COLOR_ATTACHMENT0),"tex")}, OGLMaterial(), 
					  std::vector<glm::vec3> ({glm::vec3(0.5f,0.5f,0.f),
                                                                   glm::vec3(1.f,0.5f,0.f),
                                                                   glm::vec3(1.f,1.f,0.f),
                                                                   glm::vec3(0.5f,1.f,0.f)}));
    
    OGLFramebufferRenderPass::ScreenFillingQuad shadowDepth(&simpleDepthTextureShader,{},
    {OGLTexturedMeshObject::TextureBinding(lightPass.getTextureHandleForAttachment(GL_DEPTH_ATTACHMENT),"tex")}, OGLMaterial(), 
					  std::vector<glm::vec3> ({glm::vec3(0.5f,0.0f,0.f),
                                                                   glm::vec3(1.f,0.0f,0.f),
                                                                   glm::vec3(1.f,.5f,0.f),
                                                                   glm::vec3(0.5f,.5f,0.f)}));
    
    //*/
    

    //setup the scene to render
    context.addObject(&ctrlDummy); //manages key presses (should be done befor rendering the final scene)
    context.addObject(&lightPass); //this triggers the rendering of the framebuffer internal content
    context.addObject(&camControler);
    context.addObject(&shadowTex); //overlay the current renderpass content (for debugging)
    context.addObject(&shadowDepth); //overlay the current renderpass content (for debugging)
    
    context.renderLoop();

    //Cleanup
    return EXIT_SUCCESS;

}
