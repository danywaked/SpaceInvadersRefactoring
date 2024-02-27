#include "game.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include "player.h"


// MATH FUNCTIONS
float lineLength(Vector2 A, Vector2 B)noexcept //Uses pythagoras to calculate the length of a line
{
	const float length = sqrtf(pow(B.x - A.x, 2.0f) + pow(B.y - A.y, 2.0f));

	return length;
}

bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) noexcept // Uses pythagoras to calculate if a point is within a circle or not
{
	const float distanceToCentre = lineLength(circlePos, point);

	if (distanceToCentre < radius)
	{
		return true;
	}
	return false;
}

void Game::Start()
{
	// creating walls 
	const float window_width = static_cast<float>(GetScreenWidth()); 
	const float window_height = static_cast<float>(GetScreenHeight());
	const float wall_distance = static_cast<float>(window_width / (wallCount + 1));
	for (int i = 0; i < wallCount; i++)
	{
		Wall newWalls;
		newWalls.position.y = window_height - 250; 
		newWalls.position.x = wall_distance * (i + 1); 

		Walls.push_back(newWalls); 
	}
	//creating player
	Player newPlayer;
	player = newPlayer;

	SpawnAliens();
	
	Background newBackground;
	newBackground.Initialize(600);
	background = newBackground;

	score = 0;
	gameState = State::GAMEPLAY;
}

void Game::End()
{
	//SAVE SCORE AND UPDATE SCOREBOARD
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	newHighScore = CheckNewHighScore();
	gameState = State::ENDSCREEN;
}

void Game::Continue()noexcept
{
	gameState = State::STARTSCREEN;
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		if (IsKeyReleased(KEY_SPACE))
		{
			Start();
		}

		break;
	case State::GAMEPLAY:
		if (IsKeyReleased(KEY_Q))
		{
			End();
		}
		player.Update();
		
		//Update Aliens and Check if they are past player
		for (int i = 0; i < Aliens.size(); i++)
		{
			Aliens[i].Update(); 

			if (Aliens[i].position.y > GetScreenHeight() - player.player_base_height)
			{
				End();
			}
		}
		//End game if player dies
		if (player.lives < 1)
		{
			End();
		}
		if (Aliens.size() < 1)
		{
			SpawnAliens();
		}

		// Update background with offset
		playerPos = { player.x_pos, player.player_base_height };
		cornerPos = { 0.0f, player.player_base_height };
		offset = lineLength(playerPos, cornerPos) * -1;
		background.Update(offset / 15.0f);

		//UPDATE PROJECTILE
		for (auto& p : Projectiles) {
			p.Update();
		}
		//UPDATE WALLS
		for (auto& w : Walls) {
			w.Update();
		}

		//CHECK ALL COLLISONS HERE
		for (auto& p : Projectiles) {
			if (!p.enemyBullet)
			{
				for (auto& a : Aliens) {
					if (CheckCollision(a.position, a.radius, p.lineStart, p.lineEnd))
					{
						// Set them as inactive, will be killed later
						p.active = false;
						a.active = false;
						score += 100;
					}
				}
			}
			else //ENEMY PROJECTILES HERE
			{
				if (CheckCollision({ player.x_pos, GetScreenHeight() - player.player_base_height }, player.radius, p.lineStart, p.lineEnd))
				{
					p.active = false;
					player.lives -= 1;
				}
			}

			for (auto& w : Walls) {
				if (CheckCollision(w.position, w.radius, p.lineStart, p.lineEnd))
				{
					// Set them as inactive, will be killed later
					p.active = false;
					w.health -= 1;
				}
			}
		}

		//MAKE PROJECTILE
		if (IsKeyPressed(KEY_SPACE))
		{
			SpawnPlayerProjectile();
		}

		//Aliens Shooting
		shootTimer += 1;
		if (shootTimer > 59) //once per second
		{
			AlienShooting();
		}
		EraseInactiveEntities();

		break;
	case State::ENDSCREEN:
	
		//Exit endscreen
		if (IsKeyReleased(KEY_ENTER) && !newHighScore)
		{
			Continue();
		}

		if (newHighScore)
		{
			if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
			else mouseOnText = false;

			if (mouseOnText)
			{
				// Set the window's cursor to the I-Beam
				SetMouseCursor(MOUSE_CURSOR_IBEAM);

				// Get char pressed on the queue
				int key = GetCharPressed();

				// Check if more characters have been pressed on the same frame
				while (key > 0)
				{
					// NOTE: Only allow keys in range [32..125]
					if ((key >= 32) && (key <= 125) && (letterCount < 9))
					{
						name[letterCount] = static_cast<char>(key);
						name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
						letterCount++;
					}

					key = GetCharPressed();  // Check next character in the queue
				}

				//Remove chars 
				if (IsKeyPressed(KEY_BACKSPACE))
				{
					letterCount--;
					if (letterCount < 0) letterCount = 0;
					name[letterCount] = '\0';
				}
			}
			else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			if (mouseOnText)
			{
				framesCounter++;
			}
			else
			{
				framesCounter = 0;
			}

			// If the name is right legth and enter is pressed, exit screen by setting highscore to false and add 
			// name + score to scoreboard
			if (letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER))
			{
				std::string nameEntry(name);

				InsertNewHighScore(nameEntry);

				newHighScore = false;
			}
		}
		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
}

void Game::Render() noexcept
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		//Code
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
		break;

	case State::GAMEPLAY:
		background.Render();
		DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
		DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);

		player.Render(resources.shipTextures[player.activeTexture]);
		for(auto& p : Projectiles){
				p.Render(resources.laserTexture);
		}
		for (auto& w : Walls) {
			w.Render(resources.barrierTexture);
		}
		for (auto& a : Aliens) {
			a.Render(resources.alienTexture);
		}
		break;

	case State::ENDSCREEN:
		if (newHighScore)
		{
			DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
			DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

			DrawRectangleRec(textBox, LIGHTGRAY);
			if (mouseOnText)
			{
				// HOVER CONFIRMIATION
				DrawRectangleLines(static_cast<int>(textBox.x), static_cast<int>(textBox.y), static_cast<int>(textBox.width), static_cast<int>(textBox.height), RED);
			}
			else
			{
				DrawRectangleLines(static_cast<int>(textBox.x), static_cast<int>(textBox.y), static_cast<int>(textBox.width), static_cast<int>(textBox.height), DARKGRAY);
			}

			//Draw the name being typed out
			DrawText(name, static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 8, 40, MAROON);
			//Draw the text explaining how many characters are used
			DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

			if (mouseOnText)
			{
				if (letterCount < 9)
				{
					// Draw blinking underscore char
					if (((framesCounter / 20) % 2) == 0)
					{
						DrawText("_", static_cast<int>(textBox.x) + 8 + MeasureText(name, 40), static_cast<int>(textBox.y) + 12, 40, MAROON);
					}
				}
				else
				{
					DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
				}				
			}
			// Explain how to continue when name is input
			if (letterCount > 0 && letterCount < 9)
			{
				DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
			}
		}
		else 
		{
			// If no highscore or name is entered, show scoreboard and call it a day
			DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
			DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

			for (int i = 0; i < Leaderboard.size(); i++)
			{
				char* tempNameDisplay = Leaderboard[i].name.data();
				DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
				DrawText(TextFormat("%i", Leaderboard[i].score), 350, 140 + (i * 40), 40, YELLOW);
			}
		}
		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
}

void Game::SpawnPlayerProjectile()
{
	const float window_height = static_cast<float>(GetScreenHeight());
	Projectile newProjectile;
	newProjectile.position.x = player.x_pos;
	newProjectile.position.y = window_height - 130;
	newProjectile.enemyBullet = false;
	Projectiles.push_back(newProjectile);
}

void Game::AlienShooting()
{
	int randomAlienIndex = 0;
	if (Aliens.size() > 1)
	{
		randomAlienIndex = rand() % Aliens.size();
	}

	Projectile newProjectile;
	newProjectile.position = Aliens[randomAlienIndex].position;
	newProjectile.position.y += 40;
	newProjectile.speed = -15;
	newProjectile.enemyBullet = true;
	Projectiles.push_back(newProjectile);
	shootTimer = 0;
}

void Game::EraseInactiveEntities()
{
	std::erase_if(Aliens, [](const auto& alien) {return !alien.active; });
	std::erase_if(Walls, [](const auto& wall) {return !wall.active; });
	std::erase_if(Projectiles, [](const auto& projectile) {return !projectile.active; });
}

void Game::SpawnAliens()noexcept
{
	for (int row = 0; row < formationHeight; row++) {
		for (int col = 0; col < formationWidth; col++) {
			Alien newAlien = Alien();
			newAlien.active = true;
			newAlien.position.x = static_cast<float>(formationX + 450.0f + (col * alienSpacing));
			newAlien.position.y = static_cast<float>(formationY + (row * alienSpacing));
			Aliens.push_back(newAlien);
			std::cout << "Find Alien -X:" << newAlien.position.x << std::endl;
			std::cout << "Find Alien -Y:" << newAlien.position.y << std::endl;
		}
	}
}

bool Game::CheckNewHighScore()noexcept
{
	if (score > Leaderboard[4].score)
	{
		return true;
	}
	return false;
}

void Game::InsertNewHighScore(std::string p_name)
{
	PlayerData newData;
	newData.name = p_name;
	newData.score = score;

	for (size_t i = 0; i < Leaderboard.size(); i++)
	{
		if (newData.score > Leaderboard[i].score)
		{
			Leaderboard.insert(Leaderboard.begin() + i, newData);
			Leaderboard.pop_back();
			i = Leaderboard.size();
		}
	}
}

bool Game::CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd)noexcept
{
	// our objective is to calculate the distance between the closest point on the line to the centre of the circle, 
	// and determine if it is shorter than the radius.

	// check if either edge of line is within circle
	if (pointInCircle(circlePos, circleRadius, lineStart) || pointInCircle(circlePos, circleRadius, lineEnd))
	{
		return true;
	}

	// simplify variables
	const Vector2 A = lineStart;
	const Vector2 B = lineEnd;
	const Vector2 C = circlePos;

	// calculate the length of the line
	const float length = lineLength(A, B);
	
	// calculate the dot product
	const float dotP = static_cast<float>((((C.x - A.x) * (B.x - A.x)) + ((C.y - A.y) * (B.y - A.y))) / pow(length, 2));

	// use dot product to find closest point
	const float closestX = A.x + (dotP * (B.x - A.x));
	const float closestY = A.y + (dotP * (B.y - A.y));

	//find out if coordinates are on the line.
	// we do this by comparing the distance of the dot to the edges, with two vectors
	// if the distance of the vectors combined is the same as the length the point is on the line

	//since we are using floating points, we will allow the distance to be slightly innaccurate to create a smoother collision
	const float buffer = 0.1f;

	const float closeToStart = lineLength(A, { closestX, closestY }); //closestX + Y compared to line Start
	const float closeToEnd = lineLength(B, { closestX, closestY });	//closestX + Y compared to line End

	const float closestLength = closeToStart + closeToEnd;

	if (closestLength == length + buffer || closestLength == length - buffer)
	{
		//Point is on the line!
		//Compare length between closest point and circle centre with circle radius

		const float closeToCentre = lineLength(A, { closestX, closestY }); //closestX + Y compared to circle centre

		if (closeToCentre < circleRadius)
		{
			//Line is colliding with circle!
			return true;
		}
		else
		{
			//Line is not colliding
			return false;
		}
	}
	else
	{
		// Point is not on the line, line is not colliding
		return false;
	}
}