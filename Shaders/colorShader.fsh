#version 460

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

//uniform float time;
uniform sampler2D textureSampler;

void main() {
    vec4 textureColor = texture(textureSampler, fragmentUV);
    
    color = textureColor * fragmentColor;
}