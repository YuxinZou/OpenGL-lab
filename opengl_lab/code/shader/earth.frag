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

//add a texture coordinate as a vertex attribute 
in vec2 textureCoord;

// set up a texture sampler 
uniform sampler2D texture_bright;
uniform sampler2D texture_dark;
uniform sampler2D texture_cloud;
uniform sampler2D texture_cloud_mask;



out vec4 color;
void main() {
  // get the texel value from your texture at the position of the passed texture coordinate 
  vec3 earthDay = texture2D(texture_bright, textureCoord).rgb;
  vec3 earthNight = texture2D(texture_dark, textureCoord).rgb;
  
  float cloudAlpha = texture2D(texture_cloud_mask,textureCoord).r;
  vec3 clouds = texture2D(texture_cloud,textureCoord).rgb;

  
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
  vec3 L = vec3(0);
  vec3 H;
  
  float nightCoeff = 0;
  
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

    // compute the nightCoeff from the current angle to lightsource 0  (which we assume is the sun in the scene)
    if (i == 0) {
      float angle = dot(L,N);
      float startAngle = 0.05;//start a little bit earlier to mix in the night lights
      if (angle < startAngle) { 
        nightCoeff = min(1.0,1-(angle/startAngle));
      }
    }
    
  }
  // apply material
  ambientTerm *= material.ambient_color;
  diffuseTerm *= material.diffuse_color;
  specularTerm *= material.specular_color;

  //blend different textures and maps with the lighting information to compute the final pixel color
  // - compute the ground color 
  // - compute ground emissive part
  // - blend ground color and clouds 
  // - add emissive part masked by the clouds 
  vec3 groundColor = mix(earthDay,vec3(0.0),nightCoeff);
  vec3 groundEmissive = mix(vec3(0.0),earthNight, nightCoeff);
  color = vec4(mix(groundColor,clouds, cloudAlpha), 1) * vec4(ambientTerm + diffuseTerm + specularTerm, 1) + vec4(mix(groundEmissive,vec3(0.0),cloudAlpha),0.0);
  
}


