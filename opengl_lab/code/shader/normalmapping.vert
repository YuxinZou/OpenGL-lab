#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec2 vtexcoord;
layout(location = 3) in vec3 vtangent;
layout(location = 4) in vec3 vbinormal;

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


out vec3 vertexNormalInCamSpace;
out vec3 eyeDirectionInCamSpace;
out vec3 lightDirectionInCamSpace[maxLightCount];

//add the texture coordinate as in/out variable to be passed to the fragment program //
out vec2 textureCoord;
out vec3 dbgColor;


void main() {
  mat4 modelView = viewmatrix * modelmatrix;
  int lightCount = max(min(lightsInUse, maxLightCount), 0);
  
  //create a normal matrix by inverting and transposing the modelView matrix //
  mat4 normalMatrix = transpose(inverse(modelView));
  //transform vertex position and the vertex normal using the appropriate matrices //
  // - assign the transformed vertex position (modelView & projection) to 'gl_Position'
  // - assign the transformed vertex normal (normal matrix) to your out-variable as defined above
  vertexNormalInCamSpace = (normalMatrix * vec4(vnormal, 0)).xyz;
  gl_Position = projection * modelView * vec4(vertex, 1.0);
  //compute the vectors from the current vertex towards the camera and towards the light source //
  // compute per vertex camera direction//
  vec3 vertexInCamSpace = (modelView * vec4(vertex, 1.0)).xyz;
  

  //TODO transform the binormal and tangent to camera space
  vec3 vbinormalInCamSpace = (normalMatrix * vec4(vbinormal, 0)).xyz;
  vec3 vtangentInCamSpace = (normalMatrix * vec4(vtangent, 0)).xyz;
  
  //TODO derive a transform from camera space to tangent space 
  mat3 CamSpace2TangentSpace = mat3(vtangentInCamSpace.x, vbinormalInCamSpace.x, vertexNormalInCamSpace.x,
                                 vtangentInCamSpace.y, vbinormalInCamSpace.y, vertexNormalInCamSpace.y,
                                 vtangentInCamSpace.z, vbinormalInCamSpace.z, vertexNormalInCamSpace.z);
  
  
  //TODO compute vector from vertex to camera (in TangentSpace!) //
  eyeDirectionInCamSpace = CamSpace2TangentSpace * -vertexInCamSpace;
  //TODO compute the vertex to light vector for every light source! (in TangentSpace!) //
  for (int i = 0; i < lightCount; ++i) {
    vec3 lightInCamSpace = lightSources[i].position;
    lightDirectionInCamSpace[i] = CamSpace2TangentSpace * (lightInCamSpace - vertexInCamSpace);
  }
  
  // write texcoord //
  textureCoord = vtexcoord.xy;
  
//Debugging hints
//   dbgColor = vbinormalInCamSpace*0.5+0.5;
//   dbgColor = vtangent*0.5+0.5;
//   dbgColor = vec3(vtexcoord.x,0.0,vtexcoord.y);
}


