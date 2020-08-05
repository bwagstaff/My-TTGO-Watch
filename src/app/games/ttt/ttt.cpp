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

#include "config.h"
#include <Arduino.h>

#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/statusbar.h"
#include "hardware/display.h"
#include "hardware/motor.h"

#include "ttt_tile.h"
#include "ttt.h"

/* These would be so much better if LVGL supported a data param. Oh well, one for each.*/

static TTT *gameInstance = 0;
static void OnSquareUL(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(0);
        break;
    }
}
static void OnSquareUC(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(1);
        break;
    }
}
static void OnSquareUR(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(2);
        break;
    }
}
static void OnSquareCL(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(3);
        break;
    }
}
static void OnSquareCC(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(4);
        break;
    }
}
static void OnSquareCR(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(5);
        break;
    }
}
static void OnSquareBL(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(6);
        break;
    }
}
static void OnSquareBC(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(7);
        break;
    }
}
static void OnSquareBR(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->TileClicked(8);
        break;
    }
}

TTT::TTT(TicTacToeTile *parent)
{
    gameInstance = this;
    mParent = parent;

    lv_style_init(&mStyleRed);
    lv_style_set_bg_opa(&mStyleRed, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&mStyleRed, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_style_set_bg_grad_color(&mStyleRed, LV_STATE_DEFAULT, LV_COLOR_MAROON);
    lv_style_set_bg_grad_dir(&mStyleRed, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_color(&mStyleRed, LV_BTN_STATE_DISABLED, LV_COLOR_RED);
    lv_style_set_bg_grad_color(&mStyleRed, LV_BTN_STATE_DISABLED, LV_COLOR_MAROON);
    lv_style_set_bg_grad_dir(&mStyleRed, LV_BTN_STATE_DISABLED, LV_GRAD_DIR_VER);

    lv_style_init(&mStyleBlue);
    lv_style_set_bg_opa(&mStyleBlue, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&mStyleBlue, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_style_set_bg_grad_color(&mStyleBlue, LV_STATE_DEFAULT, LV_COLOR_NAVY);
    lv_style_set_bg_grad_dir(&mStyleBlue, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_color(&mStyleBlue, LV_BTN_STATE_DISABLED, LV_COLOR_BLUE);
    lv_style_set_bg_grad_color(&mStyleBlue, LV_BTN_STATE_DISABLED, LV_COLOR_NAVY);
    lv_style_set_bg_grad_dir(&mStyleBlue, LV_BTN_STATE_DISABLED, LV_GRAD_DIR_VER);

    lv_style_init(&mStyleBlank);
    lv_style_set_bg_opa(&mStyleBlank, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&mStyleBlank, LV_STATE_DEFAULT, LV_COLOR_SILVER);
    lv_style_set_bg_grad_color(&mStyleBlank, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_bg_grad_dir(&mStyleBlank, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_color(&mStyleBlank, LV_BTN_STATE_DISABLED, LV_COLOR_SILVER);
    lv_style_set_bg_grad_color(&mStyleBlank, LV_BTN_STATE_DISABLED, LV_COLOR_GRAY);
    lv_style_set_bg_grad_dir(&mStyleBlank, LV_BTN_STATE_DISABLED, LV_GRAD_DIR_VER);

    constexpr int tops[3] = {32, 104, 176};
    constexpr int lefts[3] = {32, 104, 176};
    constexpr int width = 64;
    constexpr int height = 64;
    const lv_event_cb_t funcs[NUM_SQUARES] = {
        OnSquareUL, OnSquareUC, OnSquareUR,
        OnSquareCL, OnSquareCC, OnSquareCR,
        OnSquareBL, OnSquareBC, OnSquareBR};

    for (int i = 0; i < NUM_SQUARES; i++)
    {
        mButtons[i] = lv_btn_create(parent->GetTile(), NULL);
        if(!mButtons[i])
            log_e("Error creating button %d. Crash is immenent.", i);
        lv_obj_set_pos(mButtons[i], tops[i % 3], lefts[i / 3]);
        lv_obj_set_size(mButtons[i], width, height);
        lv_obj_reset_style_list(mButtons[i], LV_BTN_PART_MAIN);
        lv_obj_add_style(mButtons[i], LV_BTN_PART_MAIN, &mStyleBlank);
        lv_obj_set_event_cb(mButtons[i], funcs[i]);
    }

    ClearBoard();
}

TTT::~TTT()
{
    for(auto button : mButtons)
    {
        lv_obj_del(button);
    }

    gameInstance = nullptr;
}

void TTT::TileClicked(int index)
{
    if (index < 0 || index >= NUM_SQUARES)
    {
        log_e("Invaid tile number, expected 0-%d, received %d", NUM_SQUARES, index);
        return;
    }

    if (mBoard[index] == Owner::None)
    {
        lv_style_t *style = (mCurrentPlayer == Owner::Red) ? &mStyleRed : &mStyleBlue;
        
        mBoard[index] = mCurrentPlayer;

        lv_obj_add_style(mButtons[index], LV_BTN_PART_MAIN, style);
        lv_btn_set_state(mButtons[index], LV_BTN_STATE_DISABLED);

        NextPlayer();
    }
    else
    {
        /* tile already owned, do an error response, beep or something. */
    }
}

void TTT::ClearBoard()
{
    for (Owner &c : mBoard)
        c = Owner::None;
}
