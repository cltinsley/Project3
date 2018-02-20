#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

//class Actor;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    level = 1;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    // Allocate and insert 30 new star objects into the world:
    for(int i=0; i<30; i++) {
        double size = double(randInt(1, 10))/20; // generate random size between .05 - .5
        double x = randInt(0, VIEW_WIDTH-1);
        double y = randInt(0, VIEW_HEIGHT-1); // generate random coordinates
        actors.push_back(new Star(x, y, size, this)); // add the star to our list of actors
    }
    m_nachenBlaster = new NachenBlaster(this); // initialize the nachenblaster
    aliensRemaining = (4*level) + 6; // assign the number of aliens necessary to kill during this level
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_nachenBlaster->doSomething(); // let the nachenblaster do what it has to during this turn
    if(!(m_nachenBlaster->isAlive())) {
        return GWSTATUS_PLAYER_DIED; // return appropriate status if nachenblaster dies during his turn
    }
    vector<Actor*>::iterator ptr; // iterator to point at our actors vector
    ptr = actors.begin();
    while(ptr!=actors.end()) {
        (*ptr)->doSomething(); // let each actor do something during their turn
        if(!(*ptr)->isAlive()) { // check to make sure no actors died during this turn
            Actor* temp = *ptr;// use a temp variable to hold the dynamically allocated actor
            ptr = actors.erase(ptr); // remove the actor from the vector, update ptr
            delete temp; // delete the temp to avoid memory leaks
        }
        else { // if we didn't erase this one, then move to the next one
            ptr++;
        }
    }
    return GWSTATUS_CONTINUE_GAME; // if we reach the end of our method then continue the game
}

void StudentWorld::cleanUp() {
    delete m_nachenBlaster; // delete the dynamically allocated nachenblaster
    vector<Actor*>::iterator ptr; // will iteratre through our actor vector
    ptr = actors.begin();
    while(ptr!=actors.end()) {
        Actor* temp = *ptr; // use a temp variable to hold the dynamically allocated actor
        ptr = actors.erase(ptr); // remove the actor from the vector (ptr now points to the next one)
        delete temp; // delete the temp to avoid memory leaks
    }
}

