#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <ViXeL/data/ResourceManager.h>

void fetchSprites(std::vector<std::string>* loadedTexturesList) {
	for (auto& it : std::filesystem::recursive_directory_iterator("Textures")) {
		if (it.path().extension() == ".png") {
			ViXeL::ResourceManager::getTexture(it.path().string());
			loadedTexturesList->push_back(it.path().string());
			std::cout << it.path().string() << std::endl;
		}
	}
}

void fetchShader() {
	//static ViXeL::GLShader shader = ViXeL::ResourceManager::getShader("Shaders/Test_PixelCube.vert", "Shaders/Test_PixelCube.frag");
}
