#ifndef OGLLIGHTSOURCE_H
#define OGLLIGHTSOURCE_H

#include "gl/oglobject/OGLSceneGraphNode.h"
#include <iostream>
#include "gl/OGLIncludes.h"
#include "utility/OGLUtils.h"

class OGLLightSource: public OGLSceneGraphNode {
protected:
  glm::vec3 ambientColor;
  glm::vec3 diffuseColor;
  glm::vec3 specularColor;
  
public:
  static const int maxLightSources = 10;
  
  struct LightSourceLocation {
    LightSourceLocation(GLint positionLocation, 
			GLint ambientColorLocation, 
			GLint diffuseColorLocation, 
			GLint specularColorLocation):
                           positionLocation(positionLocation),
                           ambientColorLocation(ambientColorLocation),
                           diffuseColorLocation(diffuseColorLocation),
                           specularColorLocation(specularColorLocation){
      UNIFORM_CHECK(positionLocation);
      UNIFORM_CHECK(ambientColorLocation);
      UNIFORM_CHECK(diffuseColorLocation);
      UNIFORM_CHECK(specularColorLocation);
    }
    
    GLint positionLocation;
    GLint ambientColorLocation;
    GLint diffuseColorLocation;
    GLint specularColorLocation;
    
    
  };
  
  OGLLightSource(glm::vec3 ambient_color = glm::vec3(.2f,.2f,.2f),
	       glm::vec3 diffuse_color = glm::vec3(.5f,.5f,.5f),
	       glm::vec3 specular_color = glm::vec3(1.f,1.f,1.f),
	       glm::mat4 relativeTransform = glm::mat4(1.f), OGLSceneGraphNode* parent = nullptr);

   void uploadTo(LightSourceLocation location);
  
   bool enabled;
};

#endif