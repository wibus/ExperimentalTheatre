#version 120

uniform sampler2D Texture;
uniform vec4 ColorFilter;

varying vec2 tex;


void main()
{
   gl_FragColor = ColorFilter * texture2D(Texture, tex);
}
