/*   July 31 21:38:03 2020
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
#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/app_tile/app_tile.h" //TODO: Swap this with a game tile
#include "gamebase.h"

bool gamebase::DoRegisterTile(int xpos, int ypos)
{
    if (!pAppname || !pMenuIcon || !pStartFunction)
    {
        log_e("Class is not properly configured.");
        return false;
    }
    // get an app tile and copy mainstyle
    mTileId = mainbar_add_app_tile(xpos, ypos);
    if(mTileId == -1)
    {
        log_e("Could not get tile ID");
        return false;
    }
    pTile = mainbar_get_tile_obj(mTileId);
    if(!pTile)
    {
        log_e("Could not create tile");
        return false;
    }
    lv_style_copy(&mTileStyle, mainbar_get_style());
    lv_style_set_bg_color(&mTileStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
    lv_style_set_bg_opa(&mTileStyle, LV_OBJ_PART_MAIN, LV_OPA_100);
    lv_style_set_border_width(&mTileStyle, LV_OBJ_PART_MAIN, 0);
    lv_obj_add_style(pTile, LV_OBJ_PART_MAIN, &mTileStyle);

    // register an icon and set callback
    lv_obj_t *app = app_tile_register_app(pAppname); //TODO: have a game tile instead of app tile
    if(!app)
    {
        log_e("Could not register");
        return false;
    }
    lv_obj_t *image = lv_imgbtn_create(app, NULL);
    lv_imgbtn_set_src(image, LV_BTN_STATE_RELEASED, pMenuIcon);
    lv_imgbtn_set_src(image, LV_BTN_STATE_PRESSED, pMenuIcon);
    lv_imgbtn_set_src(image, LV_BTN_STATE_CHECKED_RELEASED, pMenuIcon);
    lv_imgbtn_set_src(image, LV_BTN_STATE_CHECKED_PRESSED, pMenuIcon);
    lv_obj_add_style(image, LV_IMGBTN_PART_MAIN, mainbar_get_style());
    lv_obj_align(image, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(image, pStartFunction);

    OnRegistered();

    return true;
}

void gamebase::SetupTile()
{
    DoRegisterTile(2, 1);
}

void gamebase::ExitApp()
{
    mainbar_jump_to_tilenumber(app_tile_get_tile_num(), LV_ANIM_OFF); //TODO: have a game tile instead of app tile
}
