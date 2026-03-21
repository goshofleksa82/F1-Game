#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Game.h"

int main(int argc, char* argv[]){
    Game game;

if(!game.Init()){
    return 1;
}

game.Run();
game.Shutdown();
return 0;
}