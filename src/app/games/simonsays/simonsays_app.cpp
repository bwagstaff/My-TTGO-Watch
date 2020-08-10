/*   Aug 7 20:23:34 2020
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

// Set logging level for this file
//#define CORE_DEBUG_LEVEL 5

#include "config.h"

#include "simonsays_app.h"
#include "simonsays_icon.h"

//TODO: Placeholder background image
LV_IMG_DECLARE(bg1);

/* These would be unnecessary if LVGL supported a data param... */

static SimonSaysApp *gameInstance = 0;
static void OnSquareUL(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->OnTileClicked(0);
        break;
    }
}
static void OnSquareUR(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->OnTileClicked(1);
        break;
    }
}
static void OnSquareBL(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->OnTileClicked(2);
        break;
    }
}
static void OnSquareBR(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->OnTileClicked(3);
        break;
    }
}

static void OnExit(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->OnMenuClicked(SimonSaysApp::Exit);
        break;
    }
}

static void OnNewGame(struct _lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case (LV_EVENT_CLICKED):
        gameInstance->OnMenuClicked(SimonSaysApp::NewGame);
        break;
    }
}

static void PlaybackTonesTask(lv_task_t *task)
{
    log_d("Playback tones task has triggered again...");
    gameInstance->OnNextTone();
}

SimonSaysApp::SimonSaysApp(SimonSaysIcon *icon)
{
    gameInstance = this;
    mParentIcon = icon;

    //Re-seed system-level RNG seed to an ungrounded pin
    uint16_t seed = analogRead(0);
    randomSeed(seed);
    log_d("Setting random seed %d", seed);

    log_d("Creating game tiles...");
    if (!AllocateAppTiles(2, 1))
    {
        log_e("Could not allocate tile. Aborting.");
        return;
    }
    lv_obj_t *menuTile = GetTile(0);
    lv_obj_t *gameplayTile = GetTile(1);

    log_d("Initializing styles");
    {
        // Create a general application style for all the app's tiles in the view
        lv_style_init(&mStyleApp);
        lv_style_set_radius(&mStyleApp, LV_OBJ_PART_MAIN, 0);
        lv_style_set_bg_color(&mStyleApp, LV_OBJ_PART_MAIN, LV_COLOR_NAVY);
        lv_style_set_bg_opa(&mStyleApp, LV_OBJ_PART_MAIN, LV_OPA_0);
        lv_style_set_border_width(&mStyleApp, LV_OBJ_PART_MAIN, 0);
        lv_style_set_text_color(&mStyleApp, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
        lv_style_set_image_recolor(&mStyleApp, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);

        lv_tileview_set_edge_flash(GetTileView(), false);
        lv_obj_add_style(GetTileView(), LV_OBJ_PART_MAIN, &mStyleApp);
        lv_page_set_scrlbar_mode(GetTileView(), LV_SCRLBAR_MODE_DRAG);

        // Initialize screen background
        log_d("Creating background for game tile");
        lv_obj_t *img_bin = lv_img_create(menuTile, NULL);
        lv_img_set_src(img_bin, &bg1);
        lv_obj_set_width(img_bin, LV_HOR_RES);
        lv_obj_set_height(img_bin, LV_VER_RES);
        lv_obj_align(img_bin, NULL, LV_ALIGN_CENTER, 0, 0);

        log_d("Creating menu style");
        lv_style_init(&mStyleMenu);
        lv_style_copy(&mStyleMenu, &mStyleApp);
        lv_style_set_text_color(&mStyleMenu, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
        lv_style_set_bg_opa(&mStyleMenu, LV_STATE_DEFAULT, LV_OPA_COVER);
        lv_style_set_bg_color(&mStyleMenu, LV_STATE_DEFAULT, LV_COLOR_BLUE);
        lv_style_set_bg_grad_color(&mStyleMenu, LV_STATE_DEFAULT, LV_COLOR_NAVY);
        lv_style_set_bg_grad_dir(&mStyleMenu, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
        lv_obj_add_style(menuTile, LV_LABEL_PART_MAIN, &mStyleMenu);

        log_d("Creating button styles");

        lv_color_t mainColor[NUM_SQUARES] = {LV_COLOR_GREEN, LV_COLOR_RED, LV_COLOR_BLUE, LV_COLOR_ORANGE};
        lv_color_t gradColorDim[NUM_SQUARES] = {LV_COLOR_MAKE(0x20, 0x60, 0x00), LV_COLOR_MAROON, LV_COLOR_NAVY, LV_COLOR_MAKE(0x80, 0x52, 0x00)};
        lv_color_t gradColorLit[NUM_SQUARES] = {LV_COLOR_MAKE(0x40, 0xFF, 0x40), LV_COLOR_MAKE(0xFF, 0x40, 0x40), LV_COLOR_MAKE(0x40, 0x40, 0xFF), LV_COLOR_MAKE(0xFF, 0xC0, 0x50)};

        for (int i = 0; i < NUM_SQUARES; i++)
        {
            lv_style_init(&mStyleButtons[i]);
            lv_style_set_bg_opa(&mStyleButtons[i], LV_STATE_DEFAULT, LV_OPA_COVER);
            lv_style_set_bg_color(&mStyleButtons[i], LV_STATE_DEFAULT, mainColor[i]);
            lv_style_set_bg_grad_color(&mStyleButtons[i], LV_STATE_DEFAULT, gradColorDim[i]);
            lv_style_set_bg_grad_dir(&mStyleButtons[i], LV_STATE_DEFAULT, LV_GRAD_DIR_VER);

            lv_style_set_bg_color(&mStyleButtons[i], LV_STATE_PRESSED, gradColorLit[i]);
            lv_style_set_bg_grad_color(&mStyleButtons[i], LV_STATE_PRESSED, mainColor[i]);
            lv_style_set_bg_grad_dir(&mStyleButtons[i], LV_STATE_PRESSED, LV_GRAD_DIR_VER);

            // Add halo effect on pressed/focused
            lv_style_set_transition_time(&mStyleButtons[i], LV_STATE_PRESSED, TONE_TIME);
            lv_style_set_transition_time(&mStyleButtons[i], LV_STATE_DEFAULT, 0);
            lv_style_set_transition_delay(&mStyleButtons[i], LV_STATE_DEFAULT, 10);
            lv_style_set_outline_width(&mStyleButtons[i], LV_STATE_DEFAULT, 0);
            lv_style_set_outline_width(&mStyleButtons[i], LV_STATE_PRESSED, 20);
            lv_style_set_outline_opa(&mStyleButtons[i], LV_STATE_DEFAULT, LV_OPA_COVER);
            lv_style_set_outline_opa(&mStyleButtons[i], LV_STATE_FOCUSED, LV_OPA_COVER);
            lv_style_set_outline_opa(&mStyleButtons[i], LV_STATE_PRESSED, LV_OPA_TRANSP);
            lv_style_set_transition_prop_1(&mStyleButtons[i], LV_STATE_DEFAULT, LV_STYLE_OUTLINE_OPA);
            lv_style_set_transition_prop_2(&mStyleButtons[i], LV_STATE_DEFAULT, LV_STYLE_OUTLINE_WIDTH);
        }
    }

    log_d("Initializing menu");
    {
        lv_obj_t *button;
        lv_obj_t *label;

        int offset = 64; // starting offset down the screen
        constexpr int buttonSpacing = 32;
        constexpr int buttonWidth = 120;
        constexpr int buttonHeight = 24;

        label = lv_label_create(menuTile, NULL);
        lv_label_set_text_static(label, "Simon Says");
        lv_obj_align(label, menuTile, LV_ALIGN_IN_TOP_MID, 0, 10);
        lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);

        button = lv_btn_create(menuTile, NULL);
        label = lv_label_create(button, NULL);
        lv_label_set_text(label, "New Game");
        lv_obj_set_event_cb(button, OnNewGame);
        lv_obj_set_size(button, buttonWidth, buttonHeight);
        lv_obj_align(button, menuTile, LV_ALIGN_IN_TOP_MID, 0, offset);
        offset += buttonSpacing;

        button = lv_btn_create(menuTile, NULL);
        label = lv_label_create(button, NULL);
        lv_label_set_text(label, "Exit");
        lv_obj_set_event_cb(button, OnExit);
        lv_obj_align(button, menuTile, LV_ALIGN_IN_TOP_MID, 0, offset);
        lv_obj_set_size(button, buttonWidth, buttonHeight);
        offset += buttonSpacing;
    }

    log_d("Initializing game board");
    {
        // Display a grid on the gameplay page
        constexpr int tops[2] = {
            20,
            124,
        };
        constexpr int lefts[2] = {20, 124};
        constexpr int width = 96;
        constexpr int height = 96;
        const lv_event_cb_t funcs[NUM_SQUARES] = {
            OnSquareUL, OnSquareUR,
            OnSquareBL, OnSquareBR};

        for (int i = 0; i < NUM_SQUARES; i++)
        {
            mButtons[i] = lv_btn_create(gameplayTile, NULL);
            if (!mButtons[i])
                log_e("Error creating button %d. Crash is immenent.", i);
            lv_obj_set_pos(mButtons[i], tops[i % 2], lefts[i / 2]);
            lv_obj_set_size(mButtons[i], width, height);
            lv_obj_reset_style_list(mButtons[i], LV_BTN_PART_MAIN);
            lv_obj_add_style(mButtons[i], LV_BTN_PART_MAIN, &mStyleButtons[i]);
            lv_obj_set_event_cb(mButtons[i], funcs[i]);
        }
    }

    log_d("Construction complete");
}

SimonSaysApp::~SimonSaysApp()
{
    // LVGL Objects parented to the tiles should be deleted when the tiles are destroyed.
    FreeAppTiles();
    gameInstance = nullptr;
}

// Open from watch menu
void SimonSaysApp::OnLaunch()
{
    lv_tileview_set_tile_act(GetTileView(), 0, 0, LV_ANIM_OFF);
}

void SimonSaysApp::OnExitClicked()
{
    log_d("Exiting...");
    // Pass along the message for differed deletion and return to main menu
    mParentIcon->OnExitClicked();
    FreeAppTiles();
}

void SimonSaysApp::OnTileClicked(int index)
{
    if (index < 0 || index >= NUM_SQUARES)
    {
        log_e("Invaid tile number, expected 0-%d, received %d", NUM_SQUARES, index);
        return;
    }

    if (!mPattern.size())
    {
        log_d("We aren't playing yet. Return to the menu screen.");
        lv_tileview_set_tile_act(GetTileView(), 0, 0, LV_ANIM_ON);
        return;
    }

    if (mIsDoingReplay)
    {
        log_d("Button pressed during replay. Ignoring it.");
        return;
    }

    if (mCurrentNote < mPattern.size())
    {
        if (mPattern[mCurrentNote] == index)
        {
            // Passed, yay! Advance a note.
            mCurrentNote++;

            // Did they finish the round?
            if (mCurrentNote == mPattern.size())
            {
                AddNoteAndReplay();
            }

            // Didn't finish the round? Wait for more input.
            return;
        }
    }

    // If you hit here, you didn't pass. FAIL!
    // For now, just return to menu screen.
    lv_tileview_set_tile_act(GetTileView(), 0, 0, LV_ANIM_ON);
}

void SimonSaysApp::OnMenuClicked(MenuItem item)
{
    switch (item)
    {
    case NewGame:
        StartNewGame();
        break;
    case Exit:
        OnExitClicked();
        break;
    default:
        log_e("Unknown menu command %d", item);
    }
}

void SimonSaysApp::StartNewGame()
{
    // Clear the pattern array and reset player playback
    mPattern.clear();
    mCurrentNote = 0;

    lv_tileview_set_tile_act(GetTileView(), 1, 0, LV_ANIM_ON);

    AddNoteAndReplay();
}

void SimonSaysApp::AddNoteAndReplay()
{
    // Add the item to the list
    mPattern.push_back(random(NUM_SQUARES));

    log_d("Adding note to playback, currently at %d. Pattern:", mPattern.size());
    for (int8_t &tone : mPattern)
    {
        log_d(" %d", tone);
    }

    // Mark the data to the new state
    mIsDoingReplay = true;

    // Press the button to trigger the effect
    mCurrentNote = 0;
    lv_btn_set_state(mButtons[mPattern[mCurrentNote]], LV_BTN_STATE_PRESSED);

    // Set the task to run again
    if (mPlaybackTask != NULL)
    {
        log_d("Old playback task wasn't finished! Overwriting it, but should investigate.");
    }
    mPlaybackTask = lv_task_create(PlaybackTonesTask, TONE_TIME, LV_TASK_PRIO_MID, this);
    mPlaybackTask->repeat_count = mPattern.size();
}

void SimonSaysApp::OnNextTone()
{
    if (!mIsDoingReplay)
    {
        log_d("Playing tone but not inside replay. Abort.");
        return;
    }

    if (mCurrentNote >= mPattern.size())
    {
        log_d("Playback note is farther along than the tune. Abort.");
        return;
    }

    // Clear the old button effect
    lv_btn_set_state(mButtons[mPattern[mCurrentNote]], LV_BTN_STATE_PRESSED);

    // Increase
    mCurrentNote++;

    if (mCurrentNote < mPattern.size())
    {
        // Items remain, repeat.
        lv_btn_set_state(mButtons[mPattern[mCurrentNote]], LV_BTN_STATE_PRESSED);
    }
    else
    {
        // Done!
        mCurrentNote = 0;
        mIsDoingReplay = false;
        mPlaybackTask = 0;
    }
}