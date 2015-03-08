#version 130

in vec3 position;

out vec2 screenCoord;


void main()
{
    screenCoord = (position.xy + vec2(1.0)) /2.0;
    gl_Position = vec4(position, 1);
}
