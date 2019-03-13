#ifndef OGLWIREBOX_H
#define OGLWIREBOX_H

#include "gl/oglobject/OGLColoredMeshObject.h"

class OGLWireBox: public OGLColoredMeshObject {
public:
OGLWireBox();
virtual ~OGLWireBox();
virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
};

#endif