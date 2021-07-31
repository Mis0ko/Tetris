// rayanlegris@gmail.com
#include "game.h"

bool Game::isPaused; 
bool Game::menuMode;
bool Game::IAMode;

void Game::init()
{
	win = new WindowSurface();
	planche = new Sprite("src/sprites.bmp");
	Player_board = new Board(10);
	IA_board = new Board(10 + WIN_W/TETR_SIZE);
	piece = Player_board->getCurrentPiece();
	direction = NO_MOVE;
	Player_board->print_piece_to_next();
	isPaused = false;

	music = Mix_LoadMUS("tetrisSong.mp3");
	if (!music)
		printf("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
	Mix_PlayMusic(music, -1);
	menuMode = true;

	IAPlayer = new virtualPlayer(IA_board);
	IAMode = false;
}

void Game::reset_key()
{
	direction = NO_MOVE;
}

bool Game::check_event(SDL_Event event)
{
	bool quit = false;
	int xMouse, yMouse;

	switch (event.type)
	{
	case SDL_QUIT:
		quit = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		SDL_GetMouseState(&xMouse, &yMouse);
		menuInfo infos;
		if (menuMode)
		{
			infos = PLAY;
			if (win->isInsideResumeButtom(xMouse, yMouse, infos))
			{
				IA_board = nullptr;
				menuMode = false;
			}
			infos = IA;
			if (win->isInsideResumeButtom(xMouse, yMouse, infos))
			{
				menuMode = false;
				IAMode = true;
				win->ResizeWindow(win->get_w());
				IA_board->print_piece_to_next();
			}
			infos = EXIT;
			if (win->isInsideResumeButtom(xMouse, yMouse, infos))
				quit = true;
		}
		else if (isPaused)
		{
			infos = RESUME;
			if (win->isInsideResumeButtom(xMouse, yMouse, infos))
				isPaused = false;
			infos = QUIT;
			if (win->isInsideResumeButtom(xMouse, yMouse, infos))
			{
				quit = true;
			}
		}
		break;
	case SDL_KEYUP:
		quit = keyboard(event.key.keysym.scancode);
	}
	return quit;
}

bool Game::keyboard(const Uint8 key)
{
	bool quit = false;
	if (isPaused && (key != SDL_SCANCODE_P))
		return false;
	if (key == SDL_SCANCODE_P)
		isPaused = !isPaused;
	if (key == SDL_SCANCODE_ESCAPE)
	{
		return true;
	}

	switch (key)
	{
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_LEFT:
		direction = LEFT;
		break;
	case SDL_SCANCODE_D:
	case SDL_SCANCODE_RIGHT:
		direction = RIGHT;
		break;
	case SDL_SCANCODE_UP:
		direction = ROT_R;
		break;
	case SDL_SCANCODE_W:
		direction = ROT_L;
		break;
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
		direction = DOWN;
		break;
	case SDL_SCANCODE_SPACE:
		direction = FAR_DOWN;
		break;
	case SDL_SCANCODE_C:
		Player_board->changePiece();
		break;
	default:
		break;
	}
	return quit;
}

bool Game::IsGameOver(Board* board_)
{
	if (board_ != nullptr)
	{
		if (board_->get_IsOut() == true)
		{
			return true;
		}
	}
	return false;
}

bool Game::update()
{
	int nbLines;
	Player_board->update_direction(direction);
	if (IsGameOver(Player_board) == true)
	{
		printf("GAME OVER\n");
		return true;
	}

	if (!Player_board->DetectCollision())
		Player_board->moveCurrentPiece();

	if (Player_board->DetectCollision() == 2)
	{
		if (!Player_board->getCurrentPiece()->getStateFinished())
		{
			int now = SDL_GetTicks();
			int aBouge = 1;
			int intervalle = 1000 / (1 + (Player_board->get_level() + 1) / 15);
			while (SDL_GetTicks() - now < 4 * intervalle && aBouge != 0)
			{
				reset_key();
				if (SDL_GetTicks() - now > intervalle)
				{
					aBouge = 0;
				}
				SDL_Event event;
				if (SDL_PollEvent(&event))
				{
					check_event(event);
					Player_board->update_direction(direction);
					if (!Player_board->DetectCollision())
					{
						aBouge = 1;
						Player_board->moveCurrentPiece();
						win->render(planche->get_surf(), Player_board, IA_board, false, false);
						Player_board->print_piece_to_board();
						Player_board->print_projection();
					}
				}
			}
		}
		Player_board->GoFarDown();
		Player_board->print_piece_to_background(); 
		nbLines = Player_board->LineFull();					
		Player_board->UpdateLevel();
		Player_board->setScore(Player_board->computeScore(nbLines));
		Player_board->set_nbHold(0);
		Player_board->update_screenNextPieces();
	}
	if (IA_board != nullptr && IsGameOver(IA_board) == false)
	{
		IAPlayer->chkAllCombinaison();
		if (IA_board->DetectCollision() == 2)
		{
			IA_board->GoFarDown();
			IA_board->print_piece_to_background(); 
			nbLines = IA_board->LineFull();				
			IA_board->UpdateLevel();
			IA_board->setScore(IA_board->computeScore(nbLines));
			IA_board->set_nbHold(0);
			IA_board->update_screenNextPieces();
		}
	}
	return false;
}

Uint32 Game::update_timer_descente_callback(Uint32 intervalle, void *parametre)
{
	Board *myboard = static_cast<Board *>(parametre);
	if (!isPaused && !menuMode)
	{
		myboard->update_direction(DOWN);
		if (!myboard->DetectCollision())
		{
			myboard->moveCurrentPiece();
		}
	}
	return (1000 / (1 + (myboard->get_level() + 1) / 15));
}

void Game::loop()
{
	int prev = 0, now = 0;
	bool quit = false;

	timer_descente_Player = SDL_AddTimer(1000, update_timer_descente_callback, Player_board);

	while (quit == false)
	{
		now = SDL_GetTicks();
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (check_event(event))
				break;
			if (!isPaused)
			{
				update();
			}
				reset_key();
		}
		if (now - prev > 50) 
		{
			if (!isPaused && !menuMode) 
			{
				if (IA_board == nullptr) 
				{
					quit = update();

					win->render(planche->get_surf(), Player_board, IA_board, false, false);
					Player_board->print_piece_to_board();
					Player_board->print_projection();
				}
				else 
				{
					quit = update();
					win->render(planche->get_surf(), Player_board, IA_board, false, false);
					Player_board->print_piece_to_board();
					Player_board->print_projection();
					if (IsGameOver(IA_board) == false)
						IA_board->print_piece_to_board();
				}				
			}
			else if (isPaused && !menuMode)
			{
					win->render(planche->get_surf(), Player_board, IA_board, true, false);
			}
			else if (menuMode)
			{
				win->render(planche->get_surf(), Player_board, IA_board, false, true);
			}
			prev = now;
		}
	}

	SDL_DestroyWindow(win->get_w());
	Mix_FreeMusic(music); 
	Mix_CloseAudio();	  
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) != 0)
	{
		return 1;
	}

	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) 
	{
		printf("%s", Mix_GetError());
	}

	Game g;

	g.init();
	g.loop();
	cout << "Fin du jeu" << endl;
}
