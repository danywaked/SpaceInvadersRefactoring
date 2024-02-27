#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)

#include "raylib.h"
#include "Resources.h"


class Wall
{
public:
	Vector2 position{};
	Rectangle rec{};
	float radius = 60.0f;
	
	bool active;
	int health = 50;

	void Render(const Resource& texture) const noexcept;
	void Update() noexcept;
};