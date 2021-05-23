/*
 * arcticOS
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

 #include <arctic/ui.h>

 #ifdef EMULATOR
    // All functions if they're used in the emu
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>

    SDL_Window* EMULATOR_WINDOW;
    SDL_Renderer* EMULATOR_RENDERER;
    SDL_Event EMULATOR_EVENT;

    TTF_Font* EMULATOR_REGULAR_FONT;
    TTF_Font* EMULATOR_LARGE_FONT;

    void arctic_display_init() {
        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
        TTF_Init();

        EMULATOR_WINDOW = SDL_CreateWindow("arcticOS Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 264, 176, 0);
        EMULATOR_RENDERER = SDL_CreateRenderer(EMULATOR_WINDOW, -1, 0);

        EMULATOR_REGULAR_FONT = TTF_OpenFont("res/fonts/font.ttf", 12);
        EMULATOR_LARGE_FONT = TTF_OpenFont("res/fonts/font.ttf", 24);
        if(EMULATOR_REGULAR_FONT == NULL || EMULATOR_LARGE_FONT == NULL) {
            printf("EMULATOR ERROR: Failed to load font!\n");
            exit(-1);
        }
    }

    void arctic_display_refresh() {
        SDL_RenderPresent(EMULATOR_RENDERER);
        SDL_SetRenderDrawColor(EMULATOR_RENDERER, 255, 255, 255, 255);
        SDL_RenderClear(EMULATOR_RENDERER);
    }

    void arctic_display_draw_text(int font, int x, int y, const char* text) {
        // Select what font we need
        TTF_Font* selected_font = EMULATOR_REGULAR_FONT;
        if(font > 0) selected_font = EMULATOR_LARGE_FONT;
        
        // Generate an SDL surface and texture
        SDL_Surface* text_surface = TTF_RenderText_Blended(selected_font, text, (SDL_Color) {0, 0, 0});
        if(text_surface == NULL) {
            printf("EMULATOR ERROR: Failed to create surface from string!\n");
            exit(-1);
        }
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(EMULATOR_RENDERER, text_surface);
        if(text_texture == NULL) {
            printf("EMULATOR ERROR: Failed to convert text surface to texture!\n");
            exit(-1);
        }

        // Find what the width and height of the texture are
        int width, height;
        SDL_QueryTexture(text_texture, NULL, NULL, &width, &height);
        
        // Draw the texture on the window
        SDL_Rect draw_rect = {x, y, width, height};
        SDL_RenderCopy(EMULATOR_RENDERER, text_texture, NULL, &draw_rect);

        // Free the texture and surface
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text_surface);
    }

    char arctic_wait_for_input() {
        for(;;) {
            SDL_PollEvent(&EMULATOR_EVENT);

            switch(EMULATOR_EVENT.type) {
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    if(EMULATOR_EVENT.key.repeat == 0) {
                        const char* retstr = SDL_GetKeyName(EMULATOR_EVENT.key.keysym.sym);
                        if(retstr[0] == '\0') break;
                        return retstr[0];
                    }
                    break;
            }
        }
    }
 #else
    // All functions for real hardware
 #endif