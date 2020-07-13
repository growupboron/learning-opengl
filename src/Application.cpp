#include "game.h"

int main()
{

    Game game("Learning OPenGL", 1600, 900, 4, 4, true);

    while (!game.getWindowShouldClose())
    {

        // UPDATE
        game.update();
        game.render();
    }

    // END OF PROGRAM
    return 0;
}
