#include "ImageLoader.h"
#include "lodepng/lodepng.h"
#include "IOManager.h"
#include "Errors.h"
#include "iostream"

GLTexture ImageLoader::loadPNG(const std::string& filePath) {
	GLTexture texture = {};


	std::vector<unsigned char> in;
	std::vector<unsigned char> out;

	unsigned int width, height;

	if (IOManager::readFileToBuffer(filePath, in) == false) {
		fatalError("Failed to load PNG file into buffer!");
	}

	unsigned int code = lodepng::decode(out, width, height, in.data(), in.size());
	if (code != 0) {
		fatalError("Failed to decode PNG data! Error description:\n");
		printf("%s\n", lodepng_error_text(code));
		std::cout << "Error code: " + code;
	}

	glGenTextures(1, &texture.id);

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, out.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}