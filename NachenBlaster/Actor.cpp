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

// Explosion Implementations ---------------------------------------
void Explosion::doSomething() {
    double currentSize = getSize();
    setSize(currentSize*1.5); // increase size
    m_age++;
    if(m_age>=4) { // destroy explosion 4 ticks after creation
        setDead();
    }
}

// NACHENBLASTER Implementations ----------------------------------
Explosion::Explosion(double x, double y, StudentWorld* world)
: Actor(IID_EXPLOSION, x, y, 0, 1, 0, world) {
    m_age = 0;
    //m_world = getWorld();
}

NachenBlaster::~NachenBlaster() {} // does nothing right now, will update

void NachenBlaster::doSomething() {
    if(!isAlive())
        return; // return immediately if the nachenblaster is dead (may he rest in peace)
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
        }
    }
}




