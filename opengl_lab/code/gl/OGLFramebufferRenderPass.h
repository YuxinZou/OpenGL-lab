#ifndef OGLFRAMEBUFFERRENDERPASS_H
#define OGLFRAMEBUFFERRENDERPASS_H

#include <map>
#include <vector>


#include "gl/OGLContext.h"
#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLTexture.h"
#include "gl/OGLShader.h"
#include "gl/oglobject/OGLTexturedMeshObject.h"

#include "gl/oglobject/OGLSceneGraphNode.h"

/* The Framebuffer Render Pass 
 == is a Context
   This allows us to have a scenegraph structure underneath. We will not allow the invocation of the renderloop function as the framebuffer can only live inside an 
   active opengl context (which usually comes with an event or render loop)
   
 == is a ScengraphNode
   The rendering of the scene inside this context is triggered through the update call of the scenegraph node. This allows us to setup rendereffects in the mainscene in 
   the corresponding node structure. If one would implement accelerations structures (i.e. volume hierachies) this would allow to skip screen space effects which are 
   currently not displayed
   
 == is a GLFWReshapeEventHandler
   Being able to react on the parent contexts reshape events allows us the reinitialize the framebuffer to make sure that screenspace effects are rendered with the same 
   resolution as the screen. 
*/

class OGLFramebufferRenderPass: public OGLContext, public OGLSceneGraphNode, public GLFWReshapeEventHandler {
public:
  //Helperclass that encapsules the relation between a color attachment and the textures format and type
  class Attachment {
  public:
    Attachment(GLenum attachment,GLint internalFormat,GLenum format,GLenum type): attachment(attachment),internalFormat(internalFormat),format(format),type(type){}
    GLenum attachment;
    GLint internalFormat;
    GLenum format;
    GLenum type; 
  };
  //Helperclass which gives a fast Attachment for ColorAttachments
  class DefaultColorAttachment: public Attachment {
  public:
    DefaultColorAttachment(GLenum attachment) : Attachment(attachment,GL_RGBA32F,GL_RGBA,GL_FLOAT) {}    
  };
  
  
  class ScreenFillingQuad: public OGLTexturedMeshObject{
  public:
    ScreenFillingQuad(OGLShader* shader, std::vector< OGLLightSource* > lightSources, std::vector< TextureBinding > textures, OGLMaterial material = OGLMaterial(), 
    std::vector<glm::vec3> corners = {glm::vec3(-1.f,-1.f,0.f),glm::vec3(1.f,-1.f,0.f),glm::vec3(1.f,1.f,0.f),glm::vec3(-1.f,1.f,0.f)} );
  };
  
protected:
  GLuint fbo;
  GLuint depth_rb;
  bool withDepthBuffer = false;
  bool manualDepthAttachment=false;
  bool hasColorAttachments = false;
  int numBuffers = 0;
  std::vector<OGLTexture*> textures;
  std::map<GLenum, OGLTexture*> attachmentIds;
  std::vector <Attachment> attachments;
  
  int width;
  int height;
  
  
  void setupBuffers();
  void destroyBuffers();
  
  
  GLenum* buffers;
public:
OGLFramebufferRenderPass(int width, int height, std::vector<OGLFramebufferRenderPass::Attachment> attachments, bool addDepthBuffer, OGLSceneGraphNode* parent = nullptr);
virtual OGLTexture* getTextureHandleForAttachment(GLenum attachment);
virtual ~OGLFramebufferRenderPass();


//Overrides for OGLContext
  virtual void renderCall() override;
  virtual void renderLoop() override;
  
//Overrides for OGLSceneGraphNode
  virtual void update() override;
//Overrides for GLUTReshapeEventHandler
  virtual void framebufferSizeCallback(int width, int height) override;  
};

#endif
