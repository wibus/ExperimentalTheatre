#version 120

uniform sampler2D Texture;
uniform vec4 ColorFilter;

varying vec2 relpos;
varying vec3 abspos;
varying vec2 tex;
varying vec4 col;


void main()
{
   gl_FragColor = ColorFilter * col * texture2D(Texture, tex);
}
