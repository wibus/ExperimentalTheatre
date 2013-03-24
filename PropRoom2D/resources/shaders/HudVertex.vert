#version 120

uniform mat4 Projection;
uniform vec2 Anchor;

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 tex;


void main()
{
   tex = texCoord;
   gl_Position = Projection * vec4(Anchor + position, 0.0, 1.0);
}
