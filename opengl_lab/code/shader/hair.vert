#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;

uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;


out vec3 vertexNormalInCamSpace;
out vec3 normal;

void main()
{	
  mat4 modelView = viewmatrix* modelmatrix;
  mat4 normalMatrix = transpose(inverse(modelView));
  vertexNormalInCamSpace = (normalMatrix * vec4(vnormal, 0)).xyz;
  gl_Position = projection * modelView * vec4(vertex, 1.0);
  normal = (normalMatrix * vec4(vnormal, 0)).xyz;
}



