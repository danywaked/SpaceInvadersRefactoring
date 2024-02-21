#include "wall.h"


void Wall::Render(Texture2D texture) const noexcept {
	DrawTextureEx(texture, {position.x - 100.0f, position.y	- 100.0f}, 0, 0.3f, WHITE);

	DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}

void Wall::Update() noexcept {
	if (health < 1)
	{
		active = false;
	}
}