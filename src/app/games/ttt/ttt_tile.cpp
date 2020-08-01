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

#include "config.h"
#include <Arduino.h>

#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/statusbar.h"
#include "hardware/display.h"
#include "hardware/motor.h"

#include "ttt_tile.h"
#include "ttt.h"

// Use these images (extern objects)
LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(move_64px);

TicTacToeTile gameInstance;

void tic_tac_toe_app_setup(){
    gameInstance.SetupTile();
}

static void startGame(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance.OnStartClicked();
        break;
    }
}

static void exitGame(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance.OnExitClicked();
        break;
    }
}

TicTacToeTile::TicTacToeTile()
{
    pAppname = "Tic Tac Toe";
    pMenuIcon = &move_64px;
    pStartFunction = startGame;
}

void TicTacToeTile::OnRegistered()
{
    // Have our app display an exit button.
    lv_obj_t *exit_btn = lv_imgbtn_create(pTile, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &mTileStyle);
    lv_obj_align(exit_btn, pTile, LV_ALIGN_IN_TOP_LEFT, 10, STATUSBAR_HEIGHT + 10);
    lv_obj_set_event_cb(exit_btn, exitGame);
}

void TicTacToeTile::OnStartClicked()
{
    motor_vibe(1);
    mainbar_jump_to_tilenumber(mTileId, LV_ANIM_OFF);
}

void TicTacToeTile::OnExitClicked()
{
    motor_vibe(1);
    mainbar_jump_to_tilenumber(app_tile_get_tile_num(), LV_ANIM_OFF);
}