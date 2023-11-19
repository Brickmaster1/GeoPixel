#pragma once

#include <string>
#include <GL/glew.h>

class GLSLProgram {
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	void linkShaders();

	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string &uniformName);

	void bindShaderProgram();
	void unbindShaderProgram();
private:
	int _numAttributes;

	void compileShader(const std::string& shaderFilePath, GLuint id);

	GLuint _shaderProgramID;

	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
};

