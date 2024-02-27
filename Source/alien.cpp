#include "alien.h"

void Alien::Update() noexcept {
	const int window_width = GetScreenWidth();

	if (moveRight)
	{
		position.x += speed;

		if (position.x >= window_width)
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

void Alien::Render(Resource& texture) const noexcept {
	DrawTextureEx(texture.texture, { position.x - 100.0f, position.y - 100.0f }, 0, 0.3f, WHITE);
}