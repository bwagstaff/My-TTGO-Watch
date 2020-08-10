/*   Aug 7 20:23:27 2020
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

class SimonSaysIcon;

class SimonSaysApp : public GameBase
{

private:
    static constexpr int NUM_SQUARES = 4; // 4 squares on the game board
    static constexpr int TONE_TIME = 200; // 200 ms for each button

    SimonSaysIcon *mParentIcon = 0;

    // Gameplay data

    std::vector<int8_t> mPattern; //! Array of notes being played
    size_t mCurrentNote = 0;      //! Current position
    bool mIsDoingReplay = false;  //! Are we replaying the tones to the user?

    lv_task_t *mPlaybackTask = 0;

    // Visual data
    lv_style_t mStyleApp;
    lv_style_t mStyleMenu;
    lv_style_t mStyleButtons[NUM_SQUARES];
    lv_obj_t *mButtons[NUM_SQUARES] = {0};

    /* Clean up assets and close game */
    void OnExitClicked();

    /* Reset patterns, shift to game screen, start it up*/
    void StartNewGame();

    /* Add a note and play the sequence back */
    void AddNoteAndReplay();

public:
    enum MenuItem : uint8_t
    {
        NewGame,
        Exit,
        NumMenuItems
    };

    SimonSaysApp(SimonSaysIcon *callingIcon);
    ~SimonSaysApp();

    // Launch from watch mainbar
    void OnLaunch();

    // TTT square pressed
    void OnTileClicked(int index);

    // A menu item was clicked.
    void OnMenuClicked(MenuItem item);

    // Playback has triggered the next tone to play
    void OnNextTone();
};
