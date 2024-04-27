#version 330 core
in vec2 TexCoords;
in vec4 WorldCoords;
in mat4 modelMat;
out vec4 color;

uniform sampler2D image;
uniform vec2 terrainSize;
uniform vec2 mousePos;
uniform float time;

vec3 sunPos = vec3(mousePos, 1);


int STEPS = 200;
float waterValue = 0.25;

float getTerrainHeight(vec2 pos)
{
    vec4 val = texture(image, pos);
    return (val.x + val.y + val.z + val.w)/4.0;
}

float easeOut(float x) {
  return clamp(1. - pow(1. - x, 6.), 0., 1.);
}

void main() {
    float terrainValue = getTerrainHeight(TexCoords);
    sunPos = vec3(800, 600, 1);

    sunPos = vec3(terrainSize.x * cos(time/2), terrainSize.y*sin(time/2), 1);

    vec3 pos = vec3(WorldCoords.xy, max(terrainValue, waterValue));
    vec3 stepDir = (sunPos - pos)/STEPS;

    float inShadow = 0;
    for (int i = 0; i < STEPS; i++)
    {
        pos += stepDir;

        float h = getTerrainHeight(pos.xy/terrainSize);
        if (h > pos.z)
        {
            inShadow = 0.5;
            break;
        }
        if (pos.z > 1.0)
        {
            break;
        }
    }

    if (terrainValue < waterValue) {
        color = vec4(0.4, 0.4, 1.0, 1.0);
    } else if (terrainValue < 0.3) {
        color = vec4(0.788, 0.674, 0.443, 1.0);
    } else if (terrainValue < 0.45) {
        color = vec4(0.5, 0.8, 0.3, 1.0);
    } else {
        color = vec4(0.5, 0.5, 0.5, 1.0);
    }

    vec4 shadowColor = vec4(0.137, 0.196, 0.259, 1.0);

    float waterDepth = waterValue - getTerrainHeight(TexCoords);
    float waterLerp = easeOut(waterDepth/waterValue);

    color = mix(color, vec4(0.0, 0.0, 1.0, 1.0), waterLerp);

    if (waterDepth > 0 && waterDepth < 0.05)
    {
        color = mix(color, vec4(1.0, 1.0, 1.0, 1.0), (1-waterDepth*20)*0.3* (sin((-time/50 + waterDepth)*100)/2+0.5));
    }
    

    color = mix(color, shadowColor, inShadow);

    



}