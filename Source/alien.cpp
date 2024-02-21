#include "alien.h"

void Alien::Update() noexcept {
	int window_width = GetScreenWidth();

	if (moveRight)
	{
		position.x += speed;

		if (position.x >= GetScreenWidth())
		{
			moveRight = false;
			position.y += 50;
		}
	}
	else
	{
		position.x -= speed;

		if (position.x <= 0)
		{
			moveRight = true;
			position.y += 50;
		}
	}
}

void Alien::Render(Texture2D texture) const noexcept {
	DrawTextureEx(texture, { position.x - 100.0f, position.y - 100.0f }, 0, 0.3f, WHITE);
}