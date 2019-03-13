#ifndef OGLCOLOREDMESHOBJECT_H
#define OGLCOLOREDMESHOBJECT_H
#include "OGLMeshObject.h"
#include <GL/gl.h>
class OGLColoredMeshObject: public OGLMeshObject {
public:
  typedef glm::vec3 Color;
  OGLColoredMeshObject(OGLShader* shader, 
		       const std::vector<Vertex> vertices, 
		       const std::vector<Color> colors, 
		       const std::vector<Face> faces,
		       const int verticesPerFace = 3
		      );
  static const int attribLength = 3;
};


#endif
