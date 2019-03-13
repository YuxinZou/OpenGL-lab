#ifndef OGLGRID_H
#define OGLGRID_H
#include "gl/oglobject/OGLColoredMeshObject.h"

class OGLGrid: public OGLColoredMeshObject {
protected:
  std::vector<Vertex> generateVertices(float maxX, float maxY, int subdivsX, int subdivsY);
  std::vector<Color> generateColors(float maxX, float maxY, int subdivsX, int subdivsY, Color color);
  std::vector<Face> generateFaces(float maxX, float maxY, int subdivsX, int subdivsY);
  
public:
OGLGrid(float maxX, float maxY, int subdivsX, int subdivsY, Color color);
virtual ~OGLGrid();
virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
};


#endif