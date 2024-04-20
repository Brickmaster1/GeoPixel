#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 cameraMatrix;
uniform mat4 model;

void main(void) {
    gl_Position = cameraMatrix * model * vec4(vertexPosition, 1.0f);

    fragmentColor = vertexColor;
    fragmentUV = vertexUV;
}
