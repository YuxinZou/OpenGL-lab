#include "gl/OGLLightSource.h"

OGLLightSource::OGLLightSource(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::mat4 relativeTransform, OGLSceneGraphNode* parent): 
OGLSceneGraphNode(relativeTransform, parent),
ambientColor(ambientColor),
diffuseColor(diffuseColor),
specularColor(specularColor),
enabled(true)
{

}

void OGLLightSource::uploadTo(OGLLightSource::LightSourceLocation location)
{
	//valur_ptr:Return the constant address to the data of the input parameter.
	//glUniform ¡ª Specify the value of a uniform variable for the current program object
  glUniform3fv(location.positionLocation,1, glm::value_ptr(getPosition()));
  glUniform3fv(location.ambientColorLocation,1, glm::value_ptr(ambientColor));
  glUniform3fv(location.diffuseColorLocation,1, glm::value_ptr(diffuseColor));
  glUniform3fv(location.specularColorLocation,1, glm::value_ptr(specularColor));
}

