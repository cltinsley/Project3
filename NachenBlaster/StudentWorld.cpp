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
}

StudentWorld::~StudentWorld() {
    cleanUp(); // cleanup() does exactly what the destructor needs to do
}

int StudentWorld::init()
{
    // Allocate and insert 30 new star objects into the world:
    for(int i=0; i<30; i++) {
        double x = randInt(0, VIEW_WIDTH-1);
        double y = randInt(0, VIEW_HEIGHT-1); // generate random coordinates
        createStar(x, y);
    }
    m_nachenBlaster = new NachenBlaster(this); // initialize the nachenblaster
    m_aliensRemaining = (4*getLevel()) + 6; // assign the number of aliens necessary to kill during this level
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_nachenBlaster->doSomething(); // let the nachenblaster do what it has to during this tick
    if(!(m_nachenBlaster->isAlive())) {
        decLives();
        return GWSTATUS_PLAYER_DIED; // return status if nachenblaster is dead
    }
    vector<Actor*>::iterator ptr; // iterator to point at our actors vector
    ptr = actors.begin();
    while(ptr!=actors.end()) {
        (*ptr)->doSomething(); // let each actor do something during their turn
        if(m_aliensRemaining==0) { // check if we finished the level after each actor does something
            return GWSTATUS_FINISHED_LEVEL;
        }
        else if(!(m_nachenBlaster->isAlive())) {
            decLives();
            return GWSTATUS_PLAYER_DIED; // return status if nachenblaster dies
        }
        if(!(*ptr)->isAlive()) { // check to make sure no actors died during this turn
            Actor* temp = *ptr;// use a temp variable to hold the dynamically allocated actor
            ptr = actors.erase(ptr); // remove the actor from the vector, update ptr
            delete temp; // delete the temp to avoid memory leaks
        }
        else { // if we didn't erase this one, then move to the next one
            ptr++;
        }
    }
    
    introduceNewObjects();
    updateText();
    
    return GWSTATUS_CONTINUE_GAME; // if we reach the end of our method then continue the game
}

void StudentWorld::cleanUp() {
    if(m_nachenBlaster != nullptr)
        delete m_nachenBlaster; // delete the dynamically allocated nachenblaster
    vector<Actor*>::iterator ptr; // will iteratre through our actor vector
    ptr = actors.begin();
    while(ptr!=actors.end()) {
        Actor* temp = *ptr; // use a temp variable to hold the dynamically allocated actor
        ptr = actors.erase(ptr); // remove the actor from the vector (ptr now points to the next one)
        delete temp; // delete the temp to avoid memory leaks
    }
}

void StudentWorld::createStar(double x, double y) {
    double size = double(randInt(1, 10))/20; // generate random size between .05 - .5
    actors.push_back(new Star(x, y, size, this)); // add the star to our list of actors
}

void StudentWorld::createExplosion(double x, double y) {
    actors.push_back(new Explosion(x, y, this));
}

void StudentWorld::createRepairGoodie(double x, double y) {
    actors.push_back(new RepairGoodie(x, y, this));
}

void StudentWorld::createTorpedoGoodie(double x, double y) {
    actors.push_back(new TorpedoGoodie(x, y, this));
}

void StudentWorld::createExtraLifeGoodie(double x, double y) {
    actors.push_back(new ExtraLifeGoodie(x, y, this));
}

void StudentWorld::createCabbage(double x, double y) {
    actors.push_back(new Cabbage(x, y, this));
}

void StudentWorld::introduceNewObjects() {
    if(randInt(1, 15) == 15) { // 1/15 chance to introduce new star
        createStar((VIEW_WIDTH-1), randInt(0, (VIEW_HEIGHT-1))); // new star on far right with random y
    }
    int numAliens = countAliens();
    int maxAliens = 4 + (getLevel()*0.5);
    if(numAliens<m_aliensRemaining && numAliens<maxAliens) {
        int s1 = 60;
        int level = getLevel();
        int s2 = 20 + (level*5);
        int s3 = 5 + (level*10);
        int s = randInt(1, (s1+s2+s3));
        if(s<=s1) {
            actors.push_back(new Smallgon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
        }
        else if(s<=(s1+s2)) {
            actors.push_back(new Smoregon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
        }
        else {
            actors.push_back(new Snagglegon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
        }
    }
}

void StudentWorld::updateText() { // creates and updates the text line displayed on top of the screen
    string gameText = "Lives: ";
    gameText += to_string(getLives()) + "  Health: ";
    int health = (m_nachenBlaster->getHP()/m_nachenBlaster->getMaxHP())*100; // formula for % health
    gameText += to_string(health)+"%  Score: ";
    gameText += to_string(getScore()) + "  Level: ";
    gameText += to_string(getLevel()) + "  Cabbages: ";
    int cabbages = (m_nachenBlaster->getCabbageEnergy()/30.0)*100; // formula for % cabbages
    gameText += to_string(cabbages)+"%  Torpedoes: ";
    gameText += to_string(m_nachenBlaster->getnumTorpedoes());
    setGameStatText(gameText);
}

int StudentWorld::countAliens() {
    int numAliens=0;
    vector<Actor*>::iterator ptr;
    ptr = actors.begin();
    while(ptr!=actors.end()) {
        if((*ptr)->isAlien()) {
            numAliens++;
        }
        ptr++;
    }
    return numAliens;
}

vector<Actor*> StudentWorld::getAliens() {
    vector<Actor*> aliens;
    vector<Actor*>::iterator ptr;
    ptr = actors.begin();
    while(ptr!=actors.end()) {
        if((*ptr)->isAlien()) {
            Actor* temp = (*ptr);
            aliens.push_back(temp);
        }
        ptr++;
    }
    return aliens;
}
