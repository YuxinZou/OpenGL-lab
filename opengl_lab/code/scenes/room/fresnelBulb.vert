#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;


uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;

out vec3 normalInCamSpace;
out vec3 cameraToVertexInCamSpace;

void main() {
  mat4 modelView = viewmatrix* modelmatrix;
  mat4 normalMatrix = transpose(inverse(modelView));
  
  normalInCamSpace = (normalMatrix * vec4(vnormal,0)).xyz;
  vec4 vertexInCamSpace = modelView* vec4(vertex, 1.0);
  gl_Position = projection * vertexInCamSpace;
 
  cameraToVertexInCamSpace = vertexInCamSpace.xyz;
}



