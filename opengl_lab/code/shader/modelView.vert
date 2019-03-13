#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vcolor;
uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;
out vec3 vertexColor;
// out vec4 color;

void main() {
  vertexColor = (vcolor+vec3(1,1,1))/2.0;
  gl_Position = projection*viewmatrix* modelmatrix*vec4(vertex*0.5, 1.0);
//   color = vec4(vertex,1.0);
}
