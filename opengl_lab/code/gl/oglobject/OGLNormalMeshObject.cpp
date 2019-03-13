#include "gl/oglobject/OGLNormalMeshObject.h"
#include "utility/OGLUtils.h"

OGLNormalMeshObject::OGLNormalMeshObject(OGLShader* shader, 
					 const std::vector< OGLMeshObject::Vertex > vertices, 
					 const std::vector< OGLNormalMeshObject::Normal > normals, 
					 const std::vector< OGLMeshObject::Face > faces, 
					 OGLMaterial material, 
					 std::vector< OGLLightSource* > lightSources, 
					 int vertexLength): 
					 OGLMeshObject(shader, vertexLength),
					 material(material),
					 lightSources(lightSources)
{
  assert(lightSources.size() < OGLLightSource::maxLightSources);
}


OGLNormalMeshObject::OGLNormalMeshObject(OGLShader* shader, 
					  const std::vector< OGLMeshObject::Vertex > vertices, 
					  const std::vector< OGLNormalMeshObject::Normal > normals, 
					  const std::vector< OGLMeshObject::Face > faces,
					  OGLMaterial material,
		                          std::vector<OGLLightSource*> lightSources
					):
					  OGLMeshObject(shader,OGLMeshObject::attribLength+OGLNormalMeshObject::attribLength),
					  material(material),
					  lightSources(lightSources)
{
  assert(lightSources.size() < OGLLightSource::maxLightSources);
  
  prepareInternalBuffers(vertices,normals,faces);
  prepareAndUploadGLBuffers();
  
  //Unbind buffers to leave a clean state behind
  unbindBuffers();
  
  getShaderUniforms();
}

int OGLNormalMeshObject::prepareInternalBuffers(const std::vector< OGLMeshObject::Vertex > vertices, 
						const std::vector< OGLMeshObject::Normal > normals,
						const std::vector< OGLMeshObject::Face > faces)
{
  memOffset = OGLMeshObject::prepareInternalBuffers(vertices,faces);
  assert(vertices.size() == normals.size());
  
  //memory has been allocated by parentclass - copy data to the prepared location (behind vertex coordinates)
  for (size_t i = 0; i < normals.size(); i++) {
    vertexData[memOffset+i*attribLength+0] = normals.at(i).x;
    vertexData[memOffset+i*attribLength+1] = normals.at(i).y;
    vertexData[memOffset+i*attribLength+2] = normals.at(i).z;
  }
  return memOffset+normals.size()*attribLength;
}


void OGLNormalMeshObject::prepareAndUploadGLBuffers()
{
  //Upload data to OpenGL
  OGLMeshObject::prepareAndUploadGLBuffers();
  
  //define and enable the vertex attribute pointer for vertex color
  glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,0, (void*)(memOffset*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
}


void OGLNormalMeshObject::getShaderUniforms()
{
OGLMeshObject::getShaderUniforms();
  materialLocation = OGLMaterial::OGLMaterialLocation(glGetUniformLocation(shader->getId(), "material.ambient_color"),
						      glGetUniformLocation(shader->getId(), "material.diffuse_color"),
						      glGetUniformLocation(shader->getId(), "material.specular_color"),
						      glGetUniformLocation(shader->getId(), "material.specular_shininess"));
  assert(!Utils::glError("Get material uniform variables"));
  
#define GET_UNIFORM_STRUCT_LOCATION_STRING(NAME,INDEX,ATTRIB) \
(std::string(NAME)+(std::string("[")+std::to_string(INDEX)+std::string("]."))+std::string(ATTRIB)).c_str()  
  
  for (int i= 0; i < OGLLightSource::maxLightSources; i++) {
    lightSourceLocations.push_back(
      OGLLightSource::LightSourceLocation(glGetUniformLocation(shader->getId(), GET_UNIFORM_STRUCT_LOCATION_STRING("lightSources",i,"position")),
	  			          glGetUniformLocation(shader->getId(), GET_UNIFORM_STRUCT_LOCATION_STRING("lightSources",i,"ambient_color")),
					  glGetUniformLocation(shader->getId(), GET_UNIFORM_STRUCT_LOCATION_STRING("lightSources",i,"diffuse_color")),
					  glGetUniformLocation(shader->getId(), GET_UNIFORM_STRUCT_LOCATION_STRING("lightSources",i,"specular_color"))));
  }
  assert(!Utils::glError("Get lights uniform variables"));
  
  lightsInUseLocation =  glGetUniformLocation(shader->getId(), "lightsInUse");
  viewMatrixLocation =  glGetUniformLocation(shader->getId(), "view");
  
}

void OGLNormalMeshObject::uploadUniforms(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
OGLMeshObject::uploadUniforms(projection, view, model);
  //Upload Material
  material.uploadTo(materialLocation);
  //Upload LightSources and count active lights
  int activeLightsCount = 0;
  for (int i = 0; i < lightSources.size(); i++) {
    OGLLightSource* lightSource = lightSources.at(i);
    if (lightSource->enabled) {
      lightSource->uploadTo(lightSourceLocations.at(activeLightsCount));
      activeLightsCount++;
    }
  }
  
  //Upload number of used lights
  glUniform1i(lightsInUseLocation,activeLightsCount);
}

