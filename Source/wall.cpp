#include "wall.h"


void Wall::Render(const Resource& texture) const noexcept {
	DrawTextureEx(texture.texture, {position.x - 100.0f, position.y	- 100.0f}, 0, 0.3f, WHITE);
	DrawText(TextFormat("%i", health), static_cast<int> (position.x) - 21, static_cast<int>(position.y) + 10, 40, RED);
}

void Wall::Update() noexcept {
	if (health < 1)
	{
		active = false;
	}
}