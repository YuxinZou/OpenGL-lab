#version 330
// add a texture coordinate as a vertex attribute 
in vec2 textureCoord;

// set up a texture uniform 
uniform sampler2D texture;

out vec4 color;
void main() {
  // get the texel value from your texture at the position of the passed texture coordinate 
  vec3 texColor = texture2D(texture, textureCoord).rgb;
  
  // combine the light/material color and the texture color properly //
  color = vec4(texColor, 1);



//Debugging hints:
//   color = vec4(0.5,0.5,0.0,1.0); //fixed color
   //color = vec4(texcoord,0,1); //uv coords
    // color = vec4(texture_color.rgb,1);

}
