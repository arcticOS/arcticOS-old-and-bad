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
 *
 * This file contains cryptographic functions. Note that these aren't
 * very secure and should be changed before release.
 */

// TODO: Make this not overwrite the existing string, but return a new one.
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <math/crypto.h>

void OS_do_xor_crypt(char* string, const char* key, int length) {
    int key_index = 0;

    // We use a length variable instead of strlen because when we encrypt, 
    // if the character in the data matches the character in the key, we get
    // zero returned as the byte - strlen would treat this as the end of 
    // the string.
    for(int i = 0; i < length; i++) {
        string[i] ^= key[key_index];

        key_index ++;
        if(key[key_index] == 0) key_index = 0;
    }
}

// TODO: Find more secure way of seeding this
// TODO: Have a more secure way of generating random numbers
void OS_generate_new_encryption_salt(const char* seed) {
    srand((int) OS_get_hash(seed, strlen(seed)));
    for(int i = 0; i < 8; i++) {
        OS_encryption_salt[i] = rand();
    }
}

// TODO: this is super bad and easily broken
uint8_t OS_get_hash(const char* data, int length) {
    uint8_t hash = 0;
    uint8_t randomizer = (uint8_t) 20654;

    for(int i = 0; i < length; i++) {
        hash += data[i] | randomizer;
        randomizer ^= data[i];
    }

    return hash;
}