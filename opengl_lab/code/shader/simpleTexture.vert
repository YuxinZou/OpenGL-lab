#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vnormal;
// add texture coordinates to your available vertex attributes //
layout(location = 2) in vec2 vtexcoord;

uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;

// add the texture coordinate as in/out variable to be passed to the fragment program //
out vec2 textureCoord;

void main() {
  gl_Position = projection * viewmatrix* modelmatrix * vec4(vertex, 1.0);
  // write texcoord //
  textureCoord = vtexcoord.xy;
}


