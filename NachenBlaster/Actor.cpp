#include "Actor.h"
#include "StudentWorld.h"
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
    moveTo(x, getY()); // move one pixel to the left
    if(x<=-1) { // if the star is off the screen, mark it as dead
        setDead();
    }
}




