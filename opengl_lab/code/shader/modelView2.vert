#version 330
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vcolor;
uniform mat4 modelmatrix;
uniform mat4 viewmatrix;
uniform mat4 projection;
uniform mat4 frustumTransform; 
out vec3 vertexColor;

out float gl_ClipDistance[1];
// out vec4 color;

void main() {
  vertexColor = (vcolor+vec3(1,1,1))/2.0;
  gl_Position = projection*viewmatrix*modelmatrix*vec4(vertex, 1.0);
//     gl_Position.w = 1.0;
  // Ax + By + Cz + D = 0
  vec4 clipPlaneXp = vec4( 1.0,  0.0,  0.0, 1.0);
  vec4 clipPlaneXn = vec4(-1.0,  0.0,  0.0, 1.0);  
  vec4 clipPlaneYn = vec4( 0.0, -1.0,  0.0, 1.0);
  vec4 clipPlaneYp = vec4( 0.0,  1.0,  0.0, 1.0);
  vec4 clipPlaneZp = vec4( 0.0,  0.0,  1.0, 1.0);
  vec4 clipPlaneZn = vec4( 0.0,  0.0, -1.0, 1.0);
  
  

  vec4 pos = gl_Position.xyzw;
  float clipDist = 1.f;
  mat4 normalMat = transpose(inverse(frustumTransform));
//   mat4 normalMat = frustumTransform;
  
  float clipZp  = dot(pos, (normalMat * clipPlaneZp))+0.0/**/;
  float clipZm  = dot(pos, (normalMat * clipPlaneZn))+0.0/**/;
  float clipYp  = dot(pos, (normalMat * clipPlaneYp))+0.0/**/;
  float clipYm  = dot(pos, (normalMat * clipPlaneYn))+0.0/**/;  
  float clipXp  = dot(pos, (normalMat * clipPlaneXp))+0.0/**/;
  float clipXm  = dot(pos, (normalMat * clipPlaneXn))+0.0/**/;
  float clipZ   = min((clipZp), (clipZm));
  float clipY   = min((clipYp), (clipYm));
  float clipX   = min((clipXp), (clipXm));
  float clipZY  = min((clipZ), (clipY));
  float clipXYZ = min((clipX), (clipZY));

  gl_ClipDistance[0] = clipXYZ;
}
