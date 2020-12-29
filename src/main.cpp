#include "Game.h"

int main(int argc, char** argv)
{
    const int fps = 30;
    const int frame_delay = 1000 / fps;
    const int screen_width = 736;
    const int screen_height = 896;

    Uint32 frame_start;
    int frame_time;

    Game game = Game("Pacman Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height);

    while (game.running())
    {
        frame_start = SDL_GetTicks();

        game.handleInput();
        game.update();
        game.render();

        frame_time = SDL_GetTicks() - frame_start;

        if (frame_delay > frame_time)
            SDL_Delay(frame_delay - frame_time);
    }

    return 0;
}
