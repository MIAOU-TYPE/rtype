#version 130

uniform sampler2D texture;
uniform int mode;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    if (mode == 1) {
        // Deuteranopia
        color.rgb = vec3(
            0.625 * color.r + 0.375 * color.g,
            0.70  * color.r + 0.30  * color.g,
            color.b
        );
    }
    else if (mode == 2) {
        // Protanopia
        color.rgb = vec3(
            0.567 * color.r + 0.433 * color.g,
            0.558 * color.r + 0.442 * color.g,
            color.b
        );
    }
    else if (mode == 3) {
        // Tritanopia
        color.rgb = vec3(
            color.r,
            0.95 * color.g + 0.05 * color.b,
            0.433 * color.g + 0.567 * color.b
        );
    }

    gl_FragColor = vec4(color.rgb, color.a);
}
