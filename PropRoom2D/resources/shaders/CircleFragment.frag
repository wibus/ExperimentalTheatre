#version 120

uniform sampler2D Texture;
uniform vec4 ColorFilter;
uniform float Radius;
uniform vec2 Zoom;

varying vec2 relpos;
varying vec3 abspos;
varying vec2 tex;
varying vec4 col;


void main()
{
    // TODO wbussiere 2014-09-07 : Compute window outside shader
    vec2 dir = relpos * Zoom * Radius;
    float dist = Radius - length(dir);
    float coverage = smoothstep(0.0, 1.0, dist);

    vec4 filter = ColorFilter;
    filter.a *= coverage;

    gl_FragColor = filter * texture2D(Texture, tex);
}
