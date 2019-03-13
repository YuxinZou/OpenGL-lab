#include "gl/OGLFramebufferRenderPass.h"
#include "gl/OGLTexture.h"
#include "utility/OGLUtils.h"
#include "utility/Variant.h"

OGLFramebufferRenderPass::OGLFramebufferRenderPass(int width, int height, std::vector<OGLFramebufferRenderPass::Attachment> attachments, bool addDepthBuffer, 
OGLSceneGraphNode* parent): OGLSceneGraphNode(glm::mat4(1.f), parent), attachments(attachments), withDepthBuffer(addDepthBuffer), width(width), height(height)
{
  setupBuffers();
}

void OGLFramebufferRenderPass::destroyBuffers()
{
  for (OGLTexture* tex: textures) {
    delete tex;
  }
  textures.clear();
  attachmentIds.clear();
  glDeleteRenderbuffers(1,&depth_rb);
  glDeleteFramebuffers(1,&fbo);
  
  delete[] buffers;
}

void OGLFramebufferRenderPass::setupBuffers()
{
  // create a framebuffer and store its id, then bind it 
  glGenFramebuffers(1,&fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  Utils::glError("Generating Framebuffer");
  //buffer stores the attachments which are rendered
  buffers = new GLenum[attachments.size()+1];
  int i = 0;
  
  
  for (Attachment attachment: attachments) {
    // Make sure no more than one depth buffer is used
    if (attachment.attachment == GL_DEPTH_ATTACHMENT) {
    if (withDepthBuffer) throw std::runtime_error("Cannot attach renderbuffer depth buffer and depth texture");
    if (manualDepthAttachment) throw std::runtime_error("Trying to attach multiple depth buffers");
      
      manualDepthAttachment = true;      
    }
    else {
      hasColorAttachments = true;
      buffers[i++] = attachment.attachment;
    }
    
    OGLTexture* tex; 
    //create a texture suitable for this attachment. Have a look at the Inner class Attachment for details about the data source
    tex = new OGLTexture(width,height,attachment.internalFormat,attachment.format,attachment.type);
    tex->bindTexture();
    tex->ensureFilterMode();
    tex->ensureWrappingMode();
    tex->unbindTexture();
    
    //make sure we can cleanup the texture later
    textures.push_back(tex);
    //make sure we can return texture references by attachment
    attachmentIds.insert(std::pair<GLenum,OGLTexture*>(attachment.attachment,tex));
    
    // attach the texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.attachment, GL_TEXTURE_2D, tex->getGLHandle(),0);
    Utils::glError("Adding attachment");
  }
 
  //Create a renderbuffer for the depth information  - note that renderbuffer information will not be useabel as texture input; if required render it to an extra color 
  //attachment. 
  
  if (withDepthBuffer) {
    // create a Renderbuffer for the Depth Attachment
    glGenRenderbuffers(1, &depth_rb);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);
    
    Utils::glError("Adding depth renderbuffer");
  }
  
  numBuffers = i;
  
    //Manage special case of depth only pass
  if (!hasColorAttachments) {
    buffers[0] = GL_NONE;
    numBuffers = 1; 
    std::cout << "No Colorattachment detected\n";
  }
  
  //Reset Binding to normal screen space
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void OGLFramebufferRenderPass::renderCall()
{
  //Bind the framebuffer and enable depth test and set a depth function
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  //tell ogl to draw the attachments stored in buffers
  glDrawBuffers(numBuffers, buffers);
 
  //clear color and depth
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  for (OGLObject* object: objects) {
    object->update();
  }
  for (OGLObject* object: objects) {
    object->render(glm::mat4(1.f), glm::mat4(1.0), glm::mat4(1.0));
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  Utils::glError("Offscreen Render Pass");
}

void OGLFramebufferRenderPass::renderLoop()
{
  assert(false); //We do not allow this
}

void OGLFramebufferRenderPass::framebufferSizeCallback(int width, int height)
{
  this->height = height;
  this->width = width;
  //lazy reinit
  destroyBuffers();
  setupBuffers();  
}


OGLTexture* OGLFramebufferRenderPass::getTextureHandleForAttachment(GLenum attachment)
{
  std::map<GLenum,OGLTexture*>::iterator iter = attachmentIds.find(attachment);
  if (iter != attachmentIds.end()) {
    return (*iter).second;
  }
  else {
    return nullptr; 
  }
}

OGLFramebufferRenderPass::~OGLFramebufferRenderPass()
{
  destroyBuffers();
}


void OGLFramebufferRenderPass::update()
{
OGLSceneGraphNode::update();
renderCall();
}


OGLFramebufferRenderPass::ScreenFillingQuad::ScreenFillingQuad(OGLShader* shader, std::vector< OGLLightSource* > lightSources, std::vector< 
OGLTexturedMeshObject::TextureBinding > textures, OGLMaterial material, std::vector<glm::vec3> corners): 
OGLTexturedMeshObject(shader, 
		      corners, 
		      {glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,0.f,1.f)}, 
		      {glm::vec2(0.f,0.f),glm::vec2(1.f,0.f),glm::vec2(1.f,1.f),glm::vec2(0.f,1.f)}, 
		      {{0,1,2},{0,2,3}}, 
		      material , lightSources, textures)
{

}



