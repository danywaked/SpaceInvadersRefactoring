#pragma once
#include "raylib.h"

class Player
{
	float speed = 7;
	int direction = 0;
	float timer = 0;

	void Movement() noexcept;
public:
	int activeTexture = 0;
	float x_pos = static_cast<float>(GetScreenWidth() / 2.0f);
	float player_base_height = 100;
	float radius = 50;
	int lives = 3;
	void Render(Texture2D texture) const noexcept;
	void Update() noexcept;

};