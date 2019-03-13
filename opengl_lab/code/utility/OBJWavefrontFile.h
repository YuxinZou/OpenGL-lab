#ifndef OBJWAVEFRONT_H
#define OBJWAVEFRONT_H

#include <string.h>
#include <vector>
#include <map>
#include "math3d/GLMIncludes.h"
#include <gl/oglobject/OGLMeshObject.h>
namespace Utils {

class OBJWavefrontFile {
public:
  

  typedef glm::vec3 Normal;
  typedef glm::vec3 Vertex;
  typedef glm::vec2 TexCoord;
  typedef glm::vec3 Tangent;
  typedef glm::vec3 Binormal;
  

  struct Face: public OGLMeshObject::Face {
    Face(std::string material, std::string groupName): material(material), groupName(groupName) {}
    std::string material;
    std::string groupName;
    
    std::vector<int> vertexIds;
    std::vector<int> texCoordIds;
    std::vector<int> normalIds;
  };
  
protected:
  std::string currentMaterial;
  std::string currentGroupname;
  
  void parseNormal(std::stringstream& sstream, int linenumber);
  void parseVertex(std::stringstream& sstream, int linenumber);
  void parseTexCoord(std::stringstream& sstream, int linenumber);
  void parseFace(std::stringstream& sstream, int linenumber);
  void parseGroupName(std::stringstream& sstream, int linenumber);
  void parseObjectName(std::stringstream& sstream, int linenumber);
  void parseMaterialLib(std::stringstream& sstream, int linenumber);
  void parseUseMaterial(std::stringstream& sstream, int linenumber);
  
  
  
public:
  OBJWavefrontFile(std::string filename);
  void computeTangentSpace();
  
  std::string objectName;
  std::string materialFile;
  
  std::vector<Face> faces;
  std::vector<Vertex> vertices;
  std::vector<TexCoord> texCoords;
  std::vector<Normal> normals;
  std::vector <Tangent> tangents; 
  std::vector <Binormal> binormals;


};

class OBJWavefrontMeshDataGenerator {
protected:
  std::map<std::string,int> uniqueVertexMap;
  
public:
  typedef glm::vec3 Normal;
  typedef glm::vec3 Vertex;
  typedef glm::vec2 TexCoord;
  typedef glm::vec3 Tangent;
  typedef glm::vec3 Binormal;  
  typedef std::vector<int> Face; 
  
  OBJWavefrontMeshDataGenerator(OBJWavefrontFile& objfile);
  
  std::vector<Face> faces;
  std::vector<Vertex> vertices;
  std::vector<TexCoord> texCoords;
  std::vector<Normal> normals;
  std::vector <Tangent> tangents; 
  std::vector <Binormal> binormals;
};



}
#endif
