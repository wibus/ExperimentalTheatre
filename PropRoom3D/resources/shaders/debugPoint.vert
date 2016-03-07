#version 400

uniform mat4 P;
uniform mat4 VM;

in vec3 position;


void main()
{
    const float OFFSET = 0.02; // meters
    gl_Position = P * (VM * vec4(position, 1) + vec4(0, 0, OFFSET, 0));
}
