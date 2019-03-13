#version 330
in vec3 vertexNormalInCamSpace;
out vec4 color;
vec3 normale_vertexNormalInCamSpace;
void main() {
  //map the normal to color
  color = vec4(vertexNormalInCamSpace*0.5+0.5,1.0);  
}