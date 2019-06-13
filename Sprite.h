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

    int imageInd;

    ALLEGRO_BITMAP *image[2];
    ALLEGRO_BITMAP *spriteSheet;

};

struct Barrel{
    float x, y;
    float velX, velY;
    int type, w, h, dir, ind;
    ALLEGRO_BITMAP *spriteSheet;

};

void createBarrel(float x, float y, int type, struct Node **node, int spriteSize){

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

        barrel.velX = 3.5;
        barrel.velY = barrel.velX;
        barrel.dir = 1;

    }
    else if(type == 2){
        barrel.w = 110-96;
        barrel.h = 268-259;

        barrel.velX = 0;
        barrel.velY = 3.5;

    }

    push(node,&barrel,spriteSize);
}

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

void updateAllBarrels(struct Node *node, struct Node *platList){
    struct Barrel * target;

    while (node != NULL)
    {
        target = (struct Barrel *)node->data;

        updateBarrel(target, platList);

        node = node->next;
    }

}

void updateBarrel(struct Barrel *barrel, struct Node *node){

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

            barrel->x += barrel->velX*barrel->dir;


        }
        else{

            barrel->y += barrel->type;
        }
    }
    else if(barrel->type == 2){

        al_draw_scaled_bitmap(barrel->spriteSheet,
                              barrelImageX2[barrel->ind%2], barrelImageY2[barrel->ind%2], barrel->w+1, barrel->h+1,
                              barrel->x, barrel->y, (barrel->w)*SCALE, barrel->h*SCALE, 0);
        barrel->y += barrel->velY;

    }

}


void drawSprite(struct Sprite *sprite){

    al_draw_bitmap(sprite->image[sprite->imageInd], sprite->x, sprite->y, 0);

}

struct Sprite allocateSprite(){

    return *((struct Sprite*) malloc(sizeof(struct Sprite)));

}

struct Sprite createPlayer(float x, float y){

    struct Sprite player;

    return player;

}

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

        //al_draw_bitmap_region(player->spriteSheet, playerImageX1[player->imageInd], 0, 13
          //      , 24, player->x, player->y, 0);

        al_draw_scaled_bitmap(player->spriteSheet,
                                   playerImageX1[player->imageInd], 0, 13, 24,
                                   player->x, player->y, (13+3)*SCALE, 24*SCALE, 0);

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

    printf("Vel: %f\n", player->velX);

}

void showHitbox(struct Sprite sprite){

    al_draw_filled_rectangle(sprite.x,
                             sprite.y, sprite.x+sprite.w, sprite.y+sprite.h
            , al_map_rgba_f(.6,0,.6,.6));

}

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

struct Sprite createLadder(float x1, float y1, float x2, float y2){

    struct Sprite ladder;
    ladder.x = x1;
    ladder.y = y1;

    ladder.h = y2 - y1;
    ladder.w = x2 - x1;

    return ladder;

}

bool isBottomColliding(struct Sprite *player, struct Sprite *wall){

    if(player->y + player->h +2> wall->y && player->y + player->h < wall->y + wall->h){
        //player->y = target->y+1 - player->h;
        return true;
    }
    else{
        return false;
    }

}

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

bool isSideColliding(struct Sprite * player, struct Sprite tiles){
/*
    float y = player->y;

    for (; y < player->y+player->h; y++) {

        if(tiles.y <= y-2 &&
            tiles.y+tiles.h >= y-2 &&
            player->x + player->w > tiles.x &&
            player->y < tiles.y+tiles.h && player->jumping){


            printf("Y: %f,%f\n", tiles.y, y);

            player->velX = 0;
            player->velY = 0;

            player->x = tiles.x-player->w;

            return true;

        }

*/
    float pw = player->w, ph = player->h;
    float px = player->x, py = player->y;
    float lx = tiles.x, ly = tiles.y;
    float lw = tiles.w, lh = tiles.h;

    if(py+ph > ly && py<ly+lh){

        if(px+pw > lx && px < lx){

            printf("colliding");


        }

    }

}

bool isSideCollidingWithAny(struct Sprite* player, struct Node *node){

    struct Sprite * target;


    while (node != NULL)
    {
        target = (struct Sprite *)node->data;

        if(isSideColliding(player,*target)){

            return true;

        }

        node = node->next;
    }


    return false;

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

void drawAllSprites(struct Node *node){
    while (node != NULL)
    {
        struct Sprite * sprite = (struct Sprite *)node->data;
        drawSprite(sprite);
        node = node->next;
    }
}

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



}

void genLadders(struct Node** node, unsigned spriteSize){

    for (int i = 0; i < ladderNum; ++i) {

        struct Sprite ladder = createLadder(ladderX1[i]*1.15, ladderY1[i]*1.15, ladderX2[i]*1.15, ladderY2[i]*1.15);
        ladder.h *= 1.15;
        ladder.w *= 1.15;

        push(node,&ladder,spriteSize);


    }

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




#endif //UNTITLED_SPRITE_H
