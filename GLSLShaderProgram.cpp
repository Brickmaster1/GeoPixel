#include "GLSLShaderProgram.h"
#include "Errors.h"

#include <iostream>
#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram() : _shaderProgramID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAttributes(0) {

}

GLSLProgram::~GLSLProgram() {

}

void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	_shaderProgramID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		fatalError("Vertex shader failed to be created!");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		fatalError("Fragment shader failed to be created!");
	}

	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);
}

void GLSLProgram::linkShaders() {
	// Attach our shaders to our program
	glAttachShader(_shaderProgramID, _vertexShaderID);
	glAttachShader(_shaderProgramID, _fragmentShaderID);

	// Link our program
	glLinkProgram(_shaderProgramID);

	GLint isLinked = 0;
	glGetShaderiv(_shaderProgramID, GL_LINK_STATUS, &isLinked);
	if (&isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(_shaderProgramID, maxLength, &maxLength, errorLog.data());

		std::printf("%s\n", errorLog.data());
		fatalError("Shaders failed to link!");

		// We don't need the program anymore.
		glDeleteProgram(_shaderProgramID);
		// Don't leak shaders either.
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	// Always detach shaders after a successful link.
	glDetachShader(_shaderProgramID, _vertexShaderID);
	glDetachShader(_shaderProgramID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

//Adds an attribute to our shader; Called between compiling and linking
void GLSLProgram::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(_shaderProgramID, _numAttributes++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(_shaderProgramID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform \"" + uniformName + "\" is invalid! Does it exist?");
	}
	return location;
}

void GLSLProgram::bindShaderProgram() {
	glUseProgram(_shaderProgramID);
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unbindShaderProgram() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::compileShader(const std::string& shaderFilePath, GLuint id) {
	std::ifstream vertexFile(shaderFilePath);
	if (vertexFile.fail()) {
		perror(shaderFilePath.c_str());
		fatalError("Could not open shader file! Path: " + shaderFilePath);
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(vertexFile, line)) {
		fileContents += line + "\n";
	}

	vertexFile.close();

	const char* fileContentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &fileContentsPtr, nullptr);
	
	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if (&isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);

		printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + shaderFilePath + " failed to compile!");
	}
}