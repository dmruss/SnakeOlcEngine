#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>



class Snake : public olc::PixelGameEngine
{
public:
	Snake()
	{
		// Name you application
		sAppName = "Snake";

	}

	//snake
	int counter = 0;
	int size = 1;
	std::vector<int> curX = { ScreenWidth()/2 };
	std::vector<int> curY = { ScreenHeight()/2 };
	std::vector<std::vector<int>> lastTenX = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	std::vector<std::vector<int>> lastTenY = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	
	//food
	int foodX;
	int foodY;
	bool eaten = false;

	//game
	olc::Key lastKey;
	olc::Key nextKey;
	bool nextKeySaved = false;
	int time = 50;
	int score = 0;
	std::string scoreString;
	bool gameOver = false;
	bool pause = false;
	std::string highScoreString = "0";
	int highScoreInt;
	

	void Restart() {
		score = 0;
		time = 50;
		size = 1;
		counter = 0;
		lastKey = olc::Key::A;
		gameOver = false;
		curX = { ScreenWidth() / 2 };
		curY = { ScreenHeight() / 2 };
		lastTenX = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
		lastTenY = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	}

public:
	bool OnUserCreate() override
	{
		if (std::fstream{ "highscore.txt" }) {
			std::fstream fs("highscore.txt");
			getline(fs, highScoreString);
			fs.close();
			std::cout << "found file\n";
		}
		else {
			std::ofstream fs("highscore.txt");
			fs << 0;
			fs.close();
			std::cout << "created file\n";
		}
		
		
		
		
		srand(std::time(NULL));
		//set first food location
		foodX = rand() % (ScreenWidth()-30);
		foodY = rand() % (ScreenHeight()-50);
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (gameOver) {
			DrawRect(curX[0], curY[0], 10, 10, olc::RED);
			DrawString(26, 50, "GAME OVER", olc::WHITE, 2U);
			DrawString(26, 100, "Press Q to quit or", olc::RED, 1U);
			DrawString(25, 110, "ENTER to play again", olc::RED, 1U);
			if (scoreString > highScoreString) {
				std::ofstream fs;
				fs.open("highscore.txt");
				DrawString(30, 150, "New High Score!", olc::WHITE, 1U);
				DrawString(160, 150, scoreString, olc::WHITE, 1U);
				fs << scoreString;
				highScoreString = scoreString;
				fs.close();
			}


			if (GetKey(olc::Key::Q).bPressed == true) {
				return false;
			}
			else if (GetKey(olc::Key::ENTER).bPressed == true) {
				Restart();
			}
		}
		else if (pause) {
			DrawString(48, 50, "PAUSED", olc::WHITE, 2U);
			DrawString(26, 100, "Press Q to quit or", olc::RED, 1U);
			DrawString(25, 110, "SPACE to continue", olc::RED, 1U);
			if (GetKey(olc::Key::Q).bPressed == true) {
				return false;
			}
			else if (GetKey(olc::Key::SPACE).bPressed == true) {
				pause = false;
			}
		}
		else {
			scoreString = std::to_string(score);

			//BACKGROUND and BORDER
			for (int x = 0; x < ScreenWidth(); x++)
				for (int y = 0; y < ScreenHeight(); y++)
					Draw(x, y, olc::Pixel(olc::BLACK));
			DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 20, olc::WHITE);
			DrawString(5, 185, "Score: ", olc::WHITE, 1U);
			DrawString(55, 185, scoreString, olc::WHITE, 1U);
			DrawString(90, 185, "High Score: ", olc::WHITE, 1U);
			DrawString(180, 185, highScoreString, olc::WHITE, 1U);


			//COLLISION DETECTION
			if (curX[0] == foodX + 3 && curY[0] > foodY - 13 && curY[0] < foodY + 3) {
				eaten = true;
			}
			else if (curX[0] + 10 == foodX - 3 && curY[0] > foodY - 13 && curY[0] < foodY + 3) {
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
					gameOver = true;
				}
				else if (curY[0] == curY[i] + 8 && curX[0] > curX[i] && curX[0] < curX[i] + 8) {
					std::cout << "second\n";
					gameOver = true;
				}
				else if (curX[0] + 8 == curX[i] && curY[0] > curY[i] && curY[0] < curY[i] + 8) {
					std::cout << "third\n";
					gameOver = true;
				}
				else if (curY[0] + 8 == curY[i] && curX[0] > curX[i] && curX[0] < curX[i] + 8) {
					std::cout << "fourth\n";
					gameOver = true;
				}
			}

			//FOOD
			if (eaten) {
				score++;
				time -= 5;
				size++;
				lastTenX.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
				lastTenY.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
				curX.push_back(lastTenX[size - 2][counter % 9]);
				curY.push_back(lastTenY[size - 2][counter % 9]);

				foodX = rand() % (ScreenWidth() - 30);
				foodY = rand() % (ScreenHeight() - 50);
				eaten = false;
			}
			DrawCircle(foodX, foodY, 3, olc::RED);


			//USER MOVE
			if (nextKeySaved) {
				if (nextKey == olc::Key::UP) {
					if (curX[0] > curX[2] + 11 || curX[0] < curX[2] - 11) {
						curY[0]--;
						lastKey = olc::Key::UP;
						nextKeySaved = false;
					}
				}
				else if (nextKey == olc::Key::RIGHT) {
					if (curY[0] > curY[2] + 11 || curY[0] < curY[2] - 11) {
						curX[0]++;
						lastKey = olc::Key::RIGHT;
						nextKeySaved = false;
					}
				}
				else if (nextKey == olc::Key::DOWN) {
					if (curX[0] > curX[2] + 11 || curX[0] < curX[2] - 11) {
						curY[0]++;
						lastKey = olc::Key::DOWN;
						nextKeySaved = false;
					}
				}
				else if (nextKey == olc::Key::LEFT) {
					if (curY[0] > curY[2] + 11 || curY[0] < curY[2] - 11) {
						curX[0]--;
						lastKey = olc::Key::LEFT;
						nextKeySaved = false;
					}
				}
			}
			else {
				if (GetKey(olc::Key::SPACE).bPressed == true) {
					pause = true;
				}
				else if (GetKey(olc::Key::UP).bPressed == true) {
					if (lastKey != olc::Key::DOWN) {
						if (size > 2) {
							if (curX[0] > curX[2] + 11 || curX[0] < curX[2] - 11 || curY[0] == curY[2]) {
								curY[0]--;
								lastKey = olc::Key::UP;
							}
							else {
								nextKey = olc::Key::UP;
								nextKeySaved = true;
							}
						}
						else {
							curY[0]--;
							lastKey = olc::Key::UP;
						}

					}
				}
				else if (GetKey(olc::Key::RIGHT).bPressed == true) {
					if (lastKey != olc::Key::LEFT) {
						if (size > 2) {
							if (curY[0] > curY[2] + 11 || curY[0] < curY[2] - 11 || curX[0] == curX[2]) {
								curX[0]++;
								lastKey = olc::Key::RIGHT;
							}
							else {
								nextKey = olc::Key::RIGHT;
								nextKeySaved = true;
							}
						}
						else {
							curX[0]++;
							lastKey = olc::Key::RIGHT;
						}
						
					}
				}
				else if (GetKey(olc::Key::DOWN).bPressed == true) {
					if (lastKey != olc::Key::UP) {
						if (size > 2) {
							if (curX[0] > curX[2] + 11 || curX[0] < curX[2] - 11 || curY[0] == curY[2]) {
								curY[0]++;
								lastKey = olc::Key::DOWN;
							}
							else {
								nextKey = olc::Key::DOWN;
								nextKeySaved = true;
							}
						}
						else {
							curY[0]++;
							lastKey = olc::Key::DOWN;
						}
						
					}
				}
				else if (GetKey(olc::Key::LEFT).bPressed == true) {
					if (lastKey != olc::Key::RIGHT) {
						if (size > 2) {
							if (curY[0] > curY[2] + 11 || curY[0] < curY[2] - 11 || curX[0] == curX[2]) {
								curX[0]--;
								lastKey = olc::Key::LEFT;
							}
							else {
								nextKey = olc::Key::LEFT;
								nextKeySaved = true;
							}
						}
						else {
							curX[0]--;
							lastKey = olc::Key::LEFT;
						}
						
					}
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
			for (int i = 0; i < size - 1; i++) {
				DrawRect(lastTenX[i][counter % 9], lastTenY[i][counter % 9], 10, 10, olc::WHITE);
			}


			//TIME
			if (time > 0) {
				Sleep(time);
			}

			return true;
		}
	}
};

int main()
{
	Snake game;
	if (game.Construct(200, 200, 2, 2))
		game.Start();
	return 0;
}

