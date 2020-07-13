#include "game.h"

int main()
{

    Game game("Learning OPenGL", 640, 480, 4, 5, true);

    while (!game.getWindowShouldClose())
    {

        // UPDATE
        game.update();
        game.render();
    }

    // END OF PROGRAM
    return 0;
}
