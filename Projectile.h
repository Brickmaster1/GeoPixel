#pragma once

#include <glm/glm.hpp>
#include <ViXeL/rendering/SpriteBatcher.h>

class Projectile {
public:
	Projectile(glm::vec2 pos, glm::vec2 motionVec, float speed, int lifetime, glm::vec4 hitbox, float rotation = 0.0f);
	~Projectile();

	void render(ViXeL::SpriteBatcher& spriteBatch);

	bool update(); //Returns false if lifetime is depleted

	glm::vec4 getHitbox() { return _hitbox; }
	float getRotation() { return _rotation; }
	float getSpeed() { return _speed; }
	glm::vec2 getPos() { return _pos; }
	glm::vec2 getMotionVec() { return _motionVec; }

	void setHitbox(glm::vec4 hitbox) { _hitbox = hitbox; }
	void setRotation(float rotation) { _rotation = rotation; }
	void setSpeed(float speed) { _speed = speed; }
	void setPos(glm::vec2 pos) { _pos = pos; }
	void setMotionVec(glm::vec2 motionVec) { _motionVec = motionVec; }
private:
	glm::vec2 _pos;
	glm::vec2 _motionVec;
	float _speed;
	int _lifetime;
	
	glm::vec4 _hitbox;
	float _rotation;
};

