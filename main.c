#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_primitives.h>
#include "Sprite.c"
#include "LinkedList.c"
#include "Settings.h"

enum KEYS {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE};

int main() {
    printf("Initializing!\n");



//----------------------- Declarations ---------------------------------------------------------------------------------

    al_init();
    al_init_image_addon();
    al_init_primitives_addon();

    ALLEGRO_BITMAP *background;
    background = al_load_bitmap("Sprites/Main.png");

    assert(background != NULL);

    unsigned width = al_get_bitmap_width(background);
    unsigned height = al_get_bitmap_height(background);


    ALLEGRO_DISPLAY *display = al_create_display(width*1.15,height*1.15);

    al_set_window_title(display, "DonCE-Kong!");

    ALLEGRO_EVENT_QUEUE * queue;
    ALLEGRO_TIMER * timer;

    bool key[5] = {false, false, false, false, false};

    bool running = true;

    queue = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);




    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));



    struct Sprite player = allocateSprite();


    struct Node *spriteList = NULL;
    struct Node *ladderList = NULL;
    struct Node *barrelList = NULL;
    unsigned spriteSize = sizeof(struct Sprite);
    unsigned barrelSize = sizeof(struct Barrel);

    //genPlats(&spriteList,spriteSize, width, height);

    struct Sprite plat;
    plat.x = 39*1.15;
    plat.y = 584*1.15;

    plat.h = (602 - 584)*1.15;
    plat.w = (300-39)*1.15;

    push(&spriteList, &plat, spriteSize);

    genFirstLine(&spriteList,spriteSize);
    genSecondLine(&spriteList,spriteSize);
    genThirdLine(&spriteList,spriteSize);
    genFourthLine(&spriteList,spriteSize);
    genFifthLine(&spriteList,spriteSize);

    genLadders(&ladderList, spriteSize);

    player.x= 100;
    player.y = 100;
    player.velX = 0;
    player.accX = 0;
    player.image[0] = al_load_bitmap("Sprites/marioStill.png");
    player.image[1] = al_load_bitmap("Sprites/marioRunning.png");

    player.movingR = false;
    player.movingL = false;
    player.lastR = true;

    player.spriteSheet = al_load_bitmap("Sprites/marioSheet.png");

    assert(player.spriteSheet != NULL);

    player.imageInd = 0;

    player.accY = GRAV;

    player.h = al_get_bitmap_height(player.image[player.imageInd])*SCALE;
    player.w = al_get_bitmap_width(player.image[player.imageInd])*SCALE;


    player.bx = player.w/2;
    player.by = player.h/2;

    //TRABAJAR DIRECTAMENTE CON PUNTEROS

    al_start_timer(timer);

    bool showingHitboxes = false;
    bool colliding = false;
    bool falling = true;
    player.climbing = false;

    bool devPriv = false;

    int count = 0;

//----------------------- Game Loop ---------------------------------------------------------------------------------

    while(running){

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);


    //----------------------- Event Detection ---------------------------------------------------------------------------------

        if(isCollidingWithAny(&player,spriteList)){

            colliding = true;
            falling = false;

        } else {colliding = false; falling = true;};

        if(allLadderCollide(&player,ladderList)){

            player.climbing = true;
            falling = true;

        }
        else{

            player.climbing = false;
        }

        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {

            switch(event.keyboard.keycode) {

                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = true;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
            }
        }
        else if(event.type == ALLEGRO_EVENT_KEY_UP) {

            switch(event.keyboard.keycode) {

                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = false;
                    break;
                case ALLEGRO_KEY_P:
                    printf("Barrel\n");
                    createBarrel(173,173,1,&barrelList,barrelSize);
            }
        }


        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){

            running = false;

        }

    //----------------------- Updates ---------------------------------------------------------------------------------
        if(key[KEY_SPACE]){
            if(!falling){
                player.jumping = true;
                player.velY -=JUMP;
                if(player.velY > JUMP){

                    player.velY = JUMP;

                }

                player.y-=1;
            }
        }

        if(player.y > height*1.15){

            running = false;

        }


        if(colliding){
            showHitbox(player);
            player.accY = 0;

        }
        else player.accY = GRAV;

        if (key[KEY_RIGHT]) {
            player.movingR = true;
            player.accX = ACC;
            player.movingL = false;
            player.lastR = true;
        }
        if(key[KEY_UP] && player.climbing){

            player.y -= 1;
        }

        if(key[KEY_DOWN] && player.climbing){

            player.y +=1;

        }
        if (key[KEY_LEFT]) {
            player.accX = -ACC;
            player.movingL = true;
            player.movingR = false;
            player.lastR = false;
        }

        if(isTopCollidingWithAny(&player, spriteList) && !player.climbing){

            player.velY = 0;
            player.jumping = false;

        }
        if(isSideCollidingWithAny(&player, spriteList)){

            //printf("Colliding\n");
        }


        updatePlayer(&player);



    //----------------------- Rendering ---------------------------------------------------------------------------------

        if(event.type == ALLEGRO_EVENT_TIMER){

            count +=1;

            if(count >= 15){

                count = 0;
                if(player.velX != 0 && (player.movingR || player.movingL)) {
                    (&player)->imageInd += 1;
                    (&player)->imageInd = (&player)->imageInd % 2;
                }
                if(!(player.movingR || player.movingL)){

                    player.imageInd = 0;

                }


            }


            al_clear_to_color(al_map_rgb_f(255,0,0));


            //drawAllSprites(spriteList);
            //al_draw_bitmap(background,0,0,0);
            al_draw_scaled_bitmap(background,0,0,width,height,0,0,width*1.15,height*1.15,0);
            updateAllBarrels(barrelList,spriteList);


            drawPlayer(&player);

            //if(colliding || showingHitboxes) showHitbox(player);


            al_flip_display();
        }

    }
//------------Finalization----------------------------------------------------------------------------------------------
    free(spriteList);

    al_destroy_display(display);
    al_uninstall_keyboard();
    al_destroy_timer(timer);

    return 0;
}
