uniform sampler2D texture;
uniform int colorBlindMode; // 0 = off, 1 = protanopia, 2 = deuteranopia, 3 = tritanopia

vec3 simulateProtanopia(vec3 c) {
    // approximation widely used
    mat3 m = mat3(
        0.567, 0.433, 0.0,
        0.558, 0.442, 0.0,
        0.0,   0.242, 0.758
    );
    return m * c;
}

vec3 simulateDeuteranopia(vec3 c) {
    mat3 m = mat3(
        0.625, 0.375, 0.0,
        0.7,   0.3,   0.0,
        0.0,   0.3,   0.7
    );
    return m * c;
}

vec3 simulateTritanopia(vec3 c) {
    mat3 m = mat3(
        0.95, 0.05, 0.0,
        0.0,  0.433, 0.567,
        0.0,  0.475, 0.525
    );
    return m * c;
}

void main()
{
    vec4 src = texture2D(texture, gl_TexCoord[0].st);
    vec3 rgb = src.rgb;

    if (colorBlindMode == 0) {
        gl_FragColor = src;
        return;
    }

    vec3 sim;
    if (colorBlindMode == 1) {
        sim = simulateProtanopia(rgb);
    } else if (colorBlindMode == 2) {
        sim = simulateDeuteranopia(rgb);
    } else if (colorBlindMode == 3) {
        sim = simulateTritanopia(rgb);
    } else {
        sim = rgb;
    }

    gl_FragColor = vec4(sim, src.a);
}
