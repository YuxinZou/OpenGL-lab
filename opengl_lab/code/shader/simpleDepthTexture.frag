#version 330
// add a texture coordinate as a vertex attribute 
in vec2 textureCoord;

// set up a texture uniform 
uniform sampler2D texture;

out vec4 color;


#define MAX_DEPTH 30.f




float linstep(float low, float high, float value){
    return clamp((value-low)/(high-low), 0.0, 1.0);
}

float fade(float low, float high, float value){
    float mid = (low+high)*0.5;
    float range = (high-low)*0.5;
    float x = 1.0 - clamp(abs(mid-value)/range, 0.0, 1.0);
    return smoothstep(0.0, 1.0, x);
}

vec3 getColor(float intensity){
    vec3 blue = vec3(0.0, 0.0, 1.0);
    vec3 cyan = vec3(0.0, 1.0, 1.0);
    vec3 green = vec3(0.0, 1.0, 0.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 red = vec3(1.0, 0.0, 0.0);

    vec3 color = (
        fade(-0.25, 0.25, intensity)*blue +
        fade(0.0, 0.5, intensity)*cyan +
        fade(0.25, 0.75, intensity)*green +
        fade(0.5, 1.0, intensity)*yellow +
        smoothstep(0.75, 1.0, intensity)*red
    );
    return color;
}



float LinearizeDepth(in float depth)
{
    float zNear = 0.1;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 10.0; // TODO: Replace by the zFar  of your perspective projection
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

// void main()
// {
//     float c = LinearizeDepth(texCoords);
//     gl_fragColor = vec4(c, c, c, 1.0);
// }







void main() {
//   float depth = smoothstep(0.0, 1.0, texture2D(texture, textureCoord).r/MAX_DEPTH);
//   color = vec4(getColor(depth),1.0);

  float depth = LinearizeDepth(texture2D(texture, textureCoord).x);
//   color = vec4(getColor(1-depth),1.0);
  color = vec4(1-depth,1-depth,1-depth,1);
//   color = vec4(0,1-depth,0,1);
//   color = vec4(depth,depth,depth,1);

}
