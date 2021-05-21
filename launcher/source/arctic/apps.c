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

// TODO: Update all references to "res/test/test.json" to a proper app list position

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <arctic/apps.h>
#include <arctic/file.h>

#include <cJSON/cJSON.h>

cJSON* arctic_applist;
char* arctic_applist_buffer;

void arctic_refresh_applist() {
    // Clear existing applist buffer
    if(arctic_applist != NULL) {
        cJSON_Delete(arctic_applist);
        free(arctic_applist_buffer);
    }

    // Get app list size and prepare a buffer
    int applist_size = arctic_get_file_size("res/test/test.json");
    arctic_applist_buffer = (char*) malloc((applist_size + 1) * sizeof(char));

    // Open the app list
    FILE* applist_pointer = fopen("res/test/test.json", "r");
    if(applist_pointer == NULL) {
        printf("Error: Couldn't read app list!\n");
        exit(-1);
    }
    fread(arctic_applist_buffer, applist_size, 1, applist_pointer);
    fclose(applist_pointer);

    arctic_applist = cJSON_Parse(arctic_applist_buffer); // This should return a JSON array
    if(arctic_applist == NULL) {
        const char* error_pointer = cJSON_GetErrorPtr();
        if(error_pointer != NULL) printf("Error: Couldn't parse app list! Error before: %s\n", error_pointer);
        exit(-1);
    }
}

int arctic_get_app_count() {
    cJSON* applist = arctic_applist->child;
    int count = 1; // Normally we'd start at one for this, but it dies if you try to read too far.
    while(applist->next != NULL) {
        applist = applist->next;
        count ++;
    }
    return count - 1;
}

arcticApp arctic_get_app(int index) {
    arcticApp app;
    
    // Read the applist and get the path for the current app
    cJSON* applist = arctic_applist->child;

    if(index != 0) {
        for(int i = 0; i <= index; i++) {
            applist = applist->next;
        }

        index ++;
    }
    
    // Load the file
    int appfile_size = arctic_get_file_size(applist->valuestring);
    FILE* appfile_pointer = fopen(applist->valuestring, "r");
    if(appfile_pointer == NULL) {
        printf("Error: Couldn't get app %s.\n", applist->valuestring);
    } else {
        char* appfile_buffer = (char*) malloc((appfile_size + 1) * sizeof(char));
        fread(appfile_buffer, appfile_size, 1, appfile_pointer);
        fclose(appfile_pointer);

        // Read the file's JSON
        cJSON* appfile = cJSON_Parse(appfile_buffer);
        if(appfile == NULL) {
            const char* error_pointer = cJSON_GetErrorPtr();
            if(error_pointer != NULL) printf("Error: Couldn't read json for app. Error before: %s\n", error_pointer);
            else printf("Error: Couldn't read json for app.\n");
        } else {
            cJSON* appname = cJSON_GetObjectItemCaseSensitive(appfile, "friendlyName");
            cJSON* appcommand = cJSON_GetObjectItemCaseSensitive(appfile, "launchCommand");
            cJSON* appicon = cJSON_GetObjectItemCaseSensitive(appfile, "icon");

            
            strcpy(&app.name[0], appname->valuestring);
            strcpy(&app.icon_path[0], appicon->valuestring);
            strcpy(&app.command[0], appcommand->valuestring);
        }

        // Clean up and return
        cJSON_Delete(appfile);
    }
    return app;
}