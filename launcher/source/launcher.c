/*
 * arcticOS Version 0.3-alpha
 * Copyright (c) 2021 Johnny Stene <jhonnystene@protonmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <stdio.h>

#include <arctic/display.h>
#include <arctic/apps.h>

int main(int argc, char* argv[]) {
    arctic_display_init();
    arctic_display_refresh();
    arctic_display_draw_text(1, 0, 0, "arcticOS v0.3a");

    arctic_refresh_applist();

    int text_display_height = 24;

    for(int i = 0; i < arctic_get_app_count(); i++) {
        arcticApp app = arctic_get_app(i);
        arctic_display_draw_text(0, 0, text_display_height, app.name);
        text_display_height += 12;
    }

    arctic_display_refresh();

    #ifdef EMULATOR
        arctic_display_emulator_loop();
    #endif
}