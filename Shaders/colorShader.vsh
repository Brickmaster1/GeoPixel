#version 460

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 orthoProjectionMatrix;

void main() {
	//Set the position on screen
    gl_Position.xy = (orthoProjectionMatrix * vec4(vertexPosition, 0.0, 1.0)).xy;
	//The z pos is 0 since we are in 2d
	gl_Position.z = 0.0;
	//Indicate that coords are normalized
	gl_Position.w = 1.0;

	fragmentColor = vertexColor;
	fragmentPosition = vertexPosition;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}