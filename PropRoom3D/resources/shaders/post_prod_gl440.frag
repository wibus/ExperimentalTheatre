#version 440

uniform sampler2D ImageTex;
uniform vec3 TemperatureRgb;
uniform float ContrastValue;
uniform float LuminosityValue;
uniform float AdaptationFactor;
uniform float LowpassKernel[25];
uniform int   LowpassSize;

in vec2 screenCoord;

out vec4 FragColor;


// Shader constant data
const int kTop[] = int[](1, 9, 25);

const int kIdx[] =  int[](
    12,

    6,  7,  8,
    11,     13,
    16, 17, 18,

    0,   1,  2,  3, 4,
    5,              9,
    10,             14,
    15,             19,
    20, 21, 22, 23, 24
);

const ivec2 kOff[] = ivec2[](
    ivec2(0, 0),

    ivec2(-1, -1), ivec2( 0, -1), ivec2( 1, -1),
    ivec2(-1,  0),                ivec2( 1,  0),
    ivec2(-1,  1), ivec2( 0,  1), ivec2( 1,  1),

    ivec2(-2, -2), ivec2( -1, -2), ivec2( 0, -2), ivec2( 1, -2), ivec2( 1, -2),
    ivec2(-2, -1),                                               ivec2( 2, -1),
    ivec2(-2,  0),                                               ivec2( 2,  0),
    ivec2(-2,  1),                                               ivec2( 2,  1),
    ivec2(-2,  2), ivec2( -1,  2), ivec2( 0,  2), ivec2( 1,  2), ivec2( 1,  2)
);


// Filtering subroutines
subroutine vec3 imageFiltering();
layout(location=0) subroutine uniform imageFiltering ImageFilteringFunc;

layout(index=0) subroutine(imageFiltering) vec3 diracFiltering()
{
    return texture(ImageTex, screenCoord).rgb;
}

layout(index=1) subroutine(imageFiltering) vec3 lowpassMeanFiltering()
{
    vec3 meanColor = vec3(0.0);
    int top = kTop[LowpassSize];

    for(int i=0; i < top; ++i)
        meanColor += textureOffset(ImageTex, screenCoord, kOff[i]).rgb *
            LowpassKernel[kIdx[i]];

    return meanColor;
}

layout(index=2) subroutine(imageFiltering) vec3 lowpassAdaptativeFiltering()
{
    if(AdaptationFactor == 0.0)
        return lowpassMeanFiltering();

    vec3 refColor = texture(ImageTex, screenCoord).rgb;
    float refWeight = LowpassKernel[kIdx[0]];
    float refIntensity = length(refColor);
    vec3 refDirection = vec3(0);
    if(refIntensity != 0.0)
        refDirection = refColor / refIntensity;

    float mass = refWeight;
    vec3 color = refColor * refWeight;
    int top = kTop[LowpassSize];

    for(int i=1; i<top; ++i)
    {
        vec3 neightborColor = textureOffset(ImageTex, screenCoord, kOff[i]).rgb;
        float neightborWeight = LowpassKernel[kIdx[i]];

        if(neightborColor != vec3(0))
        {
            float neightborIntensity = length(neightborColor);

            float intensityContribution = 1.0 -
                    abs(refIntensity - neightborIntensity) /
                       (refIntensity + neightborIntensity);

            vec3 neightborDirection = neightborColor / neightborIntensity;
            float directionContribution = dot(refDirection, neightborDirection);
            float adaptationWeight = directionContribution * intensityContribution;
            float adaptation = mix(1.0, adaptationWeight, AdaptationFactor);

            neightborWeight *= adaptation;
        }

        color += neightborColor * neightborWeight;
        mass += neightborWeight;
    }

    return color / mass;
}


// Fixed fonctions
vec3 adjustWhite(vec3 color)
{
    return TemperatureRgb * color;
}

vec3 adjustIntensity(vec3 color)
{
    vec3 centeredColor = color - vec3(0.5);
    vec3 luminosityColor = centeredColor + vec3(LuminosityValue);
    vec3 contrastColor = luminosityColor * ContrastValue;
    return contrastColor + vec3(0.5);
}


// Main
void main()
{
    vec3 originalColor = ImageFilteringFunc();
    vec3 adjustedWhiteColor = adjustWhite(originalColor);
    vec3 adjustedIntensityColor = adjustIntensity(adjustedWhiteColor);
    vec3 saturatedColor = clamp(adjustedIntensityColor, 0.0, 1.0);

    FragColor = vec4(saturatedColor, 1);
}
