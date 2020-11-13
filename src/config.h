/****************************************************************************
              config.h

    Tu May 22 21:23:51 2020
    Copyright  2020  Dirk Brosswick
 *  Email: dirk.brosswick@googlemail.com
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
#ifndef _CONFIG_H 
    #define _CONFIG_H 

    #define LILYGO_WATCH_2020_V1           //Required for T-Watch2020 V1 features
    #define LILYGO_WATCH_LVGL              //Use Light Versatile Graphics Library
    #define TWATCH_USE_PSRAM_ALLOC_LVGL    //Make LVGL use extended memory rather than default heap
    
    #include <LilyGoWatch.h>

    /*
    * firmeware version string
    */
    #define __FIRMWARE__            "2020091102"

#endif // _CONFIG_H
