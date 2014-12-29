#version 400

uniform sampler2D ImageTex;
uniform vec3 TemperatureRgb;
uniform float ContrastValue;
uniform float LuminosityValue;
uniform float LowpassKernel[25];
uniform int   LowpassSize;

in vec2 screenCoord;

layout(location=0) out vec4 FragColor;

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


vec3 lowpassMeanFiltering()
{
    vec3 meanColor = vec3(0.0);
    int top = kTop[LowpassSize];

    for(int i=0; i < top; ++i)
        meanColor += textureOffset(ImageTex, screenCoord, kOff[i]).rgb *
            LowpassKernel[kIdx[i]];

    return meanColor;
}

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


void main()
{
    vec3 originalColor = lowpassMeanFiltering();
    vec3 adjustedWhiteColor = adjustWhite(originalColor);
    vec3 adjustedIntensityColor = adjustIntensity(adjustedWhiteColor);
    vec3 saturatedColor = clamp(adjustedIntensityColor, 0.0, 1.0);

    FragColor = vec4(saturatedColor, 1);
}
