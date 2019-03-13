// simple fragment shader that outputs hair color

#version 330

out vec4 fragColor;

in vec4 color;

void main()
{		
	fragColor = color;
//   fragColor= vec4(.5,.5,0.0,1.0);
}
