#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
}

int StudentWorld::init()
{
    /* Responsibilities of this method
     1. Initialize the data structures used to keep track of your game’s world.
     2. Allocate and insert a valid NachenBlaster object into the game world.
     3. Allocate and insert star objects into the game world. DONE
     */
    
    // Allocate and insert 30 new star objects into the world:
    for(int i=0; i<30; i++) {
        double size = randInt(1, 10)/20;
        double x = randInt(0, VIEW_WIDTH-1);
        double y = randInt(0, VIEW_HEIGHT-1);
        actors.push_back(new Star(x, y, size));
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
}
