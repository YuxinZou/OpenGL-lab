#include "gl/oglobject/OGLSceneGraphNode.h"
#include "utility/Variant.h"
#include <iostream>

OGLSceneGraphNode::OGLSceneGraphNode(glm::mat4 relativeTransform, OGLSceneGraphNode* parent):
relativeTransform(relativeTransform), parent(parent)
{
  if (parent != nullptr) {
    parent->addChild(this);
  }
}

OGLSceneGraphNode::OGLSceneGraphNode(glm::mat4 relativeTransform, OGLSceneGraphNode* parent, OGLObject* child):
relativeTransform(relativeTransform), parent(parent)
{
  if (parent != nullptr) {
    parent->addChild(this);
  }
  addChild(child);
}

OGLSceneGraphNode::OGLSceneGraphNode(glm::mat4 relativeTransform, OGLSceneGraphNode* parent, OGLSceneGraphNode* child):
relativeTransform(relativeTransform), parent(parent)
{
  assert(child!= nullptr);
  if (parent != nullptr) {
    parent->addChild(this);
  }
  addChild(child);
  child->parent = this;
}


void OGLSceneGraphNode::addChild(OGLObject* child)
{
  if (child == nullptr) return;
  //add the child to this node and update its context setting
  childNodes.push_back(child);
  child->setContext(context);
}



void OGLSceneGraphNode::removeChild(OGLObject* child)
{
  if (child == nullptr) return;
  //set the childs context to nullptr (we cannot be sure it will be in the same context) and remove from the set
  child->setContext(nullptr);
  for (auto iter = childNodes.begin(); iter<childNodes.end();iter++) {
    if ((*iter) == child) {
      childNodes.erase(iter);
      break;
    }
  }
}



void OGLSceneGraphNode::setParent(OGLSceneGraphNode* parent)
{
  if (this->parent!= nullptr) {
    this->parent->removeChild(this);
  }
  if (parent != nullptr) {
    parent->addChild(this);
  }
  
  this->parent = parent;
}

void OGLSceneGraphNode::setRelativeTransform(glm::mat4 newrelativeTransform)
{
  relativeTransform = newrelativeTransform;
}



void OGLSceneGraphNode::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  if (!visible) return;
  
  //compute the localTransform and call the render method of all child nodes with the localTransform (leave projection and view transformation untouched)
  glm::mat4 localTransform = model*relativeTransform;
  for (OGLObject* object: childNodes) {
    object->render(projection, view , localTransform);
  }
}


void OGLSceneGraphNode::update()
{
  for (OGLObject* object: childNodes) {
    object->update();
  }
}


glm::vec3 OGLSceneGraphNode::getPosition()
{
    return glm::vec3(this->getTransform()[3]);
}

glm::mat4 OGLSceneGraphNode::getTransform()
{
  if (parent == nullptr) {
    return relativeTransform;
  }
  else {
    glm::mat4 parentT = parent->getTransform();
    return parentT*relativeTransform;
  }
}

glm::mat4 OGLSceneGraphNode::getModelTransform()
{
  if (parent == nullptr) {
    return relativeTransform;
  }
  else {
    glm::mat4 parentT = parent->getModelTransform();
    return parentT*relativeTransform;
  }
}


void OGLSceneGraphNode::setContext(OGLContext* context)
{
    OGLObject::setContext(context);
    for (OGLObject* object: childNodes) {
      object->setContext(context);
    }
}
