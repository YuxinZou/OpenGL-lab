#version 330
const int maxLightCount = 10;

struct LightSource {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  vec3 position;
};

struct Material {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_shininess;
};

uniform LightSource lightSources[maxLightCount];
uniform int lightsInUse;
uniform Material material;


//define input variable compatible with the output of your shader program
in vec3 vertexNormalInCamSpace;
in vec3 eyeDirectionInCamSpace;
in vec3 lightDirectionInCamSpace[maxLightCount];

// add a texture coordinate as a vertex attribute 
in vec2 textureCoord;

// set up a texture uniform 
uniform sampler2D colorTex;
uniform sampler2D normalTex;

in vec3 dbgColor;


out vec4 color;
void main() {
  // get the texel value from your texture at the position of the passed texture coordinate 
  vec3 color_t = texture2D(colorTex,textureCoord).rgb;
  vec3 normal_t = normalize(texture2D(normalTex,textureCoord).rgb *2 -1);
  
  int lightCount = max(min(lightsInUse, maxLightCount), 0);
  // normalize the vectors passed from your vertex program here //
  // - this needs to be done, because the interpolation of these vectors is linear //
  vec3 E = normalize(eyeDirectionInCamSpace);
//   vec3 N = normalize(vertexNormalInCamSpace);
  vec3 N = normal_t; //use the texture normal instead
  
  // compute the ambient, diffuse and specular color terms as presented in the lecture //
  // note that the multiplication with the material can be done after the summation
  vec3 ambientTerm = vec3(0);
  vec3 diffuseTerm = vec3(0);
  vec3 specularTerm = vec3(0);
  vec3 L = vec3(0);
  vec3 H;
  
  
  for (int i = 0; i < lightCount; ++i) {
    //normalize the light direction for the given lightsource
    L = normalize(lightDirectionInCamSpace[i]);
    
    // add the ambient term for this light source     
    ambientTerm += lightSources[i].ambient_color;
    // add the diffuse term for this light source 
    diffuseTerm += lightSources[i].diffuse_color * max(dot(L, N), 0);
    
    // compute and add the specular term (phong or blinn phong
    //Phong
    vec3 reflectDir = reflect(-L, N);
    specularTerm += lightSources[i].specular_color * pow(max(dot(reflectDir, E), 0), material.specular_shininess/4.0);
    
    //Blinn Phong
    //H = normalize(E + L);
    //specularTerm += lightSources[i].specular_color * pow(max(dot(H, N), 0), material.specular_shininess);
  }
  // apply material
  ambientTerm *= material.ambient_color;
  diffuseTerm *= material.diffuse_color;
  specularTerm *= material.specular_color;
  
  //assign the final color to the fragment output variable //
  //combine the light/material color and the texture color properly //
  color = vec4(color_t, 1.0) * vec4(ambientTerm + diffuseTerm + specularTerm, 1.0); 
  
  //Use for Debugging
//   color = vec4 (dbgColor,1.0);
}


