#version 330
in vec3 vertexColor;
out vec4 color;
void main() {
  color = vec4(vertexColor, 1.0); 
//   color.g = 0.5;
//   color = vec4(0.0,0.0,1.0,1.0);
}