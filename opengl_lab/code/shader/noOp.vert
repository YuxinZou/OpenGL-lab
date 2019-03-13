#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec2 vtexcoord;


out vec2 textureCoord;

void main() {
  gl_Position = vec4(vertex, 1.0);
  textureCoord = vtexcoord.xy;
}


