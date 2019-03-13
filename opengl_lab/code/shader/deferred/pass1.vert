#version 330
layout(location = 0) in vec3 vvertex;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec2 vtexcoord;
layout(location = 3) in vec3 vtangent;
layout(location = 4) in vec3 vbinormal;

// out variables to be passed to the fragment shader //
out vec3 vertex;
out vec3 tangent;
out vec3 binormal;
out vec3 normal;
out vec2 texCoord;

// modelview and projection matrix //
uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;

void main() {
  mat4 modelView = viewmatrix* modelmatrix;

  gl_Position = projection * viewmatrix* modelmatrix * vec4(vvertex, 1.0);
  
  // vertex position in camera space //
  vertex = (modelView * vec4(vvertex, 1.0)).xyz;
  
  // normal matrix //
  mat4 normalMatrix = transpose(inverse(modelView));
  //tangent, bitangent and normal //
  tangent = (normalMatrix * vec4(vtangent, 0)).xyz;
  binormal = (normalMatrix * vec4(vbinormal, 0)).xyz;
  normal = (normalMatrix * vec4(vnormal, 0)).xyz;
  
  //texture coord //
  texCoord = vtexcoord;
}