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


out vec4 color;
void main() {
  int lightCount = max(min(lightsInUse, maxLightCount), 0);
  // normalize the vectors passed from your vertex program here //
  // - this needs to be done, because the interpolation of these vectors is linear //
  vec3 E = normalize(eyeDirectionInCamSpace);
  vec3 N = normalize(vertexNormalInCamSpace);
  
  // compute the ambient, diffuse and specular color terms as presented in the lecture //
  // note that the multiplication with the material can be done after the summation
  vec3 ambientTerm = vec3(0);
  vec3 diffuseTerm = vec3(0);
  vec3 specularTerm = vec3(0);
  vec3 L = vec3(0); //light direction maybe, but i use direction_to_light directliy
  vec3 H;

  
  
  for (int i = 0; i < lightCount; ++i) {
    //TODO normalize the light direction for the given lightsource
    L = normalize(lightDirectionInCamSpace[i]);
    
    //TODO add the ambient term for this light source     
    ambientTerm += lightSources[i].ambient_color;
    //TODO add the diffuse term for this light source 
    diffuseTerm += lightSources[i].diffuse_color * max(dot(L, N), 0);
    
    //TODO compute and add the specular term (phong or blinn phong
    //Phong
    vec3 reflectDir = reflect(-L, N);
    specularTerm += lightSources[i].specular_color * pow(max(dot(reflectDir, E), 0), material.specular_shininess/4.0);
    
    //Blinn Phong
    //H = normalize(E + L);
    //specularTerm += lightSources[i].specular_color * pow(max(dot(H, N), 0), material.specular_shininess);
  }
  
  //TODO apply material
  ambientTerm *= material.ambient_color;
  diffuseTerm *= material.diffuse_color;
  specularTerm *= material.specular_color;
  
  // TODO: assign the final color to the fragment output variable //
  color = vec4(ambientTerm + diffuseTerm + specularTerm, 1);
  //TODO: remove this line to allow rendering of the calculated colors
//   color = vec4(0.0,0.0,0.5,1);
}