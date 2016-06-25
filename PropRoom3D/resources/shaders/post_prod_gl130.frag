#version 130

uniform vec2 DepthRange;
uniform sampler2D ImageTex;
uniform sampler2D DepthTex;
uniform vec3 TemperatureRgb;
uniform vec3 ExposureGain;
uniform int AcesTonemappingActive;
uniform float MiddleGrayValue;
uniform float ContrastValue;
uniform float GammaValue;
uniform float AdaptationFactor;
uniform float LowpassKernel[25];
uniform int   LowpassSize;

in vec2 screenCoord;

out vec4 FragColor;


vec3 ImageFilteringFunc()
{
    vec3 refColor = texture(ImageTex, screenCoord).rgb;

    float refWeight = LowpassKernel[12];
    float refIntensity = length(refColor);

    vec3 refDirection =  vec3(0);
    if(refIntensity != 0.0)
        refDirection = refColor / refIntensity;

    float mass = refWeight;
    vec3 color = refColor * refWeight;

    vec3 kernel[25];
    kernel[0] = textureOffset(ImageTex, screenCoord, ivec2(-2, -2)).rgb;
    kernel[1] = textureOffset(ImageTex, screenCoord, ivec2(-1, -2)).rgb;
    kernel[2] = textureOffset(ImageTex, screenCoord, ivec2( 0, -2)).rgb;
    kernel[3] = textureOffset(ImageTex, screenCoord, ivec2( 1, -2)).rgb;
    kernel[4] = textureOffset(ImageTex, screenCoord, ivec2( 2, -2)).rgb;

    kernel[5] = textureOffset(ImageTex, screenCoord, ivec2(-2, -1)).rgb;
    kernel[6] = textureOffset(ImageTex, screenCoord, ivec2(-1, -1)).rgb;
    kernel[7] = textureOffset(ImageTex, screenCoord, ivec2( 0, -1)).rgb;
    kernel[8] = textureOffset(ImageTex, screenCoord, ivec2( 1, -1)).rgb;
    kernel[9] = textureOffset(ImageTex, screenCoord, ivec2( 2, -1)).rgb;

    kernel[10] = textureOffset(ImageTex, screenCoord, ivec2(-2,  0)).rgb;
    kernel[11] = textureOffset(ImageTex, screenCoord, ivec2(-1,  0)).rgb;
    kernel[12] = refColor;
    kernel[13] = textureOffset(ImageTex, screenCoord, ivec2( 1,  0)).rgb;
    kernel[14] = textureOffset(ImageTex, screenCoord, ivec2( 2,  0)).rgb;

    kernel[15] = textureOffset(ImageTex, screenCoord, ivec2(-2,  1)).rgb;
    kernel[16] = textureOffset(ImageTex, screenCoord, ivec2(-1,  1)).rgb;
    kernel[17] = textureOffset(ImageTex, screenCoord, ivec2( 0,  1)).rgb;
    kernel[18] = textureOffset(ImageTex, screenCoord, ivec2( 1,  1)).rgb;
    kernel[19] = textureOffset(ImageTex, screenCoord, ivec2( 2,  1)).rgb;

    kernel[20] = textureOffset(ImageTex, screenCoord, ivec2(-2,  2)).rgb;
    kernel[21] = textureOffset(ImageTex, screenCoord, ivec2(-1,  2)).rgb;
    kernel[22] = textureOffset(ImageTex, screenCoord, ivec2( 0,  2)).rgb;
    kernel[23] = textureOffset(ImageTex, screenCoord, ivec2( 1,  2)).rgb;
    kernel[24] = textureOffset(ImageTex, screenCoord, ivec2( 2,  2)).rgb;

    for(int j=-2; j<=2; ++j)
    {
        for(int i=-2; i<=2; ++i)
        {
            if(i == 0 && j == 0)
                continue;

            int idx = (j+2)*5 + i + 2;
            vec3 neightboorColor = kernel[idx];
            float neightboorWeight = LowpassKernel[idx];

            if(AdaptationFactor != 0.0)
            {
                if(neightboorColor != vec3(0))
                {
                    float neightboorIntensity = length(neightboorColor);

                    float intensityContribution = 1.0 -
                            abs(refIntensity - neightboorIntensity) /
                               (refIntensity + neightboorIntensity);

                    vec3 neightboorDirection = neightboorColor / neightboorIntensity;
                    float directionContribution = dot(refDirection, neightboorDirection);
                    float adaptationWeight = directionContribution * intensityContribution;
                    float adaptation = mix(1.0, adaptationWeight, AdaptationFactor);

                    neightboorWeight *= adaptation;
                }
            }

            mass += neightboorWeight;
            color += neightboorColor * neightboorWeight;
        }
    }

    return color / mass;
}

vec3 adjustWhite(vec3 color)
{
    return TemperatureRgb * color;
}

vec3 adjustIntensity(vec3 color)
{
    vec3 luminosityColor = color - MiddleGrayValue;
    vec3 contrastColor = luminosityColor * ContrastValue;
    return contrastColor + vec3(0.5);
}

vec3 tonemapAces(in vec3 x)
{
    if(AcesTonemappingActive == 1)
    {
        x = clamp(x, 1.0e-2, 1.0e1);

        float a = 2.51f;
        float b = 0.03f;
        float c = 2.43f;
        float d = 0.59f;
        float e = 0.14f;
        return min((x*(a*x+b))/(x*(c*x+d)+e), 1.0);
    }
    else
    {
        return x;
    }
}

vec3 correctGamma(vec3 color)
{
    return pow(color, vec3(1.0 / GammaValue));
}

void main()
{
    vec3 originalColor = ImageFilteringFunc();
    vec3 exposedColor = ExposureGain * originalColor;
    vec3 adjustedWhiteColor = adjustWhite(exposedColor);
    vec3 tonemappedColor = tonemapAces(adjustedWhiteColor);
    vec3 saturatedColor = max(tonemappedColor, 0.0);
    vec3 gammaCorrectedColor = correctGamma(saturatedColor);
    vec3 adjustedIntensityColor = adjustIntensity(gammaCorrectedColor);

    FragColor = vec4(adjustedIntensityColor, 1);

    // Depth
    float n = -DepthRange[0];
    float f = -DepthRange[1];
    float wd = texture(DepthTex, screenCoord).r;
    float ndcDepth = (wd*(-(f+n)) - 2*f*n) / (-wd * (f-n));
    gl_FragDepth = clamp((ndcDepth + 1.0) / 2.0, 0.0, 0.9999);
}
