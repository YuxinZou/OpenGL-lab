#include "gl/OGLIncludes.h"

#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "utility/OGLUtils.h"
#include <iostream>

#include "utility/OBJWavefrontFile.h"
#include "utility/TestHelper.h"


/** Realtime Computergraphics 
 *  Exercise 02 - a 
 * */

int main(int argc, char **argv) {
  Utils::OBJWavefrontFile objfile(MODELPATH"monkey.obj");  
  Utils::OBJWavefrontMeshDataGenerator meshgen(objfile);
  
  //TO create reference mesh data
  /*for (auto face: meshgen.faces) {
    std::cout << "{";
    for (int vertexId: face) std::cout << vertexId <<",";
    std::cout << "},\n";
  }
  std::cout << std::endl;
  
  for (auto vertex: meshgen.vertices) std::cout << "glm::vec3("<<vertex.x<<","<<vertex.y<<","<<vertex.z<<"),\n";
  std::cout << std::endl;
  
  for (auto texCoord: meshgen.texCoords) std::cout << "glm::vec2("<<texCoord.x<<","<<texCoord.y<<"),\n";
  std::cout << std::endl;
 
  for (auto normal: meshgen.normals) std::cout << "glm::vec3("<<normal.x<<","<<normal.y<<","<<normal.z<<"),\n";
  std::cout << std::endl; //*/
  
    //0. Check the test enviroment
  test::testApproxEqual<float>("Test Env check (success)",1.f,1.f,0.001f);
  test::testApproxEqual<float>("Test Env check (failure)",1.f,2.f,0.001f);
  test::testFaceVector("Test faces", meshgen.faces,test::monkey_faces);
  test::testVectorGLM<glm::vec3>("Test vertex coordinates",meshgen.vertices,test::monkey_vertices,0.001f);
  test::testVectorGLM<glm::vec2>("Test texture coordinates",meshgen.texCoords,test::monkey_texCoords,0.001f);
  test::testVectorGLM<glm::vec3>("Test normals coordinates",meshgen.normals,test::monkey_normals,0.001f);
  
  return EXIT_SUCCESS;
}
