#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;


uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;

out vec3 vertexNormalInCamSpace;

void main() {
  mat4 modelView = viewmatrix* modelmatrix;
  //  create a normal matrix by inverting and transposing the modelView matrix //
  mat4 normalMatrix = transpose(inverse(modelView));
  // transform vertex position and the vertex normal using the appropriate matrices //
  // - assign the transformed vertex position (modelView & projection) to 'gl_Position'
  // - assign the transformed vertex normal (normal matrix) to your out-variable as defined above
  vertexNormalInCamSpace = (normalMatrix * vec4(vnormal, 0)).xyz;
  gl_Position = projection * modelView * vec4(vertex, 1.0);
}
