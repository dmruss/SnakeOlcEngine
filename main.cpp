#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <string>


// Override base class with your custom functionality
class Snake : public olc::PixelGameEngine
{
public:
	Snake()
	{
		// Name you application
		sAppName = "Snake";

	}

	//snake
	int x = 120;
	int y = 120;
	int counter = 0;
	int size = 1;
	std::vector<int> movesX = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<int> movesY = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<olc::Key> movesD = { olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP };
	
	std::vector<int> curX = { ScreenWidth()/2 };
	std::vector<int> curY = { ScreenHeight()/2 };
	std::vector<std::vector<int>> lastTenX = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	std::vector<std::vector<int>> lastTenY = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	std::vector<std::vector<olc::Key>> lastTenD = { { olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP } };
	
	
	//food
	int foodX;
	int foodY;

	bool eaten = false;

	olc::Key lastKey;

	int time = 50;

	int score = 0;
	std::string scoreString;

	bool gameOver = false;
	int menuChoice;

	/*void addBodyPiece() {
		body++;
		lastTenX.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
		lastTenY.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
		lastTenD.push_back({ olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP, olc::Key::UP });
	}*/

	bool onGameOver() {
		int playAgain = -1;
		while (playAgain < 0) {
			if (GetKey(olc::Key::ENTER).bPressed == true) {
				playAgain = 1;
			}
			else if (GetKey(olc::Key::Q).bPressed == true) {
				playAgain = 0;
			}
			return playAgain;
		}
	}

public:
	bool OnUserCreate() override
	{
		srand(std::time(NULL));
		//set first food location
		foodX = rand() % (ScreenWidth()-30);
		foodY = rand() % (ScreenHeight()-50);
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (gameOver) {
			menuChoice = onGameOver();
			if (menuChoice == 1) {
				Snake game;
				game.Start();
			}
			else {
				return false;
			}
		}

		scoreString = std::to_string(score);

		//BACKGROUND and BORDER
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(olc::BLACK));
		DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 20, olc::WHITE);		
		DrawString(5, 185, "Score: ", olc::WHITE, 1U);
		DrawString(55, 185, scoreString, olc::WHITE, 1U);

		
		//COLLISION DETECTION
		if (curX[0] == foodX + 3 && curY[0] > foodY-13 && curY[0] < foodY+3) {
			eaten = true;
		}
		else if (curX[0]+10 == foodX - 3 && curY[0] > foodY - 13 && curY[0] < foodY + 3) {
			eaten = true;
		}
		else if (curY[0] == foodY + 3 && curX[0] > foodX - 13 && curX[0] < foodX + 3) {
			eaten = true;
		}
		else if (curY[0] == foodY - 13 && curX[0] > foodX - 13 && curX[0] < foodX + 3) {
			eaten = true;
		}
		for (int i = 2; i < size; i++) {
			if (curX[0] == curX[i] + 8 && curY[0] > curY[i] + 8 && curY[0] < curY[i]) {
				std::cout << "first\n";
				//DrawRect(50, 50, 100, 100, olc::RED);
				DrawString(20, 50, "GAME OVER", olc::WHITE, 2U);
				gameOver = true;
			}
			else if (curY[0] == curY[i] + 8 && curX[0] > curX[i] && curX[0] < curX[i] + 8) {
				std::cout << "second\n";
				//DrawRect(50, 50, 100, 100, olc::RED);
				DrawString(20, 50, "GAME OVER", olc::WHITE, 2U);
				gameOver = true;
			}
			else if (curX[0] + 8 == curX[i] && curY[0] > curY[i] && curY[0] < curY[i] + 8) {
				std::cout << "third\n";
				//DrawRect(50, 50, 100, 100, olc::RED);
				DrawString(20, 50, "GAME OVER", olc::WHITE, 2U);
				gameOver = true;
			}
			else if (curY[0] + 8 == curY[i] && curX[0] > curX[i] && curX[0] < curX[i] + 8) {
				std::cout << "fourth\n";
				//DrawRect(50, 50, 100, 100, olc::RED);
				DrawString(20, 50, "GAME OVER", olc::WHITE, 2U);
				gameOver = true;
			}
		}
		
		std::cout << "here1\n";

		//FOOD
		if (eaten) {
			score++;
			time -= 5;
			size++;
			lastTenX.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			lastTenY.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
			curX.push_back(lastTenX[size - 2][counter % 9]);
			curY.push_back(lastTenY[size - 2][counter % 9]);

			foodX = rand() % (ScreenWidth()-30);
			foodY = rand() % (ScreenHeight()-50);
			eaten = false;
		}
		DrawCircle(foodX, foodY, 3, olc::RED);
		


		//USER MOVE
		if (GetKey(olc::Key::UP).bPressed == true) {
			
			if (y > 0  && lastKey != olc::Key::DOWN) {
				curY[0]--;
				lastKey = olc::Key::UP;
			}
			

		}
		else if (GetKey(olc::Key::RIGHT).bPressed == true) {
			if (x < ScreenWidth() - 10  && lastKey != olc::Key::LEFT) {
				curX[0]++;
				lastKey = olc::Key::RIGHT;
			}
			

		}
		else if (GetKey(olc::Key::DOWN).bPressed == true) {
			if (lastKey != olc::Key::UP) {
				curY[0]++;
				lastKey = olc::Key::DOWN;
			}
			

		}
		else if (GetKey(olc::Key::LEFT).bPressed == true) {
			if (x > 0  && lastKey != olc::Key::RIGHT) {
				curX[0]--;
				lastKey = olc::Key::LEFT;
			}
			

		}

		///AUTO MOVE
		if (lastKey == olc::Key::UP) {
			if (curY[0] > 0) {
				curY[0]--;
			}
			else {
				curY[0] = ScreenHeight() - 30;
			}
		}
		else if (lastKey == olc::Key::RIGHT) {
			if (curX[0] < ScreenWidth() - 10) {
				curX[0]++;
			}
			else {
				curX[0] = 0;
			}
		}
		else if (lastKey == olc::Key::DOWN) {
			if (curY[0] < ScreenHeight() - 30) {
				curY[0]++;
			}
			else {
				curY[0] = 0;
			}
		}
		else if (lastKey == olc::Key::LEFT) {
			if (curX[0] > 0) {
				curX[0]--;
			}
			else {
				curX[0] = ScreenWidth() - 10;
			}
		}

		
		//UPDATE CURRENT LOCATION
		for (int i = 1; i < size; i++) {
			curX[i] = lastTenX[i - 1][counter % 9];
			curY[i] = lastTenY[i - 1][counter % 9];
		}
		//UPDATE MOVE VECTOR
		for (int i = 0; i < size; i++) {
			lastTenX[i][counter] = curX[i];
			lastTenY[i][counter] = curY[i];
		}
				

		if (counter < 9) {
			counter++;
		}
		else {
			counter = 0;
		}

		//DRAW SNAKE
		DrawRect(curX[0], curY[0], 10, 10, olc::WHITE);
		//std::cout << size << std::endl;
		/*for (int i = 1; i < size; i++) {
			DrawRect(movesX[counter%9], movesY[counter%9], 10, 10, olc::WHITE);
		}*/
		for (int i = 0; i < size - 1; i++) {
			DrawRect(lastTenX[i][counter % 9], lastTenY[i][counter % 9], 10, 10, olc::WHITE);
			
		}
		if (time > 0) {
			Sleep(time);
		}
		

		std::cout << "here2\n";
		
		return true;
	}
};

int main()
{
	Snake game;
	if (game.Construct(200, 200, 2, 2))
		game.Start();
	return 0;
}

