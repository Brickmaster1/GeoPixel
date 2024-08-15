#pragma once

#include <ViXeL/data/ResourceManager.h>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>



inline void fetchSprites(std::vector<std::filesystem::path>* loadedTexturesList) {
    for (auto& it : std::filesystem::recursive_directory_iterator("resources/Textures")) {
		if (it.path().extension() == ".png") {
			ViXeL::ResourceManager::getTexture(it.path());
			loadedTexturesList->push_back(it.path());
			std::cout << it.path().string() << std::endl;
		}
	}
}

inline void fetchShader() {
	//static ViXeL::GLShader shader = ViXeL::ResourceManager::getShader("Shaders/Test_PixelCube.vert", "Shaders/Test_PixelCube.frag");
}
