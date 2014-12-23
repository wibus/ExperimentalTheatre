#version 400

uniform sampler2D ImageTex;

in vec2 screenCoord;

layout(location=0) out vec4 FragColor;

void main()
{
    FragColor = texture(ImageTex, screenCoord);
}
