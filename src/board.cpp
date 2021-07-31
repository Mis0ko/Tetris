#include "board.h"

Board::Board(int origin_x)
{
	carre_grill = new SDL_Rect();
	carre_grill->h = TETR_SIZE;
	carre_grill->w = TETR_SIZE;

	ORIGIN_X = origin_x;

	screenWithBlock.resize(BOARD_HEIGHT);  
	screenBackground.resize(BOARD_HEIGHT); 
	screenHold.resize(4);
	screenNextPieces.resize(12);
	for (int i = 0; i < 4; i++)
	{
		screenHold[i].resize(4);
		for (int j = 0; j < 4; j++)
		{
			screenHold[i][j] = FREE;
		}
	}
	for (int i = 0; i < 12; i++)
	{
		screenNextPieces[i].resize(4);
		for (int j = 0; j < 4; j++)
		{
			screenNextPieces[i][j] = FREE;
		}
	}
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		screenBackground[i].resize(BOARD_WIDTH);
		screenWithBlock[i].resize(BOARD_WIDTH);
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			screenBackground[i][j] = FREE;
			screenWithBlock[i][j] = FREE;
		}
	}

	currentPiece = GenerateRandomShape();
	currentPiece->y += 2;
	NextPieces[0] = GenerateRandomShape();
	NextPieces[1] = GenerateRandomShape();
	NextPieces[2] = GenerateRandomShape();
	holdPiece = nullptr;
	nbHold = 0;
	IsOut = false;
	direction = NO_MOVE;

	for (int i = 0; i < 8; i++)
	{
		color[i] = new SDL_Color();
		switch (i)
		{
		case 0:
			color[i]->r = 0;
			color[i]->g = 0;
			color[i]->b = 0;
			break;

		case 1:
			color[i]->r = 0;
			color[i]->g = 255;
			color[i]->b = 255;
			break;

		case 2:
			color[i]->r = 255;
			color[i]->g = 255;
			color[i]->b = 0;
			break;

		case 3:
			color[i]->r = 255;
			color[i]->g = 0;
			color[i]->b = 255;
			break;

		case 4:
			color[i]->r = 255;
			color[i]->g = 165;
			color[i]->b = 0;
			break;

		case 5:
			color[i]->r = 0;
			color[i]->g = 0;
			color[i]->b = 255;
			break;

		case 6:
			color[i]->r = 255;
			color[i]->g = 0;
			color[i]->b = 0;
			break;

		case 7:
			color[i]->r = 0;
			color[i]->g = 255;
			color[i]->b = 0;
			break;
		}
	}

	totalScore = 0;
	totalLines = 0;
	Level = 0;
	police = TTF_OpenFont("src/GUNSHIP2.TTF", 20);
	colorPolice = {74, 69, 68};
	positionInfos = new SDL_Rect();
}

void Board::setCurrentPiece(Tetrimino *tetr)
{
	currentPiece = tetr;
}

Tetrimino *Board::getCurrentPiece()
{
	return currentPiece;
}

void Board::update_direction(MOV_DIRECTION direction_)
{
	direction = direction_;
}

void Board::moveCurrentPiece()
{
	switch (direction)
	{
	case LEFT:
		currentPiece->x--;
		break;
	case RIGHT:
		currentPiece->x++;
		break;
	case DOWN:
		currentPiece->y++;
		break;
	case ROT_R:
	case ROT_L:
		TryRotate();
		break;
	case FAR_DOWN:
		GoFarDown();
		currentPiece->set_finished();
		break;
	case NO_MOVE:
		break;
	}
}

void Board::draw_board(SDL_Renderer *rend)
{
	int nb_color = -1;

	for (int i = 0; i < BOARD_HEIGHT; i++) 
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			carre_grill->x = (ORIGIN_X + j) * TETR_SIZE;
			carre_grill->y = (ORIGIN_Y + i) * TETR_SIZE;
			nb_color = screenWithBlock[i][j];
			if (nb_color != 9)
			{
				SDL_SetRenderDrawColor(rend, color[nb_color]->r, color[nb_color]->g, color[nb_color]->b, 255); 
				SDL_RenderFillRect(rend, carre_grill);
				if (nb_color != 0)
				{
					SDL_SetRenderDrawColor(rend, 75, 75, 75, 0);
					SDL_RenderDrawRect(rend, carre_grill); 
				}
			}
			else 
			{
				SDL_SetRenderDrawColor(rend, 0, 0, 0, 255); 
				SDL_RenderFillRect(rend, carre_grill);
				SDL_SetRenderDrawColor(rend, color[currentPiece->get_type() + 1]->r, color[currentPiece->get_type() + 1]->g,
									   color[currentPiece->get_type() + 1]->b, 255);
				SDL_RenderDrawRect(rend, carre_grill);
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			carre_grill->x = (ORIGIN_X - 6 + j) * TETR_SIZE;
			carre_grill->y = (ORIGIN_Y + i) * TETR_SIZE;
			nb_color = screenHold[i][j];
			SDL_SetRenderDrawColor(rend, color[nb_color]->r, color[nb_color]->g, color[nb_color]->b, 255); 
			SDL_RenderFillRect(rend, carre_grill);
			if (nb_color != 0)
			{
				SDL_SetRenderDrawColor(rend, 75, 75, 75, 0);
				SDL_RenderDrawRect(rend, carre_grill); 
			}
		}
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			carre_grill->x = (ORIGIN_X + 12 + j) * TETR_SIZE;
			carre_grill->y = (ORIGIN_Y + i) * TETR_SIZE;
			nb_color = screenNextPieces[i][j];
			SDL_SetRenderDrawColor(rend, color[nb_color]->r, color[nb_color]->g, color[nb_color]->b, 255); 
			SDL_RenderFillRect(rend, carre_grill);
			if (nb_color != 0)
			{
				SDL_SetRenderDrawColor(rend, 75, 75, 75, 0);
				SDL_RenderDrawRect(rend, carre_grill); 
			}
		}
	}
}

void Board::print_piece_to_board()
{
	refresh_screen();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currentPiece->current_tetr[i][j] && currentPiece->y - ORIGIN_Y + i >= 0)
				screenWithBlock[currentPiece->y - ORIGIN_Y + i][currentPiece->x - ORIGIN_X + j] = currentPiece->current_tetr[i][j];
		}
	}
}

void Board::deletePieceFromBackground()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currentPiece->current_tetr[i][j] && currentPiece->y - ORIGIN_Y + i >= 0)
				screenBackground[currentPiece->y - ORIGIN_Y + i][currentPiece->x - ORIGIN_X + j] = 0;
		}
	}
	IsOut = false;
}

void Board::print_piece_to_background()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currentPiece->current_tetr[i][j] && currentPiece->y - ORIGIN_Y + i < 0)
			{
				IsOut = true;
			}
			else if (currentPiece->current_tetr[i][j] && currentPiece->y - ORIGIN_Y + i >= 0)
				screenBackground[currentPiece->y - ORIGIN_Y + i][currentPiece->x - ORIGIN_X + j] = currentPiece->current_tetr[i][j];
		}
	}
}

void Board::print_piece_to_hold()
{
	while (currentPiece->get_num_rot() != 0)
	{
		currentPiece->rotate(ROT_R);
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			screenHold[i][j] = 0;
			if (currentPiece->current_tetr[i][j])
				screenHold[i][j] = currentPiece->current_tetr[i][j];
		}
	}
}

void Board::print_piece_to_next()
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			screenNextPieces[i][j] = 0;
			if (i < 4)
			{
				if (NextPieces[0]->current_tetr[i][j])
				{
					screenNextPieces[i][j] = NextPieces[0]->current_tetr[i][j];
				}
			}
			else if (i < 8)
			{
				if (NextPieces[1]->current_tetr[i - 4][j])
				{
					screenNextPieces[i][j] = NextPieces[1]->current_tetr[i - 4][j];
				}
			}
			else
			{
				if (NextPieces[2]->current_tetr[i - 8][j])
				{
					screenNextPieces[i][j] = NextPieces[2]->current_tetr[i - 8][j];
				}
			}
		}
	}
}

void Board::print_projection()
{
	int new_y = 0;
	int detected = 0;
	while (detected == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (currentPiece->current_tetr[i][j])
				{
					if (LookDown(i + new_y, j))
						detected = 1;
				}
			}
		}
		if (detected == 0)
			new_y++;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currentPiece->current_tetr[i][j] && currentPiece->y + new_y - ORIGIN_Y + i >= 0)
			{
				if (screenWithBlock[currentPiece->y + new_y - ORIGIN_Y + i][currentPiece->x - ORIGIN_X + j] == 0)
					screenWithBlock[currentPiece->y + new_y - ORIGIN_Y + i][currentPiece->x - ORIGIN_X + j] = 9;
			}
		}
	}
}

void Board::update_screenNextPieces()
{
	currentPiece = NextPieces[0];

	for (int i = 0; i < 2; i++)
	{
		int ok = 0;
		for (int j = 0; j < 10; j++)
		{
			if (screenBackground[i][j] != 0)
			{
				ok = 1;
			}
		}
		if (ok == 0)
		{
			currentPiece->y++;
		}
	}
	NextPieces[0] = NextPieces[1];
	NextPieces[1] = NextPieces[2];
	NextPieces[2] = GenerateRandomShape();
	print_piece_to_next();
}

void Board::refresh_screen()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			if (screenBackground[i][j] != screenWithBlock[i][j])
				screenWithBlock[i][j] = screenBackground[i][j];
		}
	}
}

int Board::DetectCollision()
{
	int detected = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currentPiece->current_tetr[i][j])
			{
				switch (direction)
				{
				case LEFT:
					if (LookLeft(i, j))
						detected = 1;
					break;
				case RIGHT:
					if (LookRight(i, j))
						detected = 1;
					break;
				case DOWN:
					if (LookDown(i, j))
						detected = 2;
					break;
				case NO_MOVE:
					if (LookDown(i, j))
						detected = 2;
					break;
				default:
					break;
				}
			}
		}
	}

	return detected;
}


int Board::LookDown(int idy, int idx)
{
	int val_y = currentPiece->y - ORIGIN_Y + idy + 1;
	if (val_y < 0)
	{
		return 0;
	}
	else if (val_y < 20)
	{
		if (screenBackground[val_y][currentPiece->x - ORIGIN_X + idx] != 0)
		{
			return 1;
		}
		return 0;
	}
	return 1;
}


int Board::LookUp(int idy, int idx)
{
	int val_y = currentPiece->y - ORIGIN_Y + idy - 1;
	if (val_y >= 0)
	{
		if (screenBackground[val_y][currentPiece->x - ORIGIN_X + idx] != 0) 
		{
			return 1;
		}
		return 0;
	}
	return 1;
}

int Board::LookRight(int idy, int idx)
{
	int val_x = currentPiece->x - ORIGIN_X + idx + 1;
	int val_y = currentPiece->y - ORIGIN_Y + idy;

	if (val_x < 10)
	{
		if (val_y < 0)
		{
			return 0;
		}
		if (screenBackground[val_y][val_x] != 0)
		{
			return 1;
		}
		return 0;
	}
	return 1;
}

int Board::LookLeft(int idy, int idx)
{
	int val_x = currentPiece->x - ORIGIN_X + idx - 1;
	int val_y = currentPiece->y - ORIGIN_Y + idy;

	if (val_x > -1)
	{
		if (val_y < 0)
		{
			return 0;
		}
		if (screenBackground[val_y][val_x] != 0)
		{
			return 1;
		}
		return 0;
	}
	return 1;
}

void Board::TryRotate()
{
	int num_rot_ini = currentPiece->get_num_rot();
	currentPiece->rotate(direction);
	int num_rot_next = currentPiece->get_num_rot();
	tetrimino_type type = currentPiece->get_type();
	int ok = 0;
	int num_wall = 0;
	if (type != BARRE)
	{
		num_wall = 1;
	}
	for (int k = 0; k < 8; k++)
	{
		if (currentPiece->get_wallkick()[num_wall][k][0][0] == num_rot_ini && currentPiece->get_wallkick()[num_wall][k][0][1] == num_rot_next)
		{
			for (int l = 1; l < 6; l++)
			{
				ok = 0;
				currentPiece->x += currentPiece->get_wallkick()[num_wall][k][l][0];
				currentPiece->y -= currentPiece->get_wallkick()[num_wall][k][l][1];
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (currentPiece->current_tetr[i][j])
						{
							if (currentPiece->x - ORIGIN_X + j < 0 || currentPiece->x - ORIGIN_X + j > 9 || currentPiece->y - ORIGIN_Y + i > 19) //|| currentPiece->y - ORIGIN_Y + i < 0
							{
								currentPiece->x -= currentPiece->get_wallkick()[num_wall][k][l][0];
								currentPiece->y += currentPiece->get_wallkick()[num_wall][k][l][1]; 
								ok = 1;
							}
							else if (currentPiece->y - ORIGIN_Y + i >= 0)
							{
								if (screenBackground[currentPiece->y - ORIGIN_Y + i][currentPiece->x - ORIGIN_X + j] != 0)
								{
									currentPiece->x -= currentPiece->get_wallkick()[num_wall][k][l][0]; 
									currentPiece->y += currentPiece->get_wallkick()[num_wall][k][l][1];
									ok = 1;
								}
							}
						}
					}
				}
				if (ok == 0)
				{
					return;
				}
			}
		}
	}
	while (currentPiece->get_num_rot() != num_rot_ini)
	{
		currentPiece->rotate(ROT_R);
	}
	return;
}

void Board::GoFarDown()
{
	int detected = 0;
	while (detected == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (currentPiece->current_tetr[i][j])
				{
					if (LookDown(i, j))
					{
						detected = 1;
					}
				}
			}
		}
		if (detected == 0)
			currentPiece->y++;
	}
}

void Board::changePiece()
{
	if (nbHold == 1)
	{
		return;
	}
	nbHold = 1;
	print_piece_to_hold();
	currentPiece->set_coord(ORIGIN_X + 3, ORIGIN_Y - 3);
	Tetrimino *intermPiece = currentPiece;
	if (holdPiece == nullptr)
	{
		holdPiece = currentPiece;
		update_screenNextPieces();
		return;
	}
	currentPiece = holdPiece;
	holdPiece = intermPiece;
	return;
}

void Board::GoFarUp()
{
	int detected = 0;
	int aBouger = 0;
	int val_y;
	while (detected == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (currentPiece->current_tetr[i][j])
				{
					if (LookUp(i, j))
						detected = 1;
				}
			}
		}
		if (detected == 0)
		{
			currentPiece->y--;
			aBouger = 1;
		}
	}
}


int Board::LineFull()
{
	int full;
	int nbLines = 0;

	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		full = 1;
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			if (screenBackground[i][j] == 0)
			{
				full = 0;
			}
		}
		if (full == 1)
		{
			nbLines++;
			BringDownColumns(i);
		}
	}
	totalLines += nbLines;
	return nbLines;
}



void Board::BringDownColumns(int i_row)
{
	int i = i_row;
	for (int j = 0; j < BOARD_WIDTH; j++)
	{
		while (i > 0)
		{
			screenBackground[i][j] = screenBackground[i - 1][j];
			i--;
		}
		screenBackground[0][j] = 0;
		i = i_row;
	}
}


int Board::nbLineFull()
{
	int full;
	int nbLines = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		full = 1;
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			if (screenBackground[i][j] == 0)
				full = 0;
		}
		if (full == 1)
			nbLines++;
	}
	return nbLines;
}

void Board::UpdateLevel()
{
	if (Level < 29)
	{
		Level = totalLines / 10;
	}
	return;
}

int Board::get_level()
{
	return Level;
}

void Board::set_nbHold(int new_nbHold)
{
	nbHold = new_nbHold;
	return;
}

int Board::get_nbHold()
{
	return nbHold;
}

void Board::set_IsOut(bool new_IsOut)
{
	IsOut = new_IsOut;
	return;
}

bool Board::get_IsOut()
{
	return IsOut;
}

tetrimino_type Board::GetRandomShape()
{
	int Shape = rand() % 7;
	switch (Shape)
	{
	case 0:
		return BARRE;
	case 1:
		return BLOC;
	case 2:
		return T_TYPE;
	case 3:
		return L_TYPE;
	case 4:
		return J_TYPE;
	case 5:
		return Z_TYPE;
	case 6:
		return S_TYPE;
	}
	return BARRE;
}

Tetrimino *Board::GenerateRandomShape()
{
	Tetrimino *randomTetrimino;
	tetrimino_type randomShape = GetRandomShape();
	if (randomShape == BARRE)
		randomTetrimino = new Tetrimino(ORIGIN_X + 3, ORIGIN_Y - 3, randomShape); 
	else if (randomShape == BLOC)
		randomTetrimino = new Tetrimino(ORIGIN_X + 3, ORIGIN_Y - 3, randomShape);
	else
		randomTetrimino = new Tetrimino(ORIGIN_X + 3, ORIGIN_Y - 3, randomShape);
	return randomTetrimino;
}



int Board::computeScore(int nbLines)
{
	int valYPiece = currentPiece->y - ORIGIN_Y + 1;
	int score = valYPiece * 2;
	switch (nbLines)
	{
	case 1:
		score += (40 * Level + 1);
		break;
	case 2:
		score += (100 * Level + 1);
		break;
	case 3:
		score += (300 * Level + 1);
		break;
	case 4:
		score += (1200 * Level + 1);
		break;
	default:
		break;
	}
	return score;
}

void Board::setScore(int valueToAdd)
{
	this->totalScore += valueToAdd;
}

int Board::get_score()
{
	return totalScore;
}

void Board::setPositionInfos(optionInfo infos)
{
	positionInfos->w = (BOARD_WIDTH * TETR_SIZE) / 2;
	positionInfos->h = 3 * TETR_SIZE;
	positionInfos->x = ((ORIGIN_X - 7) * TETR_SIZE);
	switch (infos)
	{
	case SCORE:
		positionInfos->y = (ORIGIN_Y + BOARD_HEIGHT - 10) * TETR_SIZE;
		break;
	case LEVEL:
		positionInfos->y = (ORIGIN_Y + BOARD_HEIGHT - 6.5) * TETR_SIZE;
		break;
	case LINES:
		positionInfos->y = (ORIGIN_Y + BOARD_HEIGHT - 3) * TETR_SIZE;
		break;
	case HOLD:
		positionInfos->y = (ORIGIN_Y + BOARD_HEIGHT - 23.5) * TETR_SIZE;
		positionInfos->x = (ORIGIN_X - 6.5) * TETR_SIZE;
		break;
	case NEXT:
		positionInfos->y = (ORIGIN_Y + BOARD_HEIGHT - 23.5) * TETR_SIZE;
		positionInfos->x = (ORIGIN_X + BOARD_WIDTH + 1.5) * TETR_SIZE;
		break;
	}
}


void Board::textInfos(optionInfo infos)
{
	switch (infos)
	{
	case SCORE:
		snprintf(infosMsg, 100, "Score : %i", totalScore);
		break;
	case LEVEL:
		snprintf(infosMsg, 100, "Level : %i", Level);
		break;
	case LINES:
		snprintf(infosMsg, 100, "Lines : %i", totalLines);
		break;
	case HOLD:
		snprintf(infosMsg, 100, "Hold");
		break;
	case NEXT:
		snprintf(infosMsg, 100, "Next");
		break;
	}
	infosMsg[strlen(infosMsg)] = '\0';
}

void Board::printInfosToScreen(SDL_Renderer *rend)
{
	for (optionInfo infos = SCORE; infos < 5; infos = optionInfo(int(infos) + 1))
	{
		textInfos(infos);
		textSurface = TTF_RenderText_Solid(police, infosMsg, colorPolice);
		setPositionInfos(infos);
		textTexture = SDL_CreateTextureFromSurface(rend, textSurface);

		SDL_SetRenderDrawColor(rend, 213, 213, 213, 255);
		SDL_RenderFillRect(rend, positionInfos);
		SDL_RenderDrawRect(rend, positionInfos);

		SDL_RenderCopy(rend, textTexture, NULL, positionInfos);
	}
}

void Board::freeScoreText()
{
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}