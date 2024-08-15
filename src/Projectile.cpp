#include "Projectile.h"

#include <ViXeL/data/ResourceManager.h>
#include <ViXeL/data/Vertex.h>

Projectile::Projectile(glm::vec2 pos, glm::vec2 motionVec, float speed, int lifetime, glm::vec4 hitbox, float rotation) :
	_pos(pos),
	_motionVec(motionVec),
	_speed(speed),
	_hitbox(hitbox),
	_rotation(rotation),
	_lifetime(lifetime)
{}

Projectile::~Projectile() {}

void Projectile::render(ViXeL::SpriteBatcher& spriteBatch) {
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static ViXeL::GLTexture texture = ViXeL::ResourceManager::getTexture("Textures/Test_PixelCube.png");
	ViXeL::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posRect = glm::vec4(_pos.x - _hitbox[0], _pos.y - _hitbox[1], _hitbox[2], _hitbox[3]);

	spriteBatch.render(posRect, uvRect, texture.id, 0.0f, color);
}

bool Projectile::update() {
	_pos += _motionVec * _speed;
	_lifetime--;
	if (_lifetime == 0) {
		return false;
	}
	return true;
}