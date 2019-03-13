#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;

const int maxLightCount = 10;

struct LightSource {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  vec3 position;
};

uniform LightSource lightSources[maxLightCount];
uniform int lightsInUse;

uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;

// define Output variables for the normal, direction to camera and direction to the light 
// note that the lightdirections have to be available for each light source
out vec3 vertexNormalInCamSpace;
out vec3 eyeDirectionInCamSpace;
out vec3 lightDirectionInCamSpace[maxLightCount];

void main() {
  mat4 modelView = viewmatrix* modelmatrix;
  int lightCount = max(min(lightsInUse, maxLightCount), 0);
  
  // create a normal matrix by inverting and transposing the modelView matrix //
  mat4 normalMatrix = transpose(inverse(modelView));
  // transform vertex position and the vertex normal using the appropriate matrices //
  // - assign the transformed vertex position (modelView & projection) to 'gl_Position'
  // - assign the transformed vertex normal (normal matrix) to your out-variable as defined above
  vertexNormalInCamSpace = (normalMatrix * vec4(vnormal, 0)).xyz;
  gl_Position = projection * modelView * vec4(vertex, 1.0);
  // compute the vectors from the current vertex towards the camera and towards the light source //
  // compute per vertex camera direction//
  vec3 vertexInCamSpace = (modelView * vec4(vertex, 1.0)).xyz;
  
  //compute vector from vertex to camera //
  eyeDirectionInCamSpace = -vertexInCamSpace;
  //compute the vertex to light vector for every light source! //
  for (int i = 0; i < lightCount; ++i) {
    vec3 lightInCamSpace = lightSources[i].position;
    lightDirectionInCamSpace[i] = lightInCamSpace - vertexInCamSpace;
  }
}
