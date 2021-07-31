#ifndef _GAMETETRIS_
#define _GAMETETRIS_
#include <SDL.h>
#include <string>
#include <iostream>

#include "window_surface.h"
#include "sprite.h"
#include "tetrimino.h"
#include "board.h"
#include "virtualPlayer.h"

class Game
{
private:
	/*** 
	 * win : the window of the game
	 * planche : sprite for the blue background
	 * piece : the current piece mooving down
	 * Player_board and IA_board : the boards of the player and the AI
	 * direction : the key the player is pressing (RIGHT, LEFT,...)
	 * time_descente_player : a timer to move the piece down at a regular pace
	 * isPause, menuMode, IAMode : bools to know in which state/menu we are
	 * music : the background music
	 * IAPlayer : the AI class
	 * ***/
	WindowSurface *win;
	Sprite *planche;
	Tetrimino *piece;
	Board * Player_board;
	Board * IA_board;
	MOV_DIRECTION direction;
	SDL_TimerID timer_descente_Player;

	static bool isPaused;
	static bool menuMode;
	static bool IAMode;

	Mix_Music *music; 
	virtualPlayer* IAPlayer;


public:
	inline Game(){}

	/***
	 * Create the windows, blue background, board(s)
	 * and launch the music.
	 * ***/
	void init();

	/***
	 * collect all the key pressed by the player
	 * ***/
	bool keyboard(const Uint8 key);

	/***
	 * Launch the whole process of the game,
	 * deals with timer, FPS, menuMode, detection, etc..
	 * ***/
	void loop();

	/***
	 * Indicate when the game is done 
	 * (when a piece is printed outside of the board)
	 * ***/
	bool IsGameOver(Board* board_);

	/***
	 * Update the state of the game (get the keys from the player, moving the pieces,
	 * looking for the game over, etc...)
	 * ***/
	bool update();
	
	/***
	 * Reset the direction to "NO_MOVE"
	 * ***/
	void reset_key();

	/***
	 * Check the keys the player used, and take action in function of he did
	 * ***/
	bool check_event(SDL_Event event);

	/***
	 * Fonction called to move the piece at regular pace whatever the player do
	 * (it moves the piece down)
	 * ***/
	static Uint32 update_timer_descente_callback(Uint32 intervalle, void *parametre);
};
#endif