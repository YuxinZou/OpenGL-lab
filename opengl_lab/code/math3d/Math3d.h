#ifndef MATH3D_H
#define MATH3D_H

#include "utility/Variant.h"

#include "math3d/GLMIncludes.h"
namespace Math3d {
inline float angle(glm::vec3 vec1,glm::vec3 vec2) {
  return acos((glm::dot(vec1 ,vec2))/(glm::length(vec1)*glm::length(vec2)));
}

inline glm::vec3 triangleNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
  return glm::normalize(glm::cross(B-A,C-A));
}

}

#endif
