#version 330
layout(location = 0) in vec3 vertex;
uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;
out vec3 modelspaceVertexPosition;

void main() {
  modelspaceVertexPosition = vertex;
  //apply only the rotational part of the modelView 
  //(which is the camera transform here) as the sky will be infinitely far away (so translation should not have any effects)
  gl_Position = projection*(viewmatrix* modelmatrix*vec4(vertex, 0.0)+vec4(0,0,0,1)); //do not apply translation part of the modelView
}
