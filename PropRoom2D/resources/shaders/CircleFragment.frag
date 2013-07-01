#version 120

uniform sampler2D Texture;
uniform vec4 ColorFilter;
uniform float Radius;

varying vec2 relpos;
varying vec3 abspos;
varying vec2 tex;
varying vec4 col;


void main()
{
    float window = 2.0/Radius;
    float center = 1.0 - window/2.0;
    float dist2 = relpos.x*relpos.x + relpos.y*relpos.y;
    float coverage = 1.0 - smoothstep(center-window, center+window, dist2);

    vec4 filter = ColorFilter;
    filter.a *= coverage;

    gl_FragColor = filter * texture2D(Texture, tex);
}
