#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec2 vtexcoord;


uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;

uniform mat4 lightProjection;
uniform mat4 lightTransform;



out vec2 textureCoord;

out vec4 vertexInLightViewSpace; 
out vec4 vertexInLightViewSpaceProjected;
out vec3 surfaceNormalInLightViewSpace;

void main() {
  // default vertex transformation
  gl_Position = projection * viewmatrix* modelmatrix * vec4(vertex, 1.0);
  textureCoord = vtexcoord.xy;
  
  
  // TODO: compute:
  //  - vertexInLightViewSpace
  //  - vertexInLightViewSpaceProjected
  //  - surfaceNormalInLightViewSpace
  vec3 vertexInModelview = vec3(modelmatrix * vec4(vertex,1.0f));
  vertexInLightViewSpace = lightTransform * vec4(vertexInModelview,1.0);
  vertexInLightViewSpaceProjected =lightProjection *lightTransform * vec4(vertexInModelview,1.0f);
  surfaceNormalInLightViewSpace = (transpose(inverse(lightTransform*modelmatrix)) * vec4(vnormal,0.0f)).xyz;
  
  
  
  
  
  
  
}


