//
// Created by eduardo on 02/06/19.
//

#ifndef UNTITLED_SETTINGS_H
#define UNTITLED_SETTINGS_H

const float FPS = 45;
const int WIDTH = 640;
const int HEIGHT = 700;
float GRAV = 0.5;
const float ACC = 0.25;
float JUMP = 35;
const float FRICC = -0.2;
const int PLATS = 3;
const char MARIO_SHEET[] = "MarioSheet.png\0";
int SCALE = 2;

const int PLAT_W = 37;
const int PLAT_H = 18;

const int PLAT_COORDSX1 = 301;
const int PLAT_COORDSY1 = 582;

const int playerImageX1[2] = {161,199};
const int playerImageX2[2] = {121,80};

const int barrelImageX[4] = {66,81,81,66};
const int barrelImageY[4] = {258,258,270,270};

const int barrelImageX2[2] = {96,96};
const int barrelImageY2[2] = {259,270};

const int ladderNum = 9;

const int ladderX1[9] = {226,470,264,114,311,470,208,114,470};
const int ladderX2[9] = {248,488,282,132,329,488,226,132,488};

const int ladderY1[9] = {565,516,429,439,352,361,279,284,207};
const int ladderY2[9] = {585,572,504,496,427,418,345,339,263};


#endif //UNTITLED_SETTINGS_H
