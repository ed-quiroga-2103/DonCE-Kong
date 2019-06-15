//
// Created by eduardo on 01/06/19.
//

#ifndef UNTITLED_SPRITE_H
#define UNTITLED_SPRITE_H
#include <allegro5/bitmap.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>
#include "LinkedList.h"
#include "Settings.h"
#include "Sprite.c"


const float VEL = 1;

//Sprite struct
struct Sprite{

    float x, y;

    float accX, accY;

    float velX, velY;

    int dirX, dirY;

    int bx, by;

    int w, h;

    bool movingR;
    bool movingL;
    bool lastR;
    bool jumping;
    bool climbing;
    bool hammer;

    int imageInd, hammerInd;

    ALLEGRO_BITMAP *image[2];
    ALLEGRO_BITMAP *spriteSheet;

};
//Barrel struct
struct Barrel{
    float x, y;
    float velX, velY;
    int type, w, h, dir, ind, land;
    ALLEGRO_BITMAP *spriteSheet;

};


/*
 * Description: Creates a barrel and pushes it into the list
 * Input: X, Y, Barrel type, Barrel list, Barrel Size
 * Output: None
 *
 * */
void createBarrel(float x, float y,int type, struct Node **node, int spriteSize){

    struct Barrel barrel;

    barrel.x = x;
    barrel.y = y;
    barrel.ind = 0;
    barrel.type = type;
    barrel.spriteSheet = al_load_bitmap("Sprites/misc-2.png");
    assert(barrel.spriteSheet != NULL);



    if(type == 1){
        barrel.w = 77-66;
        barrel.h = 267-258;

        barrel.velX = 2;
        barrel.velY = barrel.velX;
        barrel.dir = 1;

    }
    else if(type == 2){

        barrel.w = 110-96;
        barrel.h = 268-259;
        barrel.velX = 0;
        barrel.velY = 3.5;

    }
    else if(type == 3){
        barrel.w = 110-96;
        barrel.h = 268-259;


        barrel.velX = 2;
        barrel.velY = barrel.velX;

        srand(time(NULL));   // Initialization, should only be called once.
        int ind = rand()%4;

        barrel.land = barrelLanding[ind];

        switch (ind){

            case 0:
                barrel.dir = 1;
                break;
            case 1:
                barrel.dir = -1;
                break;
            case 2:
                barrel.dir = 1;
                break;
            case 3:
                barrel.dir = -1;
                break;

        }
    }

    push(node,&barrel,spriteSize);
}


/*
 * Description: Returns a boolean that indicates if the barrel is colliding with a platform
 * Input: Barrel struct, Sprite struct
 * Output: True/False
 *
 * |||All the collition checking functions work the same way, because of that, there will be no documentation for other "is_X_Colliding" function
 *
 * */

bool isBarrelColliding(struct Barrel *sprite, struct Sprite target){

    if(sprite->y + sprite->h*SCALE < target.y + target.h -2 &&
       sprite->x < target.x  + target.w &&
       sprite->x + sprite->w*SCALE > target.x&&
       sprite->y + sprite->h*SCALE > target.y +2){

        sprite->y = target.y + 5 - sprite->h*SCALE;
        sprite->velY = 0;
        return true;

    }
    else{

        return false;

    }


}


/*
 * Description: Updates the index of all the barrels. The index is used to select the image for the animation
 * Input: Barrel list
 * Output: None
 *
 * */
void updateBarrelInds(struct Node *node){

    struct Barrel * target;

    while (node != NULL)
    {
        target = (struct Barrel *)node->data;

        target->ind+=1;
        target->ind = target->ind%4;

        node = node->next;
    }

}

bool isBarrelCollidingAll(struct Barrel *barrel, struct Node *node){
    struct Sprite * target;

    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        if(isBarrelColliding(barrel,*target)){
            return true;
        }
        node = node->next;
    }

    return false;

}

/*
 * Description: Updates the position of the barrels
 * Input: Barrel list, Platform list, level
 * Output: None
 *
 * */

void updateAllBarrels(struct Node *node, struct Node *platList, int level){
    struct Barrel * target;

    while (node != NULL)
    {
        target = (struct Barrel *)node->data;

        updateBarrel(target, platList, level);

        node = node->next;
    }

}


/*
 * Description: Updates the position of a single barrel
 * Input: Barrel struct, Platform list, level
 * Output: None
 *
 * */

void updateBarrel(struct Barrel *barrel, struct Node *node, int level){

    if(barrel->type == 1){

        al_draw_scaled_bitmap(barrel->spriteSheet,
                              barrelImageX[barrel->ind], barrelImageY[barrel->ind], barrel->w+1, barrel->h+1,
                              barrel->x, barrel->y, (barrel->w)*SCALE, barrel->h*SCALE, 0);


        if(isBarrelCollidingAll(barrel,node)){
            if(barrel->y > 536*1.15){
                barrel->dir = -1;
            }
            else if(barrel->x > 530*1.15) {
                barrel->dir = -1;
            }
            else if(barrel->x < 54*1.15){
                barrel->dir = 1;
            }

            barrel->x += (barrel->velX+(barrel->velX*level)/5)*barrel->dir;

        }
        else{

            barrel->y += barrel->type + (barrel->type*level)/5;
        }
    }
    else if(barrel->type == 2){

        al_draw_scaled_bitmap(barrel->spriteSheet,
                              barrelImageX2[barrel->ind%2], barrelImageY2[barrel->ind%2], barrel->w+1, barrel->h+1,
                              barrel->x, barrel->y, (barrel->w)*SCALE, barrel->h*SCALE, 0);
        barrel->y += barrel->velY;

    }
    else if(barrel->type == 3){

        if(barrel->y < barrel->land){

            al_draw_scaled_bitmap(barrel->spriteSheet,
                                  barrelImageX2[barrel->ind%2], barrelImageY2[barrel->ind%2], barrel->w+1, barrel->h+1,
                                  barrel->x, barrel->y, (barrel->w)*SCALE, barrel->h*SCALE, 0);
            barrel->y += barrel->velY;


        }
        else{
            barrel->w = 77-66;
            barrel->h = 267-258;

            al_draw_scaled_bitmap(barrel->spriteSheet,
                                  barrelImageX[barrel->ind], barrelImageY[barrel->ind], barrel->w+1, barrel->h+1,
                                  barrel->x, barrel->y, (barrel->w)*SCALE, barrel->h*SCALE, 0);


            if(isBarrelCollidingAll(barrel,node)) {
                if (barrel->y > 536 * 1.15) {
                    barrel->dir = -1;
                } else if (barrel->x > 530 * 1.15) {
                    barrel->dir = -1;
                } else if (barrel->x < 54 * 1.15) {
                    barrel->dir = 1;
                }

                barrel->x += (barrel->velX + (barrel->velX * level) / 5) * barrel->dir;


            }
            else{

                barrel->y += barrel->type + (barrel->type*level)/5;
            }
        }

    }

}


/*
 * Description: Draws a generic sprite
 * Input: Sprite struct
 * Output: None
 *
 * */

void drawSprite(struct Sprite *sprite){

    al_draw_bitmap(sprite->image[sprite->imageInd], sprite->x, sprite->y, 0);

}


/*
 * Description:  Allocates a sprite in memory
 * Input: None
 * Output: Sprite struct
 *
 * */


struct Sprite allocateSprite(){

    return *((struct Sprite*) malloc(sizeof(struct Sprite)));

}


/*
 * Description:  Draws the image of the player using the imageInd value
 * Input: Player struct
 * Output: None
 *
 * */
void drawPlayer(struct Sprite *player){

    if(player->jumping){

        if(player->lastR) {


            al_draw_scaled_bitmap(player->spriteSheet,
                                  239, 0, 13+5, 24,
                                  player->x, player->y, (13+5)*SCALE, 24*SCALE, 0);
        }
        else{


            al_draw_scaled_bitmap(player->spriteSheet,
                                  39, 0, 13, 24,
                                  player->x, player->y, 13*SCALE, 24*SCALE, 0);

        }

    }

    else if(player->movingR){
/*
        if(player->hammer){
            ALLEGRO_BITMAP * hammerSprites = al_load_bitmap("Sprites/misc-2.png");

            player->h = playerHammerH[player->hammerInd];
            player->w = playerHammerW[player->hammerInd];


            al_draw_scaled_bitmap(hammerSprites,
                                  playerHammerX[player->hammerInd], playerHammerY[player->hammerInd],
                                  playerHammerW[player->hammerInd], playerHammerH[player->hammerInd],
                                  player->x, player->y,
                                  playerHammerW[player->hammerInd]*SCALE, playerHammerH[player->hammerInd]*SCALE, 0);

        }
        else {*/


            al_draw_scaled_bitmap(player->spriteSheet,
                                  playerImageX1[player->imageInd], 0, 13, 24,
                                  player->x, player->y, (13 + 3) * SCALE, 24 * SCALE, 0);
       // }
    }
    else if(player->movingL){

        al_draw_scaled_bitmap(player->spriteSheet,
                              playerImageX2[player->imageInd], 0, 13, 24,
                              player->x, player->y, 13*SCALE, 24*SCALE, 0);

    }
    else if(player->lastR){

        al_draw_scaled_bitmap(player->spriteSheet,
                              playerImageX1[0], 0, 13, 24,
                              player->x, player->y, 13*SCALE, 24*SCALE, 0);

    }
    else{

        al_draw_scaled_bitmap(player->spriteSheet,
                              playerImageX2[0], 0, 13, 24,
                              player->x, player->y, 13*SCALE, 24*SCALE, 0);


    }

}

/*
 * Description:  Updates the position of the player
 * Input: Player struct
 * Output: None
 *
 * */
void updatePlayer(struct Sprite *player){

    if(!player->climbing) {
        player->accY += player->velY * -.25;
        player->velY += player->accY;
        player->y += player->velY + player->accY;
    }

    player->accX += player->velX * FRICC;
    player->velX += player->accX;
    player->x += player->velX + player->accX * ACC;

    if(abs(round(player->velX)) < 0.01){

        player->accX = 0;
        player->movingR = false;
        player->movingL = false;

    }


}


/*
 * Description:  Creates a platform using its coordinates
 * Input: X, Y
 * Output: Sprite struct
 *
 * */
struct Sprite createPlatform(float x, float y){

    struct Sprite plat;
    plat.x = x;
    plat.y = y;

    //plat.imageInd = 0;

    //plat.image[0] = al_load_bitmap("Sprites/FloatingPlat.png");


    plat.h = 602-584;//al_get_bitmap_height(plat.image[plat.imageInd]);
    plat.w = 300-36;//al_get_bitmap_width(plat.image[plat.imageInd]);


    return plat;

}


/*
 * Description: Creates a ladder using its coordinates
 * Input: X1, Y2, X2, Y2
 * Output: Sprite struct
 *
 * */
struct Sprite createLadder(float x1, float y1, float x2, float y2){

    struct Sprite ladder;
    ladder.x = x1;
    ladder.y = y1;

    ladder.h = y2 - y1;
    ladder.w = x2 - x1;

    return ladder;

}

//Colliding functions

bool isTopColliding(struct Sprite * sprite, struct Sprite target){


    if(sprite->y < target.y + target.h &&
       sprite->x < target.x  + target.w &&
       sprite->x + sprite->w > target.x&&
       sprite->y > target.y){


        if(!sprite->climbing) {
            sprite->y = target.y + target.h + 1;
        }
        return true;

    }
    else {return false;}

}

bool isColliding(struct Sprite *sprite, struct Sprite target){
    if(sprite->y + sprite->h < target.y + target.h -2 &&
            sprite->x < target.x  + target.w &&
       sprite->x + sprite->w > target.x&&
       sprite->y + sprite->h > target.y +2){

        if(!sprite->climbing) {
            sprite->y = target.y + 2 - sprite->h;
        }
        sprite->jumping = false;
        return true;

    }
    else{

        return false;

    }

}

bool isCollidingWithAny(struct Sprite *player, struct Node *node){
    struct Sprite * target;


    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        if(isColliding(player,*target)){

            return true;

        }

        node = node->next;
    }


    return false;

}

bool isCollidingWithBarrel(struct Sprite * sprite, struct Barrel *target){
    if(sprite->y < target->y + target->h -2 &&
       sprite->x < target->x  + target->w &&
       sprite->x + sprite->w > target->x&&
       sprite->y + sprite->h > target->y +2){

        if(sprite->hammer){

            target->x = -500;
            target->y = -500;
            return false;

        }

        return true;

    }
    else{

        return false;

    }
}

bool isCollidingWithBarrels(struct Sprite *player, struct Node *node){
    struct Barrel * target;


    while (node != NULL)
    {
        target = (struct Barrel *)node->data;

        if(isCollidingWithBarrel(player,target)){

            return true;

        }

        node = node->next;
    }


    return false;

}

bool isTopCollidingWithAny(struct Sprite *player, struct Node *node){
    struct Sprite * target;


    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        if(isTopColliding(player,*target)){

            return true;

        }

        node = node->next;
    }


    return false;

}

bool ladderCollide(struct Sprite *player, struct Sprite *ladder){

    float playerBottom = player->y + player->h;

    if(playerBottom > ladder->y &&
       player->x > ladder->x-player->w &&
       player->x < ladder->x+5 + ladder->w &&
       playerBottom < ladder->y + ladder->h){


        return true;

    }
    else{

        return false;

    }


}

bool allLadderCollide(struct Sprite* player, struct Node *node){
    struct Sprite * target;


    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        if(ladderCollide(player,target)){

            return true;

        }

        node = node->next;
    }


    return false;

}

bool hammerCollide(struct Sprite *sprite, struct Sprite * target){

    if(sprite->y < target->y + target->h -2 &&
       sprite->x < target->x  + target->w &&
       sprite->x + sprite->w > target->x&&
       sprite->y + sprite->h > target->y +2){


        target->x = -500;
        target->y = -500;

        return true;

    }
    else{
        return false;

    }


}

bool allHammerCollide(struct Sprite* player, struct Node *node){
    struct Sprite * target;


    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        if(hammerCollide(player,target)){

            return true;

        }

        node = node->next;
    }


    return false;

}

//------------------------------------------------


/*
 * Description:  The next 7 functions create the platforms and the ladders that appear on screen, and pushes them into a list
 * Input: Ladder list/ Platform list, spriteSize
 * Output: None
 *
 * */


void genFirstLine(struct Node** node, unsigned spriteSize){

    float x = PLAT_COORDSX1*1.15;
    float y = PLAT_COORDSY1*1.15;

    for (int i = 0; i < 7; ++i) {

        struct Sprite plat = createPlatform(x,y);
        plat.w = PLAT_W*1.15;
        plat.h = PLAT_H*1.15;

        push(node,&plat,spriteSize);

        x+=37*1.15;
        y-=2*1.15;

    }


}

void genSecondLine(struct Node** node, unsigned spriteSize){

    float x = 489*1.15;
    float y = 514*1.15;

    for (int i = 0; i < 13 ; i++) {


        struct Sprite plat = createPlatform(x,y);
        plat.w = PLAT_W*1.15;
        plat.h = PLAT_H*1.15;

        push(node,&plat,spriteSize);

        x-=37*1.15;
        y-=2*1.15;

    }

}

void genThirdLine(struct Node** node, unsigned spriteSize){

    float x = 76*1.15;
    float y = 441*1.15;

    for (int i = 0; i < 13 ; i++) {


        struct Sprite plat = createPlatform(x,y);
        plat.w = PLAT_W*1.15;
        plat.h = PLAT_H*1.15;

        push(node,&plat,spriteSize);

        x+=37*1.15;
        y-=2*1.15;

    }

}

void genFourthLine(struct Node** node, unsigned spriteSize){

    float x = 489*1.15;
    float y = 364*1.15;

    for (int i = 0; i < 13 ; i++) {

        struct Sprite plat = createPlatform(x,y);
        plat.w = PLAT_W*1.15;
        plat.h = PLAT_H*1.15;

        push(node,&plat,spriteSize);

        x-=37*1.15;
        y-=2*1.15;

    }

}

void genFifthLine(struct Node** node, unsigned spriteSize){

    float x = 76*1.15;
    float y = 287*1.15;

    for (int i = 0; i < 13 ; i++) {

        struct Sprite plat = createPlatform(x,y);
        plat.w = PLAT_W*1.15;
        plat.h = PLAT_H*1.15;

        push(node,&plat,spriteSize);

        x+=37*1.15;
        y-=2*1.15;

    }

}

void genLastLine(struct Node** node, unsigned spriteSize){


    float x = 489*1.15;
    float y = 209*1.15;

    for (int i = 0; i < 4; ++i) {

        struct Sprite plat = createPlatform(x,y);
        plat.w = PLAT_W*1.15;
        plat.h = PLAT_H*1.15;

        push(node,&plat,spriteSize);

        x-=37*1.15;
        y-=2*1.15;

    }

    struct Sprite plat = createPlatform(39*1.15,200*1.15);
    plat.w = (376-39)*1.15;
    plat.h = (218-200)*1.15;
    push(node,&plat,spriteSize);

    plat = createPlatform(245*1.15,133*1.15);
    plat.w = (357-245)*1.15;
    plat.h = (152-133)*1.15;

    push(node,&plat,spriteSize);


}

void genLadders(struct Node** node, unsigned spriteSize){

    for (int i = 0; i < ladderNum; ++i) {

        struct Sprite ladder = createLadder(ladderX1[i]*1.15, ladderY1[i]*1.15, ladderX2[i]*1.15, ladderY2[i]*1.15);
        ladder.h *= 1.15;
        ladder.w *= 1.15;

        push(node,&ladder,spriteSize);


    }

}



/*
 * Description: Creates a hammer sprite
 * Input: Index, used in the coordinates selection
 * Output: Sprite struct
 *
 * */

struct Sprite createHammer(int ind){

    struct Sprite hammer;
    hammer.x = hammerXCoords[ind]*1.15;
    hammer.y = hammerYCoords[ind]*1.15;

    hammer.h = hammerH;
    hammer.w = hammerW;

    return hammer;

}


/*
 * Description:  Generates all the hammers on screen
 * Input: Hammer list, sprite size
 * Output: Sprite struct
 *
 * */

void genHammers(struct Node** node, unsigned spriteSize){

    struct Sprite hammer;

    for (int i = 0; i < 2; ++i) {

        hammer = createHammer(i);
        push(node,&hammer,spriteSize);

    }

}


/*
 * Description:  Draws all the hammers on screen
 * Input: Hammer list
 * Output: None
 *
 * */

void drawHammers(struct Node* node){

    ALLEGRO_BITMAP * spritesheet = al_load_bitmap("Sprites/misc-2.png");


    struct Sprite * target;


    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        al_draw_scaled_bitmap(spritesheet,
                              hammerX,hammerY, hammerW, hammerH,
                              target->x,target->y,hammerW*SCALE, hammerH*SCALE, 0);



        node = node->next;
    }


}

//Fireball creation works the same way than the hammer creation

struct Sprite createFireBall(struct Node** node){

    struct Sprite fireball;
    fireball.x = 0;
    fireball.y = 800;
    fireball.imageInd = 0;
    fireball.spriteSheet = al_load_bitmap("Sprites/misc-2.png");

    push(node,&fireball, sizeof(struct Sprite));
    printf("Created");
    return fireball;

}



/*
 * Description: Moves the fireballs closer to Mario
 * Input: Player struct, Fireball struct, level
 * Output: None
 *
 * */
void moveFireBall(struct Sprite *player, struct Sprite *fireball, int level){

    int testX = 500;
    int testY = 500;
    int minInd = 0;
    double distance = pow(player->x-fireball->x,2) + pow(player->y-fireball->y,2);
    distance = sqrt(distance);

    double distanceCmp;

    int x[8] = {1,-1,0,0,1,-1,1,-1};
    int y[8] = {0,0,1,-1,1,-1,-1,1};

    for (int i = 0; i < 8; ++i) {
        distanceCmp = pow(player->x-x[i]-fireball->x,2) + pow(player->y-y[i]-fireball->y,2);
        distanceCmp = sqrt(distanceCmp);
        if(distanceCmp < distance){
            distance = distanceCmp;
            minInd = i;
        }

    }


    switch (minInd){

        case 0:
            fireball->x += x[minInd]+level*.10;
            fireball->y += y[minInd];
            break;

        case 1:
            fireball->x += x[minInd]-level*.10;
            fireball->y += y[minInd];
            break;

        case 2:
            fireball->x += x[minInd];
            fireball->y += y[minInd]+level*.10;
            break;

        case 3:
            fireball->x += x[minInd];
            fireball->y += y[minInd]-level*.10;
            break;

        case 4:
            fireball->x += x[minInd]+level*.10;
            fireball->y += y[minInd]+level*.10;
            break;

        case 5:
            fireball->x += x[minInd]-level*.10;
            fireball->y += y[minInd]-level*.10;
            break;

        case 6:
            fireball->x += x[minInd]+level*.10;
            fireball->y += y[minInd]-level*.10;
            break;

        case 7:
            fireball->x += x[minInd]-level*.10;
            fireball->y += y[minInd]+level*.10;
            break;


    }


}


/*
 * Description:  Draws and moves all the fireballs on screen
 * Input: Player struct, Fireball struct, level
 * Output: None
 *
 * */
void updateFireBall(struct Sprite *player,struct Sprite *fireball, int level){

    moveFireBall(player,fireball, level);

    al_draw_scaled_bitmap(fireball->spriteSheet,
                          fireBallX[fireball->imageInd], fireBallY[fireball->imageInd],
                          173-157, 237-221,
                          fireball->x, fireball->y, (173-157)*SCALE, (237-221)*SCALE, 0);

}

void drawFireBall(struct Sprite *fireball){

    al_draw_scaled_bitmap(fireball->spriteSheet,
                          fireBallX[fireball->imageInd], fireBallY[fireball->imageInd],
                          173-157, 237-221,
                          fireball->x, fireball->y, (173-157)*SCALE, (237-221)*SCALE, 0);
}

void drawFireBalls(struct Node *node){
    struct Sprite * target;

    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        drawFireBall(target);

        node = node->next;
    }


}

void updateAllFireBalls(struct Sprite *player, struct Node *node, int level){

    struct Sprite * target;

    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        updateFireBall(player,target,level);

        node = node->next;
    }

}


#endif //UNTITLED_SPRITE_H
