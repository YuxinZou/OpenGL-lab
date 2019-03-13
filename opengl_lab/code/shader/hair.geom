// geometry shader for growing hair (triangles)

#version 330
// TODO : create #define for number of output vertices 
#define OUT_VERTS 80

// TODO : create layout
//	- triangles as input
//	- select a suitable output configuration, number set to previously defined number
layout(triangles) in;
layout(triangle_strip, max_vertices = OUT_VERTS) out;


// material
struct Material {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_shininess;
};

uniform Material material;

// view, modelview and projection matrix //
uniform mat4 modelview;
uniform mat4 projection;

// internalTODO: create uniforms for hair paramters (hairLength, hairWidth, hairSegments, gravity) 
#define hairLength 500 * 0.001
#define hairWidth 4 * 0.001
#define hairSegments 15
uniform vec3 gravity;

// TODO: create input and output variables
// - input : normal for each vertex of a triangle
// - output : one single color
in vec3 normal[3];

out vec4 color;

void main(void)
{
	//  compute center vertex and normal of triangle
	vec4 c = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
	vec3 n = (normal[0] + normal[1] + normal[2]) / 3.0;
	n = normalize(n);

	//  compute right vector in view space
	vec3 right_viewspace = cross(c.xyz, ( vec4(n,0)).xyz);
	right_viewspace = normalize(right_viewspace);

	//  compute hair width and length based on respective input parameters and number of segments
	float h_width = hairWidth * 0.0005 / hairSegments; // hair width
	float h_length = hairLength * 0.001 / hairSegments; // hair segment length

	//  compute center position in view space
	vec4 p = c;
	
	//      : compute output vertices
	//	- compute and emit first vertices of the hair
	//	- create for loop for remaining hair segments
	//	- compute new hair width
	//	- increment hair length
	//	- compute and draw shifted vertices along normal and right vector in view space
	//  - emit two segment vertices for each next segment

	//  - end primitive at the end
        float new_Whair = hairWidth;
        float new_Lhair = hairLength;
        color = vec4((material.ambient_color + material.diffuse_color + material.specular_color),1.0) / hairSegments;
        gl_Position = cen_pos + hairWidth * vec4(right_vec,0);
        EmitVertex();
        gl_Position = cen_pos - hairWidth * vec4(right_vec,0);
        EmitVertex();


        for(int i = 1; i < hairSegments; i++){
        new_Whair -= W_hair;
        //new_Lhair -= L_hair;
        cen_pos += L_hair * vec4(tri_normal, 0);
        //  - increment gravity
        tri_normal += gravity;
        tri_normal = normalize(tri_normal);
        right_vec = normalize(cross(cen_pos.xyz,tri_normal));

        color += vec4((material.ambient_color + material.diffuse_color + material.specular_color),1.0) / hairSegments;
        gl_Position = cen_pos + new_Whair * vec4(right_vec,0);
        EmitVertex();
        gl_Position = cen_pos - new_Whair * vec4(right_vec,0);
        EmitVertex();

	// create for loop for hair segments,
	for (int seg = int(hairSegments); seg > 0; seg--) {

	    // compute new hair width in lower and upper part of a hair
		float h_width_1 = seg * h_width;		// hair width	
		float h_width_2 = (seg - 1) * h_width;  // hair width	

		// compute hair color
		//	- you can realize the shading in the same way as in the hair line program
		color = vec4(material.diffuse_color, 1) * seg * (1.0f / hairSegments);
		
		// compute first vertices of triangles
		if (seg == int(hairSegments)) {
			
			vec4 v1_viewspace = p + h_width_1 * vec4(right_viewspace,0);
			vec4 v2_viewspace = p - h_width_1 * vec4(right_viewspace,0);
						
			vec4 v1 = v1_viewspace;
			gl_Position = v1; 	EmitVertex();

			vec4 v2 = v2_viewspace;
			gl_Position = v2;	EmitVertex();
		}

		// increment hair length
		p += h_length * vec4(n,0);

		if (abs(dot(normalize(p), vec4(n,0))) > 0.001) {
			right_viewspace = cross(normalize(p).xyz, (vec4(n,0)).xyz);
			right_viewspace = normalize(right_viewspace);
		}
		
		// compute and draw shifted vertices along hair
		vec4 v3_viewspace = p + h_width_2 * vec4(right_viewspace,0);
		vec4 v4_viewspace = p - h_width_2 * vec4(right_viewspace,0);
		
		vec4 v3 = v3_viewspace;
		gl_Position = v3;	EmitVertex();

		vec4 v4 = v4_viewspace;
		gl_Position = v4;	EmitVertex();	//EndPrimitive();

		// increment gravity
		n += gravity; 
		n = normalize(n);

	}

	EndPrimitive();

}
