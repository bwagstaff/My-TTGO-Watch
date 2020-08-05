/*   July 31 21:33:35 2020
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

#include "app\games\gamebase.h"

void tic_tac_toe_app_setup();

class TTT;

class TicTacToeTile : public gamebase
{
    std::unique_ptr<TTT> mGameInstance;
    void OnRegistered() override;

public:
    TicTacToeTile();

    // Respond to callback
    void OnStartClicked();

    // Respond to callback
    void OnExitClicked();
};