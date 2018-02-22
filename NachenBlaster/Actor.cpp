#include "Actor.h"
#include <math.h>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor Implementations --------------------------------------------
bool Actor::didCollide(Actor& other) {
    double x1 = getX();
    double y1 = getY();
    double r1 = getRadius();
    double x2 = other.getX();
    double y2 = other.getY();
    double r2 = other.getRadius();
    double distance = pow((pow((x1-x2), 2)+pow((y1-y2), 2)), 0.5);
    if(distance < (.75*(r1+r2))) {
        return true;
    }
    return false;
}

// Star Implementations --------------------------------------------
void Star::doSomething() {
    int x = getX() - 1;
    if(x<-1) { // if the star is off the screen, mark it as dead and don't move it
        setDead();
        return;
    }
    moveTo(x, getY()); // move one pixel to the left
}

// Explosion Implementations ----------------------------------------
void Explosion::doSomething() {
    double currentSize = getSize();
    setSize(currentSize*1.5); // increase size
    m_age++;
    if(m_age>=4) { // destroy explosion 4 ticks after creation
        setDead();
    }
}

Explosion::Explosion(double x, double y, StudentWorld* world)
: Actor(IID_EXPLOSION, x, y, 0, 1, 0, world) {
    m_age = 0;
    //m_world = getWorld();
}

// Ship Implementations ----------------------------------------------
Ship::Ship(int imageID, double x, double y, int direction, double size, int depth, StudentWorld* world, int maxHP) : Actor(imageID, x, y, direction, size, depth, world)  {
    m_hp = maxHP; // when we initialize the ship, its hp is at max
    m_maxHP = maxHP;
}

void Ship::decreaseHP(double amount) {
    m_hp -= amount;
    if(m_hp <= 0) {
        setDead();
    }
}

void Ship::increaseHP(double amount) {
    m_hp += amount;
    if(m_hp>m_maxHP) {
        m_hp = m_maxHP; // ensure we don't go above the allowed max hp
    }
}

void Ship::setHP(double maxHP) { // allows lower classes to change the HP of the ship
    m_hp = maxHP;
    m_maxHP = maxHP;
}

// NACHENBLASTER Implementations -------------------------------------
NachenBlaster::~NachenBlaster() {} // does nothing right now, will update

void NachenBlaster::doSomething() {
    if(!isAlive() || getHP()<=0) {
        setDead();
        return; // return immediately if the nachenblaster is dead (may he rest in peace)
    }
    if(cabbageEnergy<30)
        cabbageEnergy++; // nachenblaster gets one cabbage energy point per tick (assuming its not full)
    
    int key; // this variable will store the key pressed by the user
    if(getWorld()->getKey(key)) {
        switch(key) {
            case KEY_PRESS_UP : {
                double newY = getY() + 6; // new space in upward direction
                if(newY < VIEW_HEIGHT) { // this is a valid move
                    moveTo(getX(), newY);
                }
            }
                break;
            case KEY_PRESS_DOWN : {
                double newY = getY() - 6; // new space downward
                if(newY >= 0) { // this is a valid move
                    moveTo(getX(), newY);
                }
            }
                break;
            case KEY_PRESS_LEFT : {
                double newX = getX() - 6; // new space to the left
                if(newX >= 0) { // this is a valid move
                    moveTo(newX, getY());
                }
            }
                break;
            case KEY_PRESS_RIGHT : {
                double newX = getX() + 6; // new space to the right
                if(newX < VIEW_WIDTH) { // this is a valid move
                    moveTo(newX, getY());
                }
            }
                break;
            case KEY_PRESS_SPACE : {
                if(cabbageEnergy>=5) {
                    cabbageEnergy -= 5;
                }
            }
        }
    }
}

// Alien Implementations --------------------------------------------------
Alien::Alien(int imageID, double x, double y, double size, StudentWorld* world, int maxHP, double travelSpeed, double flightPlan, int planLength) : Ship(imageID, x, y, 0, size, 1, world, maxHP) {
    m_travelSpeed = travelSpeed;
    m_flightPlan = flightPlan;
    m_planLength = planLength;
}

void Alien::setNewPlan() {
    double y = getY();
    if(y >= (VIEW_HEIGHT-1)) {
        m_flightPlan = -1;
    }
    else if(y<=0) {
        m_flightPlan = 1;
    }
    else { // this is the case where the flight plan has expired
        m_flightPlan = randInt(-1, 1); // gives a random choice of -1, 0, 1
    }
    m_planLength = randInt(1, 32); // set the plan length to a random number between 1 and 32
}

bool Alien::checkCollisions(int score, int damage) { // returns true when the alien is killed from the collision
    StudentWorld* world = getWorld();
    NachenBlaster* nb = world->getNachenBlaster();
    if(didCollide(*nb)) { // checks if the given alien collided with the nachenblaster
        nb->decreaseHP(damage); // damage the nachenblaster
        setDead(); // kill the alien
        world->increaseScore(score); // increase player score by 250
        world->decreaseRemainingAliens(); // decrease remaining aliens
        world->playSound(SOUND_DEATH);
        world->createExplosion(getX(), getY()); // introduce new explosion where the ship was
        return true;
    }
    return false;
}

// Smallgon Implementations -----------------------------------------------
Smallgon::Smallgon(double x, double y, StudentWorld* world) : Alien(IID_SMALLGON, x, y, 1.5, world, -1, 2, 0, 0) {
    int level = world->getLevel();
    double hp = 5 * (1 + (level - 1)*.1); // hp formula for smallgon
    setHP(hp); // set the ship's max hp according to the formula
}

void Smallgon::doSomething() {
    // 1. check if dead, if so return immediately
    if(getHP() <= 0 || !isAlive()) {
        setDead();
        return;
    }
    
    double x = getX();
    if(x<0) { // 2. check if smallgon has flown off the screen
        setDead();
        return;
    }
    
    // 3. Check for collision with nachenblaster/nachenblaster's projectiles
    if(checkCollisions(250, 5)) { // smallgons are worth 250 points and do 5 damage points if they collide with the nachenblaster
        return; // we don't do anything else because this ship is dead
    }
    
    // 4. check to see if our plan has expired or if we need a new one because of our y coordinate
    if(getPlanLength()<=0 || getY()>=(VIEW_HEIGHT-1) || getY()<=0)
        setNewPlan();
    
    // 5. see if you gotta shoot the nachenblaster ***NEED TO IMPLEMENT***
    
    // 6. Try to move:
    double speed = getSpeed(); // the current flight speed
    double plan = getPlan();  // the current flight plan
    double newX = x - speed; // we move left every time
    double newY = getY() + speed*plan; // plan tells us if we move up, down or neither using 1, -1, or 0
    moveTo(newX, newY);
    decrementPlanLength(); // shorten the plan by 1 each time it moves
    
    // 7. repeat step 3, see if the move resulted in a collision
    checkCollisions(250, 5);
}

// Smoregon Implementations -----------------------------------------------
Smoregon::Smoregon(double x, double y, StudentWorld* world)
: Alien(IID_SMOREGON, x, y, 1.5, world, -1, 2.0, 0, 0) {
    int level = world->getLevel();
    double hp = 5 * (1 + (level - 1)*.1); // hp formula for smallgon
    setHP(hp); // set the ship's max hp according to the formula
}

void Smoregon::doSomething() {
    // 1. check if dead, if so return immediately
    if(getHP() <= 0 || !isAlive()) {
        setDead();
        return;
    }
    
    double x = getX();
    if(x<0) { // 2. check if smoregon has flown off the screen
        setDead();
        return;
    }
    
    // 3. Check for collision with nachenblaster/nachenblaster's projectiles
    if(checkCollisions(250, 5)) { // smoregons are worth 250 points and do 5 damage points if they collide with the nachenblaster
        dropGoodies();
        return; // we don't do anything else because this ship is dead
    }
    
    // 4. check to see if our plan has expired or if we need a new one because of our y coordinate
    if(getPlanLength()<=0 || getY()>=(VIEW_HEIGHT-1) || getY()<=0)
        setNewPlan();
    
    // 5. see if you gotta shoot the nachenblaster ***NEED TO IMPLEMENT***
    
    // 6. Try to move:
    double speed = getSpeed(); // the current flight speed
    double plan = getPlan();  // the current flight plan
    double newX = x - speed; // we move left every time
    double newY = getY() + speed*plan; // plan tells us if we move up, down or neither using 1, -1, or 0
    moveTo(newX, newY);
    decrementPlanLength(); // shorten the plan by 1 each time it moves
    
    // 7. repeat step 3, see if the move resulted in a collision
    if(checkCollisions(250, 5)) {
        dropGoodies();
    }
}

void Smoregon::dropGoodies() {
    // does nothing now, wait till goodies are implemented
}

// Goodie Implementations ------------------------------------------------------
bool Goodie::pickUp() {
    StudentWorld* world = getWorld();
    NachenBlaster* nb = world->getNachenBlaster();
    if(didCollide(*nb)) {
        world->increaseScore(100); // player gets 100 points for picking up a goodie
        setDead();
        world->playSound(SOUND_GOODIE);
        return true;
    }
    return false;
}

// execute the majority of the doSomething method for goodies, return true if picked up
bool Goodie::makeMove() {
    if(!isAlive()) {
        return false;
    }
    double x = getX();
    double y = getY();
    if(x<0 || y<0) {
        setDead();
        return false;
    }
    if(pickUp()) {
        return true;
    }
    moveTo(x-.75, y-.75);
    if(pickUp()) {
        return true;
    }
    return false;
}

// Repair Goodie Implementations -----------------------------------------------
void RepairGoodie::doSomething() {
    if(makeMove()) {
        getWorld()->getNachenBlaster()->increaseHP(10);
    }
}

// Torpedo Goodie Implementations -----------------------------------------------
void TorpedoGoodie::doSomething() {
    if(makeMove()) {
        getWorld()->getNachenBlaster()->increaseNumTorpedoes(5);
    }
}
