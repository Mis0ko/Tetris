#include "virtualPlayer.h"
#include "board.h"

virtualPlayer::virtualPlayer(Board *board) : b(board)
{
    pos = (posTetr*)malloc(sizeof(posTetr));
    int best_pos_tetr_x = 0;
    int best_plusbas = -1;
    int best_numrot = 0;
    int nbTrouMin = 19;
    int best_score = 0;
    bool IsFirst = true;
    bool IsHold = false;
};

void virtualPlayer::sliceFarLeft()
{
    b->update_direction(LEFT);
    while (!b->DetectCollision())
    {
        b->moveCurrentPiece();
    }
    return;
}

int virtualPlayer::NbTrou()
{
    int nbtrou = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (b->currentPiece->current_tetr[i][j])
            {
                int ind_i = i;
                while (b->LookDown(ind_i, j) == 0)
                {
                    nbtrou += 1;
                    ind_i++;
                }
            }
        }
    }
    return nbtrou;
}

int virtualPlayer::PlusBas()
{
    int plusbas = -1;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (b->currentPiece->current_tetr[i][j] != 0 && b->currentPiece->y - ORIGIN_Y + i > plusbas)
            {
                plusbas = b->currentPiece->y - ORIGIN_Y + i;
            }
        }
    }
    return plusbas;
}

void virtualPlayer::checkBestPosition(bool IsPieceHold)
{
    int nbTrou = 0;
    int plusbas = 0;
    int score = 0;
    int ind_x = 0;

    sliceFarLeft();

    b->GoFarDown();
    b->print_piece_to_background();

    nbTrou = NbTrou();
    plusbas = PlusBas();
    
    score = b->totalScore + b->computeScore(b->nbLineFull());
    if (score > best_score)
    {
        best_score = score;
        nbTrouMin = nbTrou;
        best_numrot = b->currentPiece->get_num_rot();
        best_pos_tetr_x = ind_x;
        best_plusbas = plusbas;
        IsHold = IsPieceHold;
    }

    else if (score == best_score && plusbas > best_plusbas)
    {
        nbTrouMin = nbTrou;
        best_numrot = b->currentPiece->get_num_rot();
        best_pos_tetr_x = ind_x;
        best_plusbas = plusbas;
        IsHold = IsPieceHold;
    }

    else if (score == best_score && plusbas == best_plusbas && nbTrou < nbTrouMin)
    {
        nbTrouMin = nbTrou;
        best_numrot = b->currentPiece->get_num_rot();
        best_pos_tetr_x = ind_x;
        IsHold = IsPieceHold;
    }

    b->deletePieceFromBackground();
    b->currentPiece->set_coord(b->currentPiece->x, 3);

    b->update_direction(RIGHT);

    while (!b->DetectCollision()) 
    {
        b->moveCurrentPiece();
        ind_x++;
        b->GoFarDown();
        b->print_piece_to_background();

        nbTrou = NbTrou();
        plusbas = PlusBas();

        score = b->totalScore + b->computeScore(b->nbLineFull());
        if (score > best_score)
        {
            best_score = score;
            nbTrouMin = nbTrou;
            best_numrot = b->currentPiece->get_num_rot();
            best_pos_tetr_x = ind_x;
            best_plusbas = plusbas;
            IsHold = IsPieceHold;
        }

        else if (score == best_score && nbTrou < nbTrouMin)
        {
            nbTrouMin = nbTrou;
            best_numrot = b->currentPiece->get_num_rot();
            best_pos_tetr_x = ind_x;
            best_plusbas = plusbas;
            IsHold = IsPieceHold;
        }

        else if (score == best_score && nbTrou == nbTrouMin && plusbas > best_plusbas)
        {
            best_numrot = b->currentPiece->get_num_rot();
            best_pos_tetr_x = ind_x;
            best_plusbas = plusbas;
            IsHold = IsPieceHold;
        }

        b->deletePieceFromBackground();
        b->currentPiece->set_coord(b->currentPiece->x, 3);
        b->update_direction(RIGHT);
    }
    return;
}

void virtualPlayer::chkAllCombinaison()
{
    bool IsPieceHold = false;

    if (IsFirst == true)
    {
        b->changePiece();
        IsFirst == false;
        return;
    }
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            checkBestPosition(IsPieceHold);
            b->update_direction(ROT_R);
            b->moveCurrentPiece();
        }
        b->changePiece();    
        IsPieceHold = !IsPieceHold;
    }

    if (IsHold == false)
    {
        b->set_nbHold(0);
        b->changePiece();
    }

    IsHold = false;

    nbTrouMin = 19;
    best_plusbas = -1;

    while (b->currentPiece->get_num_rot() != best_numrot)
    {
        b->update_direction(ROT_R);
        b->moveCurrentPiece();
    }

    sliceFarLeft();
    b->update_direction(RIGHT);
    for (int j = 0; j < best_pos_tetr_x; j++)
    {
        b->moveCurrentPiece();
    }

    b->GoFarDown();
    b->set_IsOut(false);
    b->currentPiece->set_finished();
    b->update_direction(NO_MOVE);

    return;
}