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

const int fireBallX[2] = {157,132};
const int fireBallY[2] = {221,221};

const double barrelLanding[4] = {471*1.15,389*1.15,306*1.15,235*1.15};

const int playerHammerX[3] = {158,188,208};
const int playerHammerY[3] = {54,41,55};

const int playerHammerW[3] = {183-158,202-188,236-208};
const int playerHammerH[3] = {71-54,71-41, 71-55};

const int barrelImageX[4] = {66,81,81,66};
const int barrelImageY[4] = {258,258,270,270};

const int barrelImageX2[2] = {96,96};
const int barrelImageY2[2] = {259,270};

const int heartX = 206;
const int heartY = 268;

const int heartW = 220-206;
const int heartH = 280 - 268;

const int hammerX = 195;
const int hammerY = 265;

const int hammerW = 203 - 195;
const int hammerH = 280- 265;

const int hammerXCoords[2] = {116, 491};
const int hammerYCoords[2] = {399, 294};



const int ladderNum = 10;

const int ladderX1[10] = {226,470,264,114,311,470,208,114,470,337};
const int ladderX2[10] = {248,488,282,132,329,488,226,132,488,358};

const int ladderY1[10] = {565,516,429,439,352,361,279,284,207,133};
const int ladderY2[10] = {585,572,504,496,427,418,345,339,263,201};


#endif //UNTITLED_SETTINGS_H
