#include "GlArtDirectorShaders.h"

namespace prop2
{
const std::string COMMON_VERTEX_SHADER = 
    "#version 120\n"
    "uniform mat4 Projection;\n"
    "uniform mat3 ModelView;\n"
    "uniform float Depth;\n"
    "uniform vec2 TexOffset = vec2(0.0, 0.0);\n"
    "uniform float TexStretch = 1.0;\n"
    "attribute vec2 position;\n"
    "attribute vec2 texCoord;\n"
    "attribute vec4 color;\n"
    "varying vec2 relpos;\n"
    "varying vec3 abspos;\n"
    "varying vec2 tex;\n"
    "varying vec4 col;\n"
    "void main()\n"
    "{"
    "   col = color;\n"
    "   tex = texCoord*TexStretch + TexOffset;\n"
    "   relpos = position;\n"
    "   abspos = vec3((ModelView * vec3(relpos, 1.0)).xy, -Depth);\n"
    "   gl_Position = Projection * vec4(abspos, 1.0);\n"
    "}";

const std::string CIRCLE_FRAGMENT_SHADER =
    "#version 120\n"
    "uniform sampler2D Texture;\n"
    "uniform vec4 ColorFilter;\n"
    "varying vec2 relpos;\n"
    "varying vec3 abspos;\n"
    "varying vec2 tex;\n"
    "varying vec4 col;\n"
    "void main()\n"
    "{"
    "   if(length(relpos) > 1.0)\n"
    "       discard;\n"
    "   gl_FragColor = ColorFilter * texture2D(Texture, tex);\n"
    "}";

const std::string POLYGON_FRAGMENT_SHADER =
    "#version 120\n"
    "uniform sampler2D Texture;\n"
    "uniform vec4 ColorFilter;\n"
    "varying vec2 relpos;\n"
    "varying vec3 abspos;\n"
    "varying vec2 tex;\n"
    "varying vec4 col;\n"
    "void main()\n"
    "{"
    "   gl_FragColor = ColorFilter * col * texture2D(Texture, tex);\n"
    "}";


const std::string HUD_VERTEX_SHADER =
    "#version 120\n"
    "uniform mat4 Projection;\n"
    "uniform vec2 Anchor;\n"
    "attribute vec2 position;\n"
    "attribute vec2 texCoord;\n"
    "varying vec2 tex;\n"
    "void main()\n"
    "{\n"
    "   tex = texCoord;\n"
    "   gl_Position = Projection * vec4(Anchor + position, 0.0, 1.0);\n"
    "}\n";

const std::string HUD_FRAGMENT_SHADER =
    "#version 120\n"
    "uniform sampler2D Texture;\n"
    "uniform vec4 ColorFilter;\n"
    "varying vec2 tex;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor = ColorFilter * texture2D(Texture, tex);\n"
    "}\n";
}
