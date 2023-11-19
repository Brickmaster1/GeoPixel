#version 460
//Vertex shader operates on each vertex 

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV;

void main() {
	//Set the x, y pos on screen
	gl_Position.xy = vertexPosition;
	//The z pos is 0 since we are in 2d
	gl_Position.z = 0.0;
	//Indicate that coords are normalized
	gl_Position.w = 1.0;

	fragmentColor = vertexColor;
	fragmentPosition = vertexPosition;
	fragmentUV = vertexUV;
}