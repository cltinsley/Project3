#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"

class StudentWorld;

class Actor : public GraphObject {
public:
    // Actor's constructor sets alive to true and initializes graph object
    Actor(int i_ID, double x, double y, int dir, double size, int depth, StudentWorld* world)
    : GraphObject(i_ID, x, y, dir, size, depth) {
        giveLife();
        m_world = world;
    }
    
    virtual ~Actor() {}
    virtual void doSomething() = 0;
    
    StudentWorld* getWorld() { return m_world; } // returns the world that the actor is in
    bool isAlive() { return alive; }
    void giveLife() { alive = true; }
    void setDead() { alive = false; }
    bool didCollide(Actor& other); //returns true when the actor is within collision distance from other
    
private:
    bool alive; // keeps track of whether or not the given actor is alive (should it be displayed)
    StudentWorld* m_world;
    
};

class Star : public Actor {
public:
    // star's constructor takes an input of x,y coordinates and the size and uses the default star parameters for the super constructor
    Star(double x, double y, double size, StudentWorld* world) : Actor(IID_STAR, x, y, 0, size, 3, world) {}
    virtual ~Star() {}
    virtual void doSomething();
};

class Explosion : public Actor {
public:
    Explosion(double x, double y, StudentWorld* world);
    virtual ~Explosion() {}
    void doSomething();
private:
    int m_age; // keeps track of when to remove the explosion from the screen
};

class NachenBlaster : public Actor {
public:
    NachenBlaster(StudentWorld* world) : Actor(IID_NACHENBLASTER, 0, 128, 0, 1, 0, world) {
        m_hp = 50;
        cabbageEnergy = 30;
    }
    virtual ~NachenBlaster();
    void doSomething();
    
    int getHP() { return m_hp; } // returns the nachenblaster's current hp
    void decreaseHP(double amount) { // dectreases the hp by the given amount, sets it to dead if necessary
        m_hp -= amount;
        if(m_hp<=0) {
            setDead();
        }
    }
    
    
private:
    int m_hp; // variable to store hit points
    int cabbageEnergy; // this is my favorite variable name (stores cabbage energy, whatever that is)    
};



// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
