#version 330
layout(location = 0) in vec3 vvertex;
layout(location = 2) in vec2 vtexcoord;

const int maxLightCount = 10;

struct LightSource {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  vec3 position;
};

uniform LightSource lightSources[maxLightCount];
uniform int lightsInUse;

// out variables to be passed to the fragment shader //
out vec3 lightPos[maxLightCount];
out vec2 texCoord;

// modelview and projection matrix //
uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;

void main() {
  gl_Position = projection * viewmatrix* modelmatrix * vec4(vvertex, 1.0);
  // compute vertex to light vector for every light source! //
  int lightCount = max(min(lightsInUse, maxLightCount), 0);
  for (int i = 0; i < lightCount; ++i) {
    lightPos[i] = lightSources[i].position;
  }
  
  // write texcoord //
  texCoord = vtexcoord;
  
}