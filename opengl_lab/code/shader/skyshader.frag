#version 330
in vec3 modelspaceVertexPosition;
out vec4 color;

vec3 hsv2rgb(vec3 hsv) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(hsv.xxx + K.xyz) * 6.0 - K.www);
  return hsv.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hsv.y);
}

void main() {
  //TODO: figure out a nice simple way to color our skydome
  // play arround with for example 
  color = vec4(hsv2rgb(vec3(213.0/360.0,max(modelspaceVertexPosition.y,0),200.0/255.0)), 1.0); 
}