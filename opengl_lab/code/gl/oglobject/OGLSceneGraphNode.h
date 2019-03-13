#ifndef OGLSCENEGRAPHNODE_H
#define OGLSCENEGRAPHNODE_H
#include <gl/OGLObject.h>
#include "math3d/GLMIncludes.h"
#include <vector>


class OGLSceneGraphNode: public OGLObject {
protected: 
  glm::mat4 relativeTransform;  
  OGLSceneGraphNode* parent;
  std::vector<OGLObject*> childNodes; 
  
public:
  bool visible = true;
  
  OGLSceneGraphNode(glm::mat4 relativeTransform = glm::mat4(1.f), OGLSceneGraphNode* parent = nullptr); 
  //convenience constructor 
  OGLSceneGraphNode(glm::mat4 relativeTransform, OGLSceneGraphNode* parent, OGLObject* child);
  OGLSceneGraphNode(glm::mat4 relativeTransform, OGLSceneGraphNode* parent, OGLSceneGraphNode* child);
  virtual void setParent(OGLSceneGraphNode* parent);
  virtual void setRelativeTransform(glm::mat4 newrelativeTransform);
  
  virtual void addChild(OGLObject* child);
  virtual void removeChild(OGLObject* child);
  
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
  virtual void update() override;
  
  virtual glm::mat4 getTransform(); 
  virtual glm::vec3 getPosition();
  
  virtual glm::mat4 getModelTransform();
  
  virtual void setContext(OGLContext* context) override;
};

#endif