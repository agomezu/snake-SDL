#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include <iostream>
#include <string>
#include <strstream>
#include <sstream>
#include <fstream>


#define SCREEN_WIDTH 	350
#define SCREEN_HEIGHT 	350

#define ARRIBA 		1
#define ABAJO 		2
#define IZQUIERDA 	3
#define DERECHA 	4

using namespace std;

SDL_Surface *screen, *background, *portada, *gameover;

int manyTimes;
int manyTurns;

SDL_Event event;

/********************* REDEFINITION **********************/

void addBlock();
void checkCollision();
int doSnake();
int drawSnake();
void eated();
int gameOver();
void getKey();
void init();
SDL_Surface* loadImage(const char* name);
void makeEat();
void modBody();
int showMenu();
int showScore();
void playMusic();
void playEffect(Mix_Chunk* effect);

/******************** ALL SNAKE CLASSES *******************/

class soundEffect {
	public:

	Mix_Chunk* comer;
	Mix_Chunk* choque;
	Mix_Chunk* choice;
	Mix_Music* music;

} sound;

class fonts {
	public:

	TTF_Font* scoreFont;
	SDL_Surface* showScore;
	SDL_Color colorText;

} displayFont;

class player {
	public:

	SDL_Surface* blockSnake[1000];
	SDL_Rect position[1000];


	SDL_Surface* head;
	SDL_Rect headPosition;
	SDL_Rect headSaved[1000000];

	SDL_Surface* eat;
	SDL_Rect eatPosition;

	int scorePoints;
	
} snake;

class keys {
	public:

	int UP;
	int DOWN;
	int LEFT;
	int RIGHT;
	
	int now;
	int before;

} key;

/******************** MAIN DEVELOPEMENT ********************/

int main() {
	
	init();
	
	makeEat();
	showMenu();

	while (true) {
		
		SDL_BlitSurface(background,NULL,screen,NULL);		
		//SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 255, 255, 255));
		SDL_BlitSurface(snake.eat,NULL,screen,&snake.eatPosition);
		
		checkCollision();

		showScore();
		getKey();
		doSnake();
		
		eated();
		
		modBody();
		
		drawSnake();		
		
		manyTurns++;
		
		SDL_Flip(screen);
		SDL_Delay(100);
		
	}
	
	return 0;
}

/******************* SNAKE'S INIT ONES ********************/

void init() {
	
	manyTimes = 0;
	manyTurns = 0;

	SDL_Init(SDL_INIT_EVERYTHING);
	Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
	TTF_Init();

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Snake v1.0", NULL);

	for (int i = 0; i < 1000; i++) {
		snake.blockSnake[i] = SDL_LoadBMP("img/body.bmp");
		snake.position[i].x = 0;
		snake.position[i].y = 0;
	}

	for (int i = 0; i < 1000000; i++) {
		snake.headSaved[i].x = NULL;
		snake.headSaved[i].y = NULL;
	}

	snake.head = SDL_LoadBMP("img/head.bmp");	
	background = SDL_LoadBMP("img/bg.bmp");
	portada = SDL_LoadBMP("img/portada.bmp");
	gameover = SDL_LoadBMP("img/gameover.bmp");
	snake.eat = SDL_LoadBMP("img/eat.bmp");

	snake.headPosition.x = 10;
	snake.headPosition.y = 10;

	snake.scorePoints = 0;
	
	key.UP = false;
	key.DOWN = true;
	key.LEFT = false;
	key.RIGHT = false;

	key.now = ABAJO;

	displayFont.scoreFont = TTF_OpenFont("fonts/Lanehum.ttf",60);

	sound.comer = Mix_LoadWAV("sound/eat.wav");	
	sound.choque = Mix_LoadWAV("sound/choque.wav");
	sound.music = Mix_LoadMUS("sound/music.wav");
	sound.choice = Mix_LoadWAV("sound/choice.wav");

	srand (time(NULL));
}

/***************** LOAD AND DRAW SNAKE FUNCTIONS ****************/

SDL_Surface* loadImage(const char* name) {

	return SDL_LoadBMP(name);

}

int drawSnake() {

	SDL_BlitSurface(snake.head,NULL,screen,&snake.headPosition);

	for (int i = 0; i < manyTimes; i++) {

		SDL_BlitSurface(snake.blockSnake[i], NULL, screen, &snake.headSaved[manyTurns-i-1]);

	}	
}

/****************** SNAKE'S MOVIMENT FUNCTIONS ******************/

void getKey() {
	while(SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}
		if (event.type == SDL_KEYDOWN) {
			
			key.before = key.now;	

			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE: 
					exit(0);
				break;
				
				case SDLK_UP:
					
					key.UP = true;
					key.DOWN = false;
					key.LEFT = false;
					key.RIGHT = false;
					
					key.now = ARRIBA;
				break;
			
				case SDLK_DOWN:
					
					key.DOWN = true;
					key.UP = false;
					key.LEFT = false;
					key.RIGHT = false;
					
					key.now = ABAJO;
				break;

				case SDLK_LEFT:
					
					key.LEFT = true;
					key.UP = false;
					key.DOWN = false;
					key.RIGHT = false;
					
					key.now = IZQUIERDA;
				break;

				case SDLK_RIGHT:
					
					key.RIGHT = true;
					key.UP = false;
					key.DOWN = false;
					key.LEFT = false;
					
					key.now = DERECHA;
				break;	
			}
		}
	}
}

int doSnake() {

	if (key.UP == true) {
		if (key.before != ABAJO) {
			snake.headPosition.y -= 10;			
		} else {
			snake.headPosition.y += 10;
		}
	}

	if (key.DOWN == true) {
		if (key.before != ARRIBA) {
			snake.headPosition.y += 10;
		} else {
			snake.headPosition.y -= 10;
		}
	}

	if (key.LEFT == true) {
		if (key.before != DERECHA) {
			snake.headPosition.x -= 10;
		} else {
			snake.headPosition.x += 10;
		}
	}

	if (key.RIGHT == true) {
		if (key.before != IZQUIERDA) {
			snake.headPosition.x += 10;
		} else {
			snake.headPosition.x -= 10;
		}
	}
}

/******************** SNAKE'S EAT FUNCTIONS ***********************/

void makeEat() {
	
	snake.eatPosition.x = rand() % 34;
	snake.eatPosition.y = rand() % 34;
	snake.eatPosition.x = snake.eatPosition.x * 10; 
	snake.eatPosition.y = snake.eatPosition.y * 10;

	if (snake.eatPosition.x == 0 || snake.eatPosition.y == 0) {
		makeEat();
	}

	SDL_BlitSurface(snake.eat,NULL,screen,&snake.eatPosition);
}

void addBlock() {
	
	if (manyTimes > 0) {
		
		snake.position[manyTimes].x = snake.position[manyTimes-1].x - 10;
		snake.position[manyTimes].y = snake.position[manyTimes-1].y;	    
	
	} else {
		
		snake.position[manyTimes].x = snake.headPosition.x - 10;
		snake.position[manyTimes].y = snake.headPosition.y;	
	}
	
}

void eated() {
	if ((snake.headPosition.x == snake.eatPosition.x) && (snake.headPosition.y == snake.eatPosition.y)) {

		playEffect(sound.comer);

		for (int i = 0; i < 5 ; i++) {
			snake.scorePoints++;	
		}

		for (int i = 0; i < 2 ; i++) {	
			addBlock();	
			manyTimes++;
		}

		makeEat();
	}
}

/****************** SNAKE'S BODY FUNCTIONS *********************/ 

void modBody() {
	
	snake.headSaved[manyTurns].x = snake.headPosition.x;
	snake.headSaved[manyTurns].y = snake.headPosition.y;

}

/*********** MENU DISPLAY AND PUNTIATION FUNCTIONS *************/

int showMenu() {
	int x_mouse, y_mouse;
	playMusic();
	while (true) {

		SDL_BlitSurface(portada,NULL,screen,NULL);

		while(SDL_PollEvent(&event)) {

			if ( event.type == SDL_MOUSEBUTTONDOWN ) {

				if (event.button.button == SDL_BUTTON_LEFT) {

					x_mouse = event.button.x;
					y_mouse = event.button.y;

					if ((x_mouse > 15 && y_mouse > 140) && (x_mouse < 145 && y_mouse < 170)) {
						playEffect(sound.choice);
						return 0;
					}
					if ((x_mouse > 15 && y_mouse > 190) && (x_mouse < 160 && y_mouse < 220)) {
						exit(0);
					}
					if ((x_mouse > 15 && y_mouse > 240) && (x_mouse < 100 && y_mouse < 270)) {
						playEffect(sound.choice);
						exit(0);
					}
				}
			}
		}
		SDL_Flip(screen);
		SDL_Delay(100);
	}
}

int gameOver() {

	int x_mouse, y_mouse;

	while(true) {

		SDL_BlitSurface(gameover,NULL,screen,NULL);

		while (SDL_PollEvent(&event)) {
			
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					x_mouse = event.button.x;
					y_mouse = event.button.y;

					if ((x_mouse > 15) && (y_mouse > 190) && (x_mouse < 140) && (y_mouse < 225)) {
						main();
					}
				}
			}
		}

		SDL_Flip(screen);
		SDL_Delay(100);
	}
}

int showScore() {

	stringstream pointing;
	pointing << snake.scorePoints << " points";

	const string str = pointing.str();
	
	displayFont.colorText = {166,166,166};

	displayFont.showScore = TTF_RenderText_Solid(displayFont.scoreFont, str.c_str(), displayFont.colorText); 

	SDL_Rect posText;

	posText.x = 20;
	posText.y = 270; 

	SDL_BlitSurface(displayFont.showScore, NULL, screen,&posText); 

	return 0;
}

/******************* COLLISION DETECTION *******************/

void checkCollision() {

	if ((snake.headPosition.x == 340) || (snake.headPosition.y == 340)) {
		playEffect(sound.choque);
		gameOver();
	} 
	
	if ((snake.headPosition.x == 0) || (snake.headPosition.y == 0)) {
		playEffect(sound.choque);
		gameOver();
	}

	for (int i = 0; i < manyTimes; i++) {
		if ((snake.headPosition.x == snake.headSaved[manyTurns-i-2].x) && (snake.headPosition.y == snake.headSaved[manyTurns-i-2].y)) {
			playEffect(sound.choque);
			gameOver();
		}
	} 
}

/******************* MUSIC EFFECT FUNCTIONS ***************/

void playMusic() {

	Mix_PlayMusic(sound.music, -1);

}

void playEffect(Mix_Chunk* effect) {

	Mix_PlayChannel(-1,effect,0);

}

/****************** HIGHSCORE FUNCTIONS ***************/

/* void checkExist(const string path) {

	ofstream scoreDat(path,"i");
	
}

void addScore(int score) {

	scoreDat << " " << score;

} */
