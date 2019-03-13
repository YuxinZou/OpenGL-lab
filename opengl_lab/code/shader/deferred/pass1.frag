#version 330
// variables passed from vertex to fragment program //
in vec3 vertex;
in vec3 tangent;
in vec3 binormal;
in vec3 normal;
in vec2 texCoord;

// define your fragment outputs here //
out vec4 deferred_vertex;
out vec4 deferred_normal;
out vec4 deferred_texCoord;

// normal map //
uniform sampler2D normalMap;

void main() {
  //  write position in cam space //
  deferred_vertex = vec4(vertex, 1);
  
  //  compute modified surface normal //
  vec3 normalTS = normalize(texture2D(normalMap, texCoord).rgb * 2 - 1);
  
  vec3 tangentWS = normalize(tangent);
  vec3 binormalWS = normalize(binormal);
  vec3 normalWS = normalize(normal);
  mat3 TSpace2WSpace = inverse(mat3(tangentWS.x, binormalWS.x, normalWS.x,
                                    tangentWS.y, binormalWS.y, normalWS.y,
                                    tangentWS.z, binormalWS.z, normalWS.z));
  
  //  write modified normal in camera space //
  deferred_normal = vec4(normalize(TSpace2WSpace * normalTS), 0);
//   deferred_normal = vec4(normalize(normal), 0);
  
  //  write texture coordinate //
  deferred_texCoord = vec4(texCoord, 0, 0);
  
  
  //DEBUG
//   vec3 dbg = (binormalWS+1)*0.5;
//   vec3 dbg = (normalWS+1)*0.5;
//   vec3 dbg = (deferred_normal.xyz+1)*0.5;
//   vec3 dbg = (tangentWS+1)*0.5;
//   vec3 dbg = (normalTS+1)*0.5;
  
//   deferred_texCoord.zw = dbg.xy;
//   deferred_normal.w = dbg.z;
}
