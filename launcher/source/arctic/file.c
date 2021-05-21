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

int arctic_get_file_size(const char* path) {
    FILE* opened_file = fopen(path, "r");
    if(opened_file == NULL) {
        printf("Error: Couldn't read file to check size");
        return 0;
    }
    fseek(opened_file, 0L, SEEK_END);
    int size = ftell(opened_file);
    fclose(opened_file);
    return size;
}