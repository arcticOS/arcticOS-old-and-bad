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

void OS_master_timed_irq() {
    /*
     * An important question to ask is whether or not a "real" multitasking system is
     * useful in this scenario. It would help make some things considerably easier,
     * but since we're running on a Harvard architecture, we can't just load in some
     * extra binaries. 
     * 
     * Because of this, the best thing to do is most likely to have 2-process multitasking,
     * where core functions run on one thread and other functions run on the other. Luckily,
     * this can be easily tweaked to become a full multitasking system later since we'll still
     * need context switching.
     */
}