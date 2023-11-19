#version 460
//Fragment shader operates on each pixel in a polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform float time;
uniform sampler2D textureSampler;

void main() {

    vec4 textureColor = texture(textureSampler, fragmentUV);

    color = textureColor * fragmentColor;

    //color = fragmentColor + vec4(fragmentColor.r * (cos(fragmentPosition.x * 4.0 + time) + 1.0) * 0.5,
    //                             fragmentColor.g * (cos(fragmentPosition.y * 4.0 + time) + 1.0) * 0.5,
    //                             fragmentColor.b * (cos(fragmentPosition.x * 2.0 + time) + 1.0) * 0.5,
    //                             fragmentColor.a);
}

