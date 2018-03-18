#include <iostream>
#include <sstream>
#include <time.h>	// For srand() initialisation
#include "SDL.h"
#include "Timer.h"
#include "SDL_ttf.h"
using namespace std;

// Global variable declaration
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;
const int numEnemies = 45;
const int numEnemyBullets = 45;
int enemyDir = 32;
int enemiesRemaining = 45;
int playerScore = 0;
bool restartGameFlag = false;
int enemyBulletToFire = -1;

// Set up screen
SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

// Load images from file
SDL_Surface* player = SDL_LoadBMP("resources//Player.bmp");
SDL_Surface* enemy1 = SDL_LoadBMP("resources//Enemy1.bmp");
SDL_Surface* enemy2 = SDL_LoadBMP("resources//Enemy2.bmp");
SDL_Surface* enemy3 = SDL_LoadBMP("resources//Enemy3.bmp");
SDL_Surface* bullet = SDL_LoadBMP("resources//Bullet.bmp");
SDL_Surface* wall = SDL_LoadBMP("resources//Wall.bmp");

TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 20);
SDL_Rect textScoreNumberLocation = {100, 558, 0, 0};
std::string s = std::to_string(playerScore);
char const *pchar = s.c_str();
SDL_Color foregroundColor = {0, 255, 0};
SDL_Color backgroundColor = {0, 0, 0};
SDL_Surface* textScoreNumberSurface = TTF_RenderText_Shaded(font, pchar, foregroundColor, backgroundColor);
SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "Lives:", foregroundColor, backgroundColor);
SDL_Surface* textScoreSurface = TTF_RenderText_Shaded(font, "Score:", foregroundColor, backgroundColor);

//SDL_Surface* score = SDL_LoadBMP("resources//score.bmp");

// Struct for enemy properties
struct gameObject 
{
	SDL_Rect position;
	bool visible;
};

int main(int, char**)
{
	int playerScore = 0;
	string stringScore = "      ";

	// Initialise text
	TTF_Init();

	// Set Text Locations
	SDL_Rect textLivesLocation = { 750, 557, 0, 0 };
	SDL_Rect textScoreLocation = { 40, 557, 0, 0 };
	SDL_Rect lifeOneLocation = { 810, 550, 0, 0 };
	SDL_Rect lifeTwoLocation = { 850, 550, 0, 0 };
	SDL_Rect lifeThreeLocation = { 890, 550, 0, 0 };
	SDL_Rect wallOneLocation = { SCREEN_WIDTH * 0.75, 450, 0, 0 };
	SDL_Rect wallTwoLocation = { SCREEN_WIDTH / 2, 450, 0, 0 };
	SDL_Rect wallThreeLocation = { SCREEN_WIDTH / 4, 450, 0, 0 };

	// Set font
	font = TTF_OpenFont("ARIAL.TTF", 20);

	std::string s = std::to_string(playerScore);

	char const *pchar = s.c_str();

	foregroundColor = { 0, 255, 0 };
	backgroundColor = { 0, 0, 0 };

	textScoreNumberSurface = TTF_RenderText_Shaded(font, pchar, foregroundColor, backgroundColor);
	textSurface = TTF_RenderText_Shaded(font, "Lives:", foregroundColor, backgroundColor);
	textScoreSurface = TTF_RenderText_Shaded(font, "Score:", foregroundColor, backgroundColor);

	int enemyDir = 32;

	// Initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Set name of window
	SDL_WM_SetCaption("Space Invaders", NULL);

	// Prototypes
	void draw(SDL_Rect, gameObject[], gameObject, gameObject[]);
	void setupEnemies(gameObject enemyRect[], gameObject enemyBulletRect[]);
	void updateEnemy(gameObject[], gameObject[]);
	void collision(gameObject[], gameObject&, int&, gameObject[]);

	// Object declaration
	gameObject enemyRect[numEnemies];
	gameObject enemyBulletRect[numEnemyBullets];
	gameObject bulletRect = {};

	// Calls function to create enemies
	setupEnemies(enemyRect, enemyBulletRect);

	// Creates SDL event
	SDL_Event evt;

	// Timer declarations
	Timer enemyTimer;
	Timer playerTimer;
	Timer bulletTimer;
	Timer enemyBulletTimer;
	Timer randomEnemyFire;

	// Local variable declaration
	bool quit = false;
	int dir = 0;
	int pos = (SCREEN_WIDTH / 2);

	// Screen declaration
	SDL_Rect fill{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// Player creation
	SDL_Rect playerRect = { pos, SCREEN_HEIGHT - 96, 32, 32 };

	// Starts timers
	playerTimer.Start();
	bulletTimer.Start();
	enemyTimer.Start();
	enemyBulletTimer.Start();
	randomEnemyFire.Start();

	// Game While loop
	while (!quit)
	{
		// Screen creation
		SDL_FillRect(screen, &fill, 0x000000);

		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
				quit = true;
			// If a key has been pressed do this
			else if (evt.type == SDL_KEYDOWN)
			{
				switch (evt.key.keysym.sym)
				{
					// 'A' key press
				case SDLK_a:
					dir = -1;
					break;
					// 'D' key press
				case SDLK_d:		
					dir = +1;
					break;
					// Space bar key press
				case SDLK_SPACE:		
					if (!bulletRect.visible)
					{
						bulletRect.visible = true;
						bulletRect.position.x = pos;
						bulletRect.position.y = SCREEN_HEIGHT - 82;
					}
					break;
				}
			}
			else if (evt.type == SDL_KEYUP) // When key is released set 0 direction
				dir = 0;
		}

		SDL_BlitSurface(textSurface, NULL, screen, &textLivesLocation);
		SDL_BlitSurface(textScoreNumberSurface, NULL, screen, &textScoreNumberLocation);
		SDL_BlitSurface(textScoreSurface, NULL, screen, &textScoreLocation);
		SDL_BlitSurface(player, NULL, screen, &lifeOneLocation);
		SDL_BlitSurface(player, NULL, screen, &lifeTwoLocation);
		SDL_BlitSurface(player, NULL, screen, &lifeThreeLocation);
		SDL_BlitSurface(wall, NULL, screen, &wallOneLocation);
		SDL_BlitSurface(wall, NULL, screen, &wallTwoLocation);
		SDL_BlitSurface(wall, NULL, screen, &wallThreeLocation);

		// Player boundary check
		if (pos + dir > 0 && pos + dir + 32 < SCREEN_WIDTH && playerTimer.GetElapsedTimeAsSeconds() >= 0.005f) {
			pos += dir;
			playerRect.x = pos;
			playerTimer.Restart();
		}

		// Bullet boundary check
		if (bulletRect.visible == true && bulletTimer.GetElapsedTimeAsSeconds() >= 0.01f) {
			bulletTimer.Restart();
			bulletRect.position.y -= 6;
			//if (bulletRect.visible)
				collision(enemyRect, bulletRect, playerScore, enemyBulletRect);
		}

		// Enemy speed
		if (enemyTimer.GetElapsedTimeAsSeconds() >= ((float)(enemiesRemaining + 3) / numEnemies)) {
			enemyTimer.Restart();
			updateEnemy(enemyRect, enemyBulletRect);
		}

		// Enemy bullet timer - Randomly spawns an Enemy Bullet every so often
		if (enemyBulletTimer.GetElapsedTimeAsSeconds() >= 0.02f)
		{
			enemyBulletTimer.Restart();
			srand(time(NULL));

			// Build a seperate Timer here which controls time between random enemy fires
			if (randomEnemyFire.GetElapsedTimeAsSeconds() >= 2.5f)
			{
				randomEnemyFire.Restart();
				enemyBulletToFire = rand() % 45;
			}

			for (int i = 0; i < numEnemyBullets; i++)
			{
				// If Enemy Bullet is the one selected at Random to fire
				if (i == enemyBulletToFire)
				{
					enemyBulletRect[i].visible = true;
					enemyBulletRect[i].position.y += 6;
					collision(enemyRect, bulletRect, playerScore, enemyBulletRect);
					break;
				}
			}
		}

		// If bullet has reached the edge of the screen set it to not be visible
		if (bulletRect.position.y <= 0) {
			bulletRect.visible = false;
		}

		// If bullet has reached the edge of the screen set it to not be visible
		for (int i = 0; i < numEnemyBullets; i++)
			if (enemyBulletRect[i].position.y >= SCREEN_HEIGHT)
				enemyBulletRect[i].visible = false;

		// Calls draw function to create player, enemies and bullet (if necessary)
		draw(playerRect, enemyRect, bulletRect, enemyBulletRect);//, wallRect, wall2Rect, wall3Rect);

		//NOT SURE YET
		SDL_Flip(screen);

		// If all enemies have been killed end game
		//if (enemiesRemaining == 0)
			//quit = true;
	}

	//SDL_FreeSurface(textSurface);

	TTF_CloseFont(font);
	TTF_Quit();
	
	// Close program
	return 0;
}

void createScoreText(string& scoreText)
{
	scoreText = to_string(playerScore);
}

// Enemy set up function
void setupEnemies(gameObject enemyRect[], gameObject enemyBulletRect[])
{
	for (int y = 0; y < 5; y++) 
	{
		for (int x = 0; x < 9; x++)
		{
			const int pos = x + y * 9;
			enemyRect[pos].position.x = x * 36 + 400;
			enemyRect[pos].position.y = y * 36 + 48;
			enemyBulletRect[pos].position.x = enemyRect[pos].position.x;
			enemyBulletRect[pos].position.y = enemyBulletRect[pos].position.y;
			enemyBulletRect[pos].visible = false;
			//enemyRect[pos].visible = true;
		}
	}
}

// Draw function
void draw(SDL_Rect playerRect, gameObject enemyRect[], gameObject bulletRect, gameObject enemyBulletRect[])
{
	void drawEnemies(gameObject enemyRect[]);
	void drawEnemyBullets(gameObject enemyBulletRect[]);
	
	// If Player bullet is firing
	if (bulletRect.visible)
		// Draw Player Bullet
		SDL_BlitSurface(bullet, NULL, screen, &bulletRect.position);

	// Draw player
	SDL_BlitSurface(player, NULL, screen, &playerRect);

	// Draw Score Numbers
	SDL_BlitSurface(textScoreNumberSurface, NULL, screen, &textScoreNumberLocation);

	// Draw Enemies
	drawEnemies(enemyRect);

	// If Enemy bullet is firing
	drawEnemyBullets(enemyBulletRect);
}

// Draw enemies function
void drawEnemies(gameObject enemyRect[])
{
	int i = 0;

	for (; i < 9; ++i) 
	{
		if (enemyRect[i].visible)
			SDL_BlitSurface(enemy3, NULL, screen, &enemyRect[i].position);
	}
	for (; i < 27; ++i)			//i = 9
	{		
		if (enemyRect[i].visible)
			SDL_BlitSurface(enemy2, NULL, screen, &enemyRect[i].position);
	}
	for (; i < numEnemies; ++i) //i = 27
	{		
		if (enemyRect[i].visible)
			SDL_BlitSurface(enemy1, NULL, screen, &enemyRect[i].position);
	}
}

// Draw Enemy Bullets function
void drawEnemyBullets(gameObject enemyBulletRect[])
{
	int i = 0;

	for (; i < 45; ++i) 
	{
		// If Enemy Bullet is firing - draw Enemy Bullet
		if (enemyBulletRect[i].visible)
			SDL_BlitSurface(bullet, NULL, screen, &enemyBulletRect[i].position);
	}
}

// Update enemy function
void updateEnemy(gameObject enemyRect[], gameObject enemyBulletRect[]) 
{
	bool collided = false;
	for (int y = 0; y < 5; y++) 
	{
		for (int x = 0; x < 9; x++)
		{
			const int pos = x + y * 9;
			enemyRect[pos].position.x += enemyDir;
			if (!enemyBulletRect[pos].visible)
				enemyBulletRect[pos].position = enemyRect[pos].position;
		}
	}
	for (int i = 0; i < 45; i++) {
		// If enemy collides with a boundary, inverse its speed and take it down a level
		if ((enemyRect[i].position.x + 64 >= SCREEN_WIDTH || enemyRect[i].position.x - 32 <= 0) && (!collided && enemyRect[i].visible))
		{
			for (int y = 0; y < 5; y++) 
			{
				for (int x = 0; x < 9; x++)
				{
					const int pos = x + y * 9;
					enemyRect[pos].position.y += 32;
					enemyRect[pos].position.x -= enemyDir;
					if (!enemyBulletRect[pos].visible)
						enemyBulletRect[pos].position = enemyRect[pos].position;
				}
			}
			collided = true;
			enemyDir *= -1;
		}
		// If an enemy collides with the walls at the bottom reset the 
		if (enemyRect[i].position.y >= SCREEN_HEIGHT - 244 && !collided && enemyRect[i].visible)
		{
			for (int y = 0; y < 5; y++) 
			{
				for (int x = 0; x < 9; x++)
				{
					const int pos = x + y * 9;
					enemyRect[pos].visible = true;
					if (!enemyBulletRect[pos].visible)
						enemyBulletRect[pos].position = enemyRect[pos].position;
				}
			}
			enemiesRemaining = 45;
			restartGameFlag = true;
			setupEnemies(enemyRect, enemyBulletRect);
			collided = true;
		}
	}
}

// Bullet collision function
void collision(gameObject enemy[], gameObject& bullet, int& playerScore, gameObject enemyBulletRect[]) 
{
	bool found = false;
	int i = 0;

	while (i < 45 && !found) 
	{
		if ((enemy[i].position.x <= bullet.position.x + 16 && enemy[i].position.x + 32 >= bullet.position.x + 16) && (enemy[i].position.y <= bullet.position.y && enemy[i].position.y + 32 >= bullet.position.y) && enemy[i].visible)
		{
			found = true;
			enemy[i].visible = false;
			bullet.visible = false;
			--enemiesRemaining;
			playerScore += 1000;
			font = TTF_OpenFont("ARIAL.TTF", 20);

			std::string s = std::to_string(playerScore);
			char const *pchar = s.c_str();
			foregroundColor = { 0, 255, 0 };
			backgroundColor = { 0, 0, 0 };
			textScoreNumberSurface = TTF_RenderText_Shaded(font, pchar, foregroundColor, backgroundColor);
		}
		++i;
	}
}