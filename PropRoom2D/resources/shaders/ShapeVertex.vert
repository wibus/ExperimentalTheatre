#version 120

uniform mat4 Projection;
uniform mat3 ModelView;
uniform float Depth;
uniform vec2 TexOffset = vec2(0.0, 0.0);
uniform float TexStretch = 1.0;

attribute vec2 position;
attribute vec2 texCoord;
attribute vec4 color;

varying vec2 relpos;
varying vec3 abspos;
varying vec2 tex;
varying vec4 col;


void main()
{
   col = color;
   tex = texCoord*TexStretch + TexOffset;
   relpos = position;
   abspos = vec3(vec2(ModelView * vec3(relpos, 1.0)), -Depth);
   gl_Position = Projection * vec4(abspos, 1.0);
}
