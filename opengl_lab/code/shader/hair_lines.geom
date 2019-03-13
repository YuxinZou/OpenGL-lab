#version 330

// TODO : create #define for number of output vertices 
#define OUT_VERTS 20

// TODO : create layout
//	- triangles as input
//	- line strips as output, number set to previously defined number
layout(triangles) in;
layout(line_strip, max_vertices = OUT_VERTS) out;


// view, modelview and projection matrix //
uniform mat4 modelview;
uniform mat4 projection;

struct Material {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_shininess;
};

uniform Material material;


#define hairLength 20.0 * 0.01
uniform vec3 gravity;

//  create input and output variables
// - input : normal for each vertex of a triangle
// - output : one single color
in vec3 normal[3];

out vec4 color;

void main(void) {
  //  compute center of triangle
  vec4 c = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
  //  compute normalized face normal
  vec3 n = (normal[0] + normal[1] + normal[2]) / 3.0;
  n = normalize(n);
  
  //  compute and emit hair vertices for each hair segment, which is processed in fragment shader
  // project triangle center and set for for output
  // - compute output color from material color
  //		-	for this task you can simply fake the shading by setting
  //			a simple gradient from first to last vertex of a hair
  // - compute vertex position for each hair segment
  //		-	begin at triangle center
  //		-	reduce y-component of normal in each step to simulate simple gravity effect
  //		-	normalize normal
  //		-	step into normal direction for each new segment vertex
  //		- emit vertex using EmitVertex()
  // - close primitive by calling EndPrimitive()
  
  gl_Position = c;
  
  color = vec4(material.diffuse_color, 1);
  
  EmitVertex();
  	
  vec4 b = c;
  
  for (int i = 0; i < OUT_VERTS-1; i++) {
    n += gravity; // default : - 0. 1
    n = normalize(n);
    b += 0.001 * hairLength * vec4(n, 0);

    gl_Position = b;

    color = vec4(material.diffuse_color, 1) * (OUT_VERTS-i) * 1.0/OUT_VERTS;
    EmitVertex();
  }
  EndPrimitive();
  
  
}
