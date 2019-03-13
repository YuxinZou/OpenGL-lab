#include "utility/OBJWavefrontFile.h"
#include "utility/Variant.h"

#include <iostream>
#include <fstream>
#include <sstream>

Utils::OBJWavefrontMeshDataGenerator::OBJWavefrontMeshDataGenerator(Utils::OBJWavefrontFile& objfile)
{
  for (Utils::OBJWavefrontFile::Face& face: objfile.faces) {
    assert((face.vertexIds.size() > 0 ));
    assert((face.vertexIds.size() == face.normalIds.size()) || (face.normalIds.size()==0));
    assert((face.vertexIds.size() == face.texCoordIds.size()) || (face.texCoordIds.size()==0));

    bool hasTexCoords = face.texCoordIds.size()>0;
    bool hasNormals = face.normalIds.size()>0;
    bool hasTangentsAndBinormals = (objfile.binormals.size()>0) & (objfile.tangents.size()>0);
    
    assert(hasTexCoords || (texCoords.size()==0));
    assert(hasNormals || (normals.size() == 0 ));

    // For each vertex build a unique string identifying the given set of ids 
    // reuse found ids and fill the public buffers such that the ids in faces are valid for 
    // vertices, texCoords and normals. Make sure to triangulate polygons with more than 3 vertices
    // (you only have to provide a solution for quads)
    
    std::vector<int> vertexIds;
    for (int vertexNo = 0; vertexNo < face.vertexIds.size(); vertexNo++) {
      std::string id = std::to_string(face.vertexIds.at(vertexNo)) + "_";
      if (hasTexCoords) id += std::to_string(face.texCoordIds.at(vertexNo));
      id+= "_";
      if (hasNormals) id+= std::to_string(face.normalIds.at(vertexNo));
      
      //search the map if a vertex for this id was already created      
      std::map<std::string,int>::iterator vidIter = uniqueVertexMap.find(id);
      if (vidIter != uniqueVertexMap.end()) {
	vertexIds.push_back(vidIter->second);
      }
      else {
	vertices.push_back(objfile.vertices.at(face.vertexIds.at(vertexNo)));
	if (hasTexCoords) texCoords.push_back(objfile.texCoords.at(face.texCoordIds.at(vertexNo)));
	if (hasNormals) normals.push_back(objfile.normals.at(face.normalIds.at(vertexNo)));
	if (hasTangentsAndBinormals) {
	  binormals.push_back(objfile.binormals.at(face.vertexIds.at(vertexNo)));
	  tangents.push_back(objfile.tangents.at(face.vertexIds.at(vertexNo)));
	}
	vertexIds.push_back(vertices.size()-1);
      }
    }
    
    
    //Copy the faces to output vectors
    std::vector<int> face_out;
    for (int i = 0; i < std::min(vertexIds.size(),static_cast<size_t>(3)); i++) {
      face_out.push_back(vertexIds.at(i));
    }
    faces.push_back(face_out);
    
    //Triangulation of quads 
    if (vertexIds.size() == 4) {
      face_out.clear();
      face_out.push_back(vertexIds.at(0));
      face_out.push_back(vertexIds.at(2));
      face_out.push_back(vertexIds.at(3));
      faces.push_back(face_out);
    }
  }
}

void Utils::OBJWavefrontFile::computeTangentSpace()
{
  tangents.resize(vertices.size(),Tangent(0.f,0.f,0.f));
  binormals.resize(vertices.size(),Binormal(0.f,0.f,0.f));
  std::vector<Normal> smoothNormals (vertices.size(),Normal(0.f,0.f,0.f));
  
  for (Face face: faces) {
    //Resolve the first 3 indices for Vertex Position and Texture Coordinate and store the result for later use
    Vertex faceVertices[3];
    TexCoord faceTexCoords[3];
    
    for (int i = 0; i < 3; i++) {
      faceVertices[i] = vertices.at(face.vertexIds.at(i));
      faceTexCoords[i] = texCoords.at(face.texCoordIds.at(i));
    }
    
    
    // compute triangle edges E1, E2 //
    glm::vec3 E1 = faceVertices[1] - faceVertices[0];
    glm::vec3 E2 = faceVertices[2] - faceVertices[0];
    
    // compute dU and dV //
    GLfloat du1 = faceTexCoords[1].x - faceTexCoords[0].x;
    GLfloat dv1 = faceTexCoords[1].y - faceTexCoords[0].y;
    GLfloat du2 = faceTexCoords[2].x - faceTexCoords[0].x;
    GLfloat dv2 = faceTexCoords[2].y - faceTexCoords[0].y;
                     
    // compute determinant //
    GLfloat det = du1 * dv2 - dv1 * du2;
    
    // compute the tangent for this triangle //
    // make sure that matrix was invertible
    glm::vec3 tangent(1, 0, 0);
    if (det != 0) {
      GLfloat invDet = 1.0f / det;
      // compute only tangent (binormal gets recomputed later anyways) //
      
      assert(!std::isnan(invDet));
      tangent = (E1 * dv2 - E2 * dv1) * invDet;
    }
    //distribute the computed tangent to the faces vertices (as the tangent will be normalized later just adding it is sufficient)
    //also compute the smoothed vertex normal (we assume that the whole surface is smooth and require the smooth normal later
    for (int i = 0; i < face.vertexIds.size(); i++) {
      tangents.at(face.vertexIds.at(i)) += tangent;
      smoothNormals.at(face.vertexIds.at(i)) += normals.at(face.normalIds.at(i));
    }
  }
  
  //TODO: Use Gram-Schmidt to reorthogonalize tangent to normal (and compute the binormal)
  for (int i =0; i < vertices.size(); i++) {
    Tangent tangent = glm::normalize(tangents.at(i));
    std::cout<<"tangent size"<<tangents.size()<<std::endl;


    //assert(glm::length(tangent) > 0);
    Normal normal = glm::normalize(smoothNormals.at(i)); 

    //apply Gram-Schmidt to tangent
    tangent = glm::normalize(tangent);
    tangent -= glm::dot(normal,tangent)*normal;
    assert(glm::length(tangent) > 0);
    tangent = glm::normalize(tangent);

    //compute the binormal orthogonal to tangent and normal (cross product)
    Binormal binormal = glm::normalize(glm::cross(tangent,normal));
    
    
    //Write into our storage vectors
    tangents.at(i) = tangent;
    binormals.at(i) = binormal;
    
  }
}



void Utils::OBJWavefrontFile::parseFace(std::stringstream& sstream,int linenumber)
{
  //Parse a face entry  f [vertexAttribReference]... 
  std::string vertexAttribReference;
  std::string vRef, vtRef, vnRef;
  
  Utils::OBJWavefrontFile::Face face(currentMaterial,currentGroupname);
  
  sstream >> vertexAttribReference; 
  
  //Go through all vertices (note that a face can describe arbitrary polygons)
  while (vertexAttribReference.length()>0) {
    std::stringstream attribs(vertexAttribReference);
    
    // Write code which allows to collect all references to vertex coordinates, texture coordinates and normals
    // Note that texture coordinates and normals are optional. Furthermore ensure that no malformed descriptions are accepted.
    // Texture coordinates and normals must be present for either all or no vertices. 
    // Note2: Objfiles count from 1  
    //Vertex Reference
    if (std::getline(attribs,vRef,'/')) {
      face.vertexIds.push_back(std::atoi(vRef.c_str())-1);
    }
    else {
      //Malformed line 
      std::cerr <<  "Malformed VertexReference in facedefinition: " << vertexAttribReference << " in line " << linenumber << std::endl;
      return; 
    } 
    
    //TexCoord Reference
    if (std::getline(attribs,vtRef,'/')) {
      if (vtRef.length() > 0) {
        face.texCoordIds.push_back(std::atoi(vtRef.c_str())-1);
      }
    }
    
    if (!((face.texCoordIds.size() == 0 )  || (face.texCoordIds.size() == face.vertexIds.size()))) {
	  //Malformed line 
          std::cerr <<  "Malformed facedefinition - either no or all texture coordinates have to be present  Line " << linenumber << std::endl;
          return; 
    }
    
    //Vertex Normal
    if (std::getline(attribs,vnRef,'/')) {
      if (vnRef.length() > 0){
	face.normalIds.push_back(std::atoi(vnRef.c_str())-1);
      }
    }
    
    if (!((face.normalIds.size() == 0 )  || (face.normalIds.size() == face.vertexIds.size()))) {
	  //Malformed line 
          std::cerr <<  "Malformed facedefinition - either no or all normales have to be present  Line " << linenumber << std::endl;
          return; 
    }
    
    //get next vertex reference
    vertexAttribReference = "";
    sstream >> vertexAttribReference; 
    /////////////////////////////////////////////////////////////////////////////////////////////
  }
  faces.push_back(face);

}

void Utils::OBJWavefrontFile::parseGroupName(std::stringstream& sstream,int linenumber)
{
  sstream >> currentGroupname;
}

void Utils::OBJWavefrontFile::parseMaterialLib(std::stringstream& sstream,int linenumber)
{
  sstream >> materialFile;
}

void Utils::OBJWavefrontFile::parseNormal(std::stringstream& sstream,int linenumber){
  float x,y,z;
  //parse a normal entry  vn [x] [y] [z] and add it to the list of normals
  sstream >> x >> y >> z;
  normals.push_back(OBJWavefrontFile::Normal(x,y,z));
}

void Utils::OBJWavefrontFile::parseVertex(std::stringstream& sstream, int linenumber)
{
  float x,y,z;
  //parse a vertex coord entry v [x] [y] [z] and add it to the list of vertices
  sstream  >> x >> y >> z;
  vertices.push_back(OBJWavefrontFile::Vertex(x,y,z));
}

void Utils::OBJWavefrontFile::parseObjectName(std::stringstream& sstream ,int linenumber)
{
  sstream >> objectName;
}


void Utils::OBJWavefrontFile::parseTexCoord(std::stringstream& sstream, int linenumber)
{
  float u,v;
  //parse a texture coordinate entry  vt [u] [v] and add it to the list of texture coordinates
  sstream >> u >> v;
  texCoords.push_back(OBJWavefrontFile::TexCoord(u,v));
}

void Utils::OBJWavefrontFile::parseUseMaterial(std::stringstream& sstream,int linenumber)
{
  sstream >> currentMaterial;
}



Utils::OBJWavefrontFile::OBJWavefrontFile(std::string filename)
{
  std::ifstream objFile;
  objFile.open(filename);
  if (!objFile.is_open()) {
    throw std::runtime_error(filename+" not found");
  }
  
  //parser variables
  std::string line;
  std::string key;
  std::stringstream sstream;

  int linenumber = -1;

  while (std::getline(objFile,line)) {
    if (line.length() == 0) continue;
    linenumber++;
    key = "";
    sstream.clear();
    sstream.str(line);

    sstream >> key;

    //Comment
    if (key[0]== '#') continue;

      //Object Name
    if (key.compare("o") == 0) {
      parseObjectName(sstream,linenumber);
      continue;
    }
      //Vertex
    if (key.compare("v") == 0) {
      parseVertex(sstream,linenumber);
      continue;
    }
      //Vertex normal
    if (key.compare("vn") == 0) {
      parseNormal(sstream,linenumber);
      continue;
    }
      //Texture Coords
    if (key.compare("vt") == 0) {
      parseTexCoord(sstream,linenumber);
      continue;
    }
      //Material library
    if (key.compare("mtllib") == 0) {
      parseMaterialLib(sstream,linenumber);
      continue;
    }
      //face
    if (key.compare("f") == 0) {
      parseFace(sstream,linenumber);
      continue;
    }
      //groupname
    if (key.compare("g") == 0) {
      parseGroupName(sstream,linenumber);
      continue;
    }
      //material usage
    if (key.compare("usemtl") == 0) {
      parseUseMaterial(sstream,linenumber);
      continue;
    }
    //Inform user about ignored tags (ie smoothness, surfaces ...)
    std::cerr << "OBJWavefrontFile Fileparser: Ignoring tag " << key << " from line "<< linenumber << "\n";
  }
  objFile.close();
}
