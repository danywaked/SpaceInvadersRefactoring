#include "projectile.h"


void Projectile::Update() noexcept{
	position.y -= speed;

	// UPDATE LINE POSITION
	lineStart.y = position.y - 15;
	lineEnd.y = position.y + 15;
	lineStart.x = position.x;
	lineEnd.x = position.x;

	if (position.y < 0 || position.y > GetScreenWidth())
	{
		active = false;
	}
}

void Projectile::Render(Resource& texture) const noexcept{
	DrawTextureEx(texture.texture, position, 0, 0.3f, WHITE);
}