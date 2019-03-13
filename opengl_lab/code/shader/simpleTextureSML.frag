#version 330

in vec2 textureCoord;

in vec4 vertexInLightViewSpace; 
in vec4 vertexInLightViewSpaceProjected;
in vec3 surfaceNormalInLightViewSpace;

uniform sampler2D tex;

uniform sampler2D shadowMap;
// uniform sampler2DShadow shadowMap;

uniform float baseIntensity = 1.0;

out vec4 color;

void main() {
  float light= 0.4; //ambient
  vec3 texColor = texture(tex, textureCoord).rgb;

  //TODO Calculate the additional light from our light source (you can assume that our scene is perfectly diffuse)
  vec3 lightColor = vec3(1.0);

  // - prepare light dir and normal for the phon coefficient calculation
  vec3 lightDirection = -normalize(vertexInLightViewSpace.xyz);
  vec3 normal = normalize(surfaceNormalInLightViewSpace);

  float diff = max(dot(lightDirection, surfaceNormalInLightViewSpace),0.0);
  vec3 diffuse = diff * lightColor;
  // - apply perspective division to the projected vertex position and check against the camera frustum
  vec3 projCoords = vertexInLightViewSpaceProjected.xyz / vertexInLightViewSpaceProjected.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(shadowMap, projCoords.xy).r;
  float currentDepth = projCoords.z;
  float shadow = currentDepth > closestDepth ? 1.0: 0.0;
  float intensityCo = 1 / dot(vertexInLightViewSpace, vertexInLightViewSpace) * 2;
  if(dot(lightDirection, normal) < 0.26)
      intensityCo = 0;
  if (projCoords.x < 0  || projCoords.x > 1 || projCoords.y <0 || projCoords.y >1 )
      shadow = 0;
  vec3 lighting;
  lighting = texColor*light + (1.0-shadow)*diffuse*texColor * intensityCo * baseIntensity;
  color = vec4(lighting,1.0f);



  //   - if inside the frustum get the depth from the shadow map and compare to the object depth and compute the resulting light
  //     (do not forget to apply the phong coefficient) 
  //   - also add baseIntensity which is used to toggle the lamp on and off 
  //   - finally create a nice and smooth rounded spotlight by making the light intensity dependent on the distance to the optical axis 

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //color = vec4(texColor*light,1);
  
  
  //TODO: remove this line so the actual shader takes effekt 
  //color = vec4(texColor,1);
  
  
  
  
  
//Debugging Hints
//  visualize the frustum
//if (abs(abs(l_lightPos.x)-1)< 0.002 || abs(abs(l_lightPos.y)-1)< 0.002 )  color = vec4(1,0,0,1);
// visualize normal / light dir
//color = vec4((f_normal+1.0)/2.0,1.0);
// color = vec4((lightDirection+1.0)/2.0,1.0);

//  visualize self occluded areas
//if (dot(f_ldir,f_normal) < 0) color = vec4(-dot(f_ldir,f_normal),0,1,1);

}

