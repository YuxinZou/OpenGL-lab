#version 330
const int maxLightCount = 10;

struct LightSource {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  vec3 position;
  // float power;
};

struct Material {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_shininess;
};

uniform LightSource lightSources[maxLightCount];
uniform int lightsInUse;
uniform Material material;

// variables passed from vertex to fragment program //
in vec3 lightPos[maxLightCount];
in vec2 texCoord;

// texture //
uniform sampler2D diffuseTexture;

// pass 1 input //
uniform sampler2D def_vertexMap;
uniform sampler2D def_normalMap;
uniform sampler2D def_texCoordMap;

// this defines the fragment output //
out vec4 color;

void main() {
  // check, if pixel is object //
  if (texture2D(def_vertexMap, texCoord).w == 0) discard;
  
  // get position in camera space //
  vec3 def_vertex = texture2D(def_vertexMap, texCoord).xyz;
  
  // get texture coordinates //
  vec2 def_texCoord = texture2D(def_texCoordMap, texCoord).xy;
  
  // eye vector //
  vec3 E = normalize(-def_vertex);
  // normal in camera space normal //
  vec3 N = normalize(texture2D(def_normalMap, texCoord).xyz);
  
  
  // light computation //
  int lightCount = max(min(lightsInUse, maxLightCount), 0);
  // compute the ambient, diffuse and specular color terms //
  vec3 ambientTerm = vec3(0);
  vec3 diffuseTerm = vec3(0);
  vec3 specularTerm = vec3(0);
  vec3 L, H;
  for (int i = 0; i < lightCount; ++i) {
    L = normalize(lightPos[i] - def_vertex);
    H = normalize(E + normalize(L));
    ambientTerm += lightSources[i].ambient_color;
    diffuseTerm += lightSources[i].diffuse_color * max(dot(L, N), 0);
    specularTerm += lightSources[i].specular_color * pow(max(dot(H, N), 0), material.specular_shininess);
  }
  ambientTerm *= material.ambient_color;
  diffuseTerm *= material.diffuse_color;
  specularTerm *= material.specular_color;
  
  //  get diffuse texture color //
  vec3 diffuse = texture2D(diffuseTexture, def_texCoord).rgb;
  
  //  assign the final color to the fragment output variable //
  color = vec4(diffuse, 1) * vec4(ambientTerm + diffuseTerm + specularTerm, 1);
  
//   color = vec4(0.5,0.5,0,1);
//   color = vec4(diffuse,1);
//   color = vec4(material.ambient_color,1);
//   color = vec4(0.0,0.2,lightCount-1,1);
// color.b = 0.2;
// color = vec4((N+1)*0.5,1);
//   color = vec4(lightSources[0].diffuse_color,1);
//   color = vec4(texture2D(def_texCoordMap, texCoord).zw,texture2D(def_normalMap, texCoord).w,1);
//   color = vec4(def_texCoord,0.1,1);
}
