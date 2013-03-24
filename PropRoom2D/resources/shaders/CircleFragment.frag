#version 120

uniform sampler2D Texture;
uniform vec4 ColorFilter;

varying vec2 relpos;
varying vec3 abspos;
varying vec2 tex;
varying vec4 col;


void main()
{
   if(length(relpos) > 1.0)
       discard;
   gl_FragColor = ColorFilter * texture2D(Texture, tex);
}
