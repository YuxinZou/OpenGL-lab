#ifndef OGLMATERIAL_H
#define OGLMATERIAL_H
#include "math3d/GLMIncludes.h"
#include "gl/OGLIncludes.h"

#include "utility/OGLUtils.h"

class OGLMaterial {
public:
  struct OGLMaterialLocation {
    OGLMaterialLocation(GLint ambientColorLocation = 0, 
			GLint diffuseColorLocation = 0, 
			GLint specularColorLocation = 0, 
			GLint shininessLocation = 0):
                          ambientColorLocation(ambientColorLocation),
                          diffuseColorLocation(diffuseColorLocation),
                          specularColorLocation(specularColorLocation),
                          shininessLocation(shininessLocation){
  UNIFORM_CHECK(ambientColorLocation);
  UNIFORM_CHECK(diffuseColorLocation);
  UNIFORM_CHECK(specularColorLocation);
  UNIFORM_CHECK(shininessLocation);
}
    
    GLint ambientColorLocation;
    GLint diffuseColorLocation;
    GLint specularColorLocation;
    GLint shininessLocation;    
  };
  
  glm::vec3 ambientColor;
  glm::vec3 diffuseColor;
  glm::vec3 specularColor;
  float shininess;
  OGLMaterial(glm::vec3 ambientColor = glm::vec3(1.f,1.f,1.f), glm::vec3 diffuseColor = glm::vec3(1.f,1.f,1.f), glm::vec3 specularColor = glm::vec3(1.f,1.f,1.f), float shininess = 5.f): 
  ambientColor(ambientColor),diffuseColor(diffuseColor),specularColor(specularColor),shininess(shininess) {};
  
  void uploadTo(OGLMaterialLocation location) {
    glUniform3fv(location.ambientColorLocation,1, glm::value_ptr(ambientColor));
    glUniform3fv(location.diffuseColorLocation,1, glm::value_ptr(diffuseColor));
    glUniform3fv(location.specularColorLocation,1, glm::value_ptr(specularColor));
    glUniform1f(location.shininessLocation,shininess);
  }
};



#endif