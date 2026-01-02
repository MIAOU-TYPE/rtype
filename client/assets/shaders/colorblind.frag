#version 120

uniform sampler2D texture;
uniform int mode;

const int NONE = 0;
const int DEUTERANOPIA = 1;
const int PROTANOPIA = 2;
const int TRITANOPIA = 3;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    if (mode == DEUTERANOPIA) {
        color.rgb = vec3(
            0.625 * color.r + 0.375 * color.g,
            0.70  * color.r + 0.30  * color.g,
            color.b
        );
    }
    else if (mode == PROTANOPIA) {
        color.rgb = vec3(
            0.567 * color.r + 0.433 * color.g,
            0.558 * color.r + 0.442 * color.g,
            color.b
        );
    }
    else if (mode == TRITANOPIA) {
        color.rgb = vec3(
            color.r,
            0.95 * color.g + 0.05 * color.b,
            0.433 * color.g + 0.567 * color.b
        );
    }

    gl_FragColor = vec4(color.rgb, color.a);
}
