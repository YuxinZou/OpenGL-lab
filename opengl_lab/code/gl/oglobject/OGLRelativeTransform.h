#ifndef OGLRELATIVETRANSFORM_H
#define OGLRELATIVETRANSFORM_H

#include "gl/oglobject/OGLSceneGraphNode.h"

class OGLRelativeTransform : public OGLSceneGraphNode {
protected:
  bool inverted;
  OGLSceneGraphNode* reference;
  
  inline glm::mat4 getRelativeTransform();
  
public:
  OGLRelativeTransform( OGLSceneGraphNode* reference, OGLSceneGraphNode* parent = nullptr, bool invertTransform = false);
  glm::mat4 getTransform() override;
  virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
};

#endif