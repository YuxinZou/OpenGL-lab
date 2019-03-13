#version 330
out vec4 color;

in vec3 normalInCamSpace;
in vec3 cameraToVertexInCamSpace;

uniform float bias = 0.0;
uniform float scale = 2.0;
uniform int power = 2; 
uniform int lampOn = 0;

void main() {
  vec3 I = normalize(cameraToVertexInCamSpace);
  vec3 N = normalize(normalInCamSpace);
  
  float R = max(0, min(1, bias+scale*pow(1.0+dot( I,N),power)));

  color = mix(vec4(0.1,0.1,0.1,0.2), vec4(1.0), R);
  
  if (lampOn!= 0) {
    color = vec4(1.0,1.0,1.0,1.0);
  }
}
  
  
