/*   July 31 21:21:07 2020
 *   Copyright  2020  Bryan Wagstaff
 *   Email: programmer@bryanwagstaff.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#pragma once

class TicTacToeTile;

class TTT
{
private:
    static constexpr int NUM_SQUARES = 9;

    TicTacToeTile *mParent = 0;

    enum Owner : uint8_t
    {
        None = 0,
        Red = 'X',
        Blue = 'O',
    };

    // Gameplay data
    TicTacToeTile *parent;
    TTT::Owner mBoard[NUM_SQUARES];
    TTT::Owner mCurrentPlayer = Red;

    // Visual data
    lv_style_t mStyleRed;
    lv_style_t mStyleBlue;
    lv_style_t mStyleBlank;
    lv_obj_t *mButtons[NUM_SQUARES] = {0};

    void NextPlayer()
    {
        if (mCurrentPlayer == Red)
        {
            mCurrentPlayer = Blue;
        }
        else
        {
            mCurrentPlayer = Red;
        }
    }

public:
    TTT(TicTacToeTile *parent);
    ~TTT();

    void TileClicked(int buttonNumber);
    void ClearBoard();
};
