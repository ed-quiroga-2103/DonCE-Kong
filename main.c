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
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define MAX 80


enum KEYS {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE};

int BARRELCODE = 0;

int clientSocket, ret;
struct sockaddr_in serverAddr;
char buffer[1024] = "Hola";
char bufferR[1024];
int msg = 0;
bool running = true;
int barrelCode = 0;
ALLEGRO_THREAD      *thread_1    = NULL;
ALLEGRO_THREAD      *thread_2    = NULL;

int start(int level);
void win();
void gameOver();

static void *Func_Thread1(ALLEGRO_THREAD *thr, void *arg);
static void *Func_Thread2(ALLEGRO_THREAD *thr, void *arg);
#define SA struct sockaddr
#define PORT 4444

/*
 *Socket function implemented from GeeksForGeeks:
 *https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *
 * Eduardo Quiroga modified:
 * -Message detection was added
 * -Message send function was removed
 */
void func(int sockfd)
{
    char buff[MAX] = "test\0";
    char buffM[MAX] = "The player won the level! Try harder!\n";
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        n = 0;
        if(msg == 1){
            write(sockfd, buffM, sizeof(buff));
            msg = 0;
        }
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);

        //Checking the message for a barrel code
        if((strncmp(buff, "1", 1)) == 0){

            barrelCode = 1;
        }
        if((strncmp(buff, "21", 2)) == 0){

            barrelCode = 21;

        }
        if((strncmp(buff, "22", 2)) == 0){

            barrelCode = 22;

        }
        if((strncmp(buff, "23", 2)) == 0){

            barrelCode = 23;

        }
        if((strncmp(buff, "31", 2)) == 0){
            printf("31\n");
            barrelCode = 31;

        }
        if((strncmp(buff, "32", 2)) == 0){

            barrelCode = 32;

        }
        if((strncmp(buff, "33", 2)) == 0){

            barrelCode = 33;

        }


        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main() {

//-------------------------------------------------Threads--------------------------------------------------------------

    int data =0;

    thread_1 = al_create_thread(Func_Thread1, &data);
    al_start_thread(thread_1);
    thread_2 = al_create_thread(Func_Thread2, &data);
    al_start_thread(thread_2);
//----------------------------------------------------------------------------------------------------------------------
    printf("Done\n");

    //Loop to keep the program running
    while(running){}

    return 0;
}


static void *Func_Thread1(ALLEGRO_THREAD *thr, void *arg) {

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("172.20.10.9");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    while (!al_get_thread_should_stop(thr)) {


        func(sockfd);

    }
        return NULL;

}

static void *Func_Thread2(ALLEGRO_THREAD *thr, void *arg) {

    while (!al_get_thread_should_stop(thr)) {
    //Settings for the game
        int lives = 5;
        int level = 0;
        while(lives != 0){
            //Winning the level
            int code = start(level);
            if (code == 1){
                level +=1;
                win();
                lives +=1;
            }
            //Win case
            else if(level == 10){
                win();
                running = false;
                break;

            }
            //Loss case
            else{
                lives-=1;
            }
        }

        gameOver();
        running = false;
        al_destroy_thread(thread_1);
        al_destroy_thread(thread_2);

    }
        return NULL;

}
//Game over display
void gameOver(){

    al_init();
    al_init_image_addon();
    al_init_primitives_addon();


    ALLEGRO_BITMAP *background;
    background = al_load_bitmap("Sprites/Main.png");

    assert(background != NULL);

    unsigned width = al_get_bitmap_width(background);
    unsigned height = al_get_bitmap_height(background);
    ALLEGRO_DISPLAY *display = al_create_display(width*1.15,height*1.15);




    ALLEGRO_EVENT_QUEUE * queue;
    ALLEGRO_TIMER * timer;


    queue = al_create_event_queue();
    timer = al_create_timer(1/FPS);


    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));



    al_set_window_title(display, "DonCE-Kong!");


    bool running = true;
    int count = 0;
    al_start_timer(timer);

    while(running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {

            switch(event.keyboard.keycode) {

                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
            }
        }


        if (event.type == ALLEGRO_EVENT_TIMER) {

            count+=1;
            if(count == 180){
                running = false;
            }
            ALLEGRO_BITMAP * gameover = al_load_bitmap("Sprites/GameOver.png");
            assert(gameover != NULL);

            al_draw_scaled_bitmap(gameover,67,150,139-67,8,(width/2)-(139-67)/2, height/2, (139-67)*2, 8*2, 0);

            al_flip_display();

        }
    }
}
//Winning display
void win(){
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();


    ALLEGRO_BITMAP *background;
    background = al_load_bitmap("Sprites/Main.png");

    assert(background != NULL);

    unsigned width = al_get_bitmap_width(background);
    unsigned height = al_get_bitmap_height(background);
    ALLEGRO_DISPLAY *display = al_create_display(width*1.15,height*1.15);




    ALLEGRO_EVENT_QUEUE * queue;
    ALLEGRO_TIMER * timer;


    queue = al_create_event_queue();
    timer = al_create_timer(1/FPS);


    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));



    al_set_window_title(display, "DonCE-Kong!");


    bool running = true;
    int count = 0;
    al_start_timer(timer);

    while(running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {

            switch(event.keyboard.keycode) {

                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
            }
        }


        if (event.type == ALLEGRO_EVENT_TIMER) {

            count+=1;
            if(count == 180){
                al_destroy_display(display);
                al_uninstall_keyboard();
                al_destroy_timer(timer);
                return;

            }
            ALLEGRO_BITMAP * gameover = al_load_bitmap("Sprites/misc-2.png");
            assert(gameover != NULL);

            al_draw_scaled_bitmap(gameover,heartX,heartY,heartW,heartH,(width/2)-heartW, height/2, heartW*3, heartH*3, 0);

            al_flip_display();

        }
    }
}
//Main game function
int start(int level){
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


    queue = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);




    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));



    struct Sprite player = allocateSprite();

    //List creation
    struct Node *spriteList = NULL;
    struct Node *ladderList = NULL;
    struct Node *barrelList = NULL;
    struct Node *hammerList = NULL;
    struct Node *fireList = NULL;
    unsigned spriteSize = sizeof(struct Sprite);
    unsigned barrelSize = sizeof(struct Barrel);

    //Platform creation
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
    genLastLine(&spriteList,spriteSize);

    genLadders(&ladderList, spriteSize);

    genHammers(&hammerList, spriteSize);
    //Player settings
    player.x= 101;
    player.y = 555;
    player.velX = 0;
    player.accX = 0;
    player.image[0] = al_load_bitmap("Sprites/marioStill.png");
    player.image[1] = al_load_bitmap("Sprites/marioRunning.png");

    player.movingR = false;
    player.movingL = false;
    player.lastR = true;
    player.hammer = false;

    player.spriteSheet = al_load_bitmap("Sprites/marioSheet.png");

    assert(player.spriteSheet != NULL);

    player.imageInd = 0;

    player.accY = GRAV;

    player.h = al_get_bitmap_height(player.image[player.imageInd])*SCALE;
    player.w = al_get_bitmap_width(player.image[player.imageInd])*SCALE;


    player.bx = player.w/2;
    player.by = player.h/2;

    //Timer and game states
    al_start_timer(timer);

    bool showingHitboxes = false;
    bool colliding = false;
    bool falling = true;
    player.climbing = false;

    int count = 0;
    int countBall = 0;
    int hammerTimer = 0;
    int moveCount = 0;

//----------------------- Game Loop ---------------------------------------------------------------------------------

    while(running){

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);


        //----------------------- Event Detection ---------------------------------------------------------------------------------
        if(allHammerCollide(&player,hammerList)){

            player.hammer = true;
            hammerTimer = 0;

        }
        //Platform collition
        if(isCollidingWithAny(&player,spriteList)){

            colliding = true;
            falling = false;

        } else {colliding = false; falling = true;};
        //Barrel collition
        if(isCollidingWithBarrels(&player,barrelList)){
            al_destroy_display(display);
            al_uninstall_keyboard();
            al_destroy_timer(timer);

            return -1;

        }
        //Ladder collition
        if(allLadderCollide(&player,ladderList)){

            player.climbing = true;
            falling = true;

        }
        else{

            player.climbing = false;
        }

        //Keyboard detection
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
                //P,K and L are used for debugging
                case ALLEGRO_KEY_P:
                    printf("Barrel 1\n");
                    createBarrel(173,173,1,&barrelList,barrelSize);
                    break;
                case ALLEGRO_KEY_K:
                    printf("Barrel 2\n");
                    createBarrel(173,173,2,&barrelList,barrelSize);
                    break;
                case ALLEGRO_KEY_L:
                    printf("Barrel 2\n");
                    createBarrel(173,173,3,&barrelList,barrelSize);
                    break;
            }
        }
        //Barrel creation from server-----------------------------------------------------------------------------------
        if(barrelCode == 1){

            createBarrel(173,173,1,&barrelList,barrelSize);
            barrelCode = 0;

        }

        if(barrelCode == 21){
            createBarrel(173,173,2,&barrelList,barrelSize);
            barrelCode = 0;

        }

        if(barrelCode == 22){
            createBarrel(width/2,173,2,&barrelList,barrelSize);
            barrelCode = 0;

        }
        if(barrelCode == 23){
            createBarrel(width-173,173,2,&barrelList,barrelSize);
            barrelCode = 0;

        }
        if(barrelCode == 31){createBarrel(173,173,3,&barrelList,barrelSize);
            barrelCode = 0;
        }
        if(barrelCode == 32){    createBarrel(width/2,173,3,&barrelList,barrelSize);
            barrelCode = 0;
        }
        if(barrelCode == 33){    createBarrel(width-173,173,3,&barrelList,barrelSize);
            barrelCode = 0;
        }
    //------------------------------------------------------------------------------------------------------------------

        //If the window is closed
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){

            running = false;

        }

        //----------------------- Updates ------------------------------------------------------------------------------
        if(key[KEY_SPACE]){
            if(!falling){
                player.jumping = true;
                player.velY -=JUMP;
                if(player.movingR){
                    player.velX += 5;
                }
                if(player.movingL){
                    player.velX -= 5;
                }
                if(player.velY > JUMP){

                    player.velY = JUMP;

                }

                player.y-=1;
            }
        }

        //If the player falls off the map
        if(player.y > height*1.15){
            al_destroy_display(display);
            al_uninstall_keyboard();
            al_destroy_timer(timer);

            return -1;

        }

        //Level win condition (reaching the princess)
        if(player.y < 133 && player.x < 295){

            al_destroy_display(display);
            al_uninstall_keyboard();
            al_destroy_timer(timer);
            msg = 1;

            return 1;

        }

        //Fireball collition (Uses the same function as the ladders)
        if(allLadderCollide(&player,fireList)){
            al_destroy_display(display);
            al_uninstall_keyboard();
            al_destroy_timer(timer);
            msg = 1;

            return -1;

        }
        //Stop the player if colliding with a platform
        if(colliding){

            player.accY = 0;

        }
        else player.accY = GRAV;

        //-------------------------------------------Movement-----------------------------------------------------------
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
        //--------------------------------------------------------------------------------------------------------------


        updatePlayer(&player);



        //----------------------- Rendering ----------------------------------------------------------------------------

        if(event.type == ALLEGRO_EVENT_TIMER){

            count +=1;
            countBall +=1;
            moveCount +=1;

            //Hammer timer
            if(player.hammer){

                hammerTimer+=1;

                if(hammerTimer == 480){

                    player.hammer = false;
                    hammerTimer = 0;

                }

            }
            //Drawing sprites, background and enemies
            al_clear_to_color(al_map_rgb_f(255,0,0));

            al_draw_scaled_bitmap(background,0,0,width,height,0,0,width*1.15,height*1.15,0);


            if(countBall > 300){

                countBall = 0;
                createFireBall(&fireList);

            }
            if(moveCount > 1){
                moveCount = 0;
                updateAllFireBalls(&player,fireList, level);

            }


            if(count >= 15){

                updateBarrelInds(barrelList);

                count = 0;

                //imageInd is used in the animation of the player and other sprites
                if(player.velX != 0 && (player.movingR || player.movingL)) {
                    (&player)->imageInd += 1;
                    (&player)->imageInd = (&player)->imageInd % 2;
                    (&player)->hammerInd += 1;
                    (&player)->hammerInd = (&player)->hammerInd%3;
                }
                //If the player isnt moving
                if(!(player.movingR || player.movingL)){

                    player.imageInd = 0;

                }


            }


            updateAllBarrels(barrelList,spriteList,level);
            drawHammers(hammerList);

            drawPlayer(&player);
            drawFireBalls(fireList);

            al_flip_display();
        }

    }
//------------Finalization----------------------------------------------------------------------------------------------

    al_destroy_display(display);
    al_uninstall_keyboard();
    al_destroy_timer(timer);


}
