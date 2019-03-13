#include <iostream>

#include "math3d/GLMIncludes.h"
#include "math3d/Math3d.h"

#include "utility/TestHelper.h"
/** Realtime Computergraphics 
 *  Exercise 01 - b
 *  
 *  Task: 
 *  
 * */

int main(int argc, char **argv) {
  //0. Check the test enviroment
  test::testApproxEqual<float>("Test Env check (success)",1.f,1.f,0.001f);
  test::testApproxEqual<float>("Test Env check (failure)",1.f,2.f,0.001f);
  //1. Angle between vectors
  test::testApproxEqual<float>("Angle between vectors [0]",Math3d::angle(glm::vec3(1,0,0),glm::vec3(1,0,0)),0.f,0.001f);
  test::testApproxEqual<float>("Angle between vectors [1]",Math3d::angle(glm::vec3(1,0,0),glm::vec3(0,1,0)),M_PI/2,0.001f);
  test::testApproxEqual<float>("Angle between vectors [2]",Math3d::angle(glm::vec3(1,0,0),glm::vec3(0,1,0)),M_PI/2,0.001f);
  test::testApproxEqual<float>("Angle between vectors [3]",Math3d::angle(glm::vec3(1,1,0),glm::vec3(-1,1,0)),M_PI/2,0.001f);
  test::testApproxEqual<float>("Angle between vectors [4]",Math3d::angle(glm::vec3(1,0,0),glm::vec3(1,0,0)),0.f,0.001f);
  test::testApproxEqual<float>("Angle between vectors [5]",Math3d::angle(glm::vec3(1,0,0),glm::vec3(0,0,1)),M_PI/2,0.001f);
  test::testApproxEqual<float>("Angle between vectors [6]",Math3d::angle(glm::vec3(1,0,0),glm::vec3(0,0,1)),M_PI/2,0.001f);
  test::testApproxEqual<float>("Angle between vectors [7]",Math3d::angle(glm::vec3(1,0,1),glm::vec3(-1,0,1)),M_PI/2,0.001f);
  test::testApproxEqual<float>("Angle between vectors [8]",Math3d::angle(glm::vec3(1,1,1),glm::vec3(-1,1,-1)),1.910633f,0.001f);
  test::testApproxEqual<float>("Angle between vectors [9]",Math3d::angle(glm::vec3(1,0,0),glm::vec3(0.21,0.42,1)),1.379547,0.001f);
  
  //2. Triangle normals
  glm::vec3 normal = Math3d::triangleNormal(glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,2.f,0.f),glm::vec3(1.f,0.f,0.f));
  test::testEqualGLM<glm::vec3>("Triangle Normal", normal, glm::vec3(0.f,0.f,-1.f));
 
  return EXIT_SUCCESS;
}
