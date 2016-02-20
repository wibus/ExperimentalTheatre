#version 400

uniform mat4 PVM;

in vec3 position;


void main()
{
    gl_Position = PVM * vec4(position, 1);
}
