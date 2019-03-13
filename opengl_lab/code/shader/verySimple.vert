#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vcolor;
out vec3 vertexColor;
// out vec4 color;

void main() {
//   mat4 proj =  mat4(2.414214, 0.000000, 0.000000, 0.000000, 0.000000, 2.414214, 0.000000, 0.000000, 0.000000, 0.000000, -1.002002, -1.000000, 0.000000, 0.000000, -0.020020, 0.000000);
//   mat4 mv = mat4(1.0, 0.0, 0.0, 0.000000, 0.000000, 1.0, 0.0, 0.000000, 0.0, 0.0, 1.0, 0.000000, 0.000000, 0.000000, 0.0, 1.000000);

  vertexColor = (vcolor+vec3(1,1,1))/2.0;
  gl_Position = vec4(vertex*0.5, 1.0);
//   color = vec4(vertex,1.0);
}
