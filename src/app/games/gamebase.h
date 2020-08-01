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

#pragma once

class gamebase
{
    private:

        /* Called when the tile is successfully registered at startup. 
        */
        virtual void OnRegistered() {};

    protected:
        // Set these during construction, used in DoRegisterTile
        const lv_img_dsc_t *pMenuIcon = 0;
        const char *pAppname = 0;
        lv_event_cb_t pStartFunction = 0;

        // Tile object created at system startup, must be persistent.
        lv_obj_t *pTile = 0;   // Tile for my application
        uint32_t mTileId;       // ID for my application tile
        lv_style_t mTileStyle; // visual style for tile icon

        /* Register this tile on the menu
    @param xpos Position on the menu. TODO: make this automatic, remove
    @param ypos Position on the menu. TODO: make this automatic, remove
    @param startEvent Static function (couldn't avoid it) to call when the app starts.
    @return True on successful registration. False means errors with registration, check the log.
    */
        bool DoRegisterTile(int xpos, int ypos);

        /* Call to return control to the parent tile. Still need to clean up your stuff. */
        void ExitApp();

    public:
        void SetupTile();

};
