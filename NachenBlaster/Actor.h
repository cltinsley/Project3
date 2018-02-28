#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>

class StudentWorld;

const int SMALL       = 1;
const int SMORE       = 2;
const int SNAGGLE     = 3;

// ACTOR definition ----------------------------------------------------------------------------
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
    virtual void decreaseHP(double amount) {} // by default actors have no HP so this does nothing
    virtual bool canCollide() { return false; } // by default, actors cannot collide with each other
    virtual bool isAlien() { return false; } // by default, actors are not aliens
    
    
    StudentWorld* getWorld() { return m_world; } // returns the world that the actor is in
    bool isAlive() { return m_alive; }
    void giveLife() { m_alive = true; }
    void setDead() { m_alive = false; }
    bool didCollide(Actor& other); //returns true when the actor is within collision distance from other
    
private:
    bool m_alive; // keeps track of whether or not the given actor is alive (should it be displayed)
    StudentWorld* m_world; // pointer to the world that the actor exists in
    
};


// STAR definition ----------------------------------------------------------------------------
class Star : public Actor {
public:
    // star's constructor takes an input of x,y coordinates and the size and uses the default star parameters for the super constructor
    Star(double x, double y, double size, StudentWorld* world) : Actor(IID_STAR, x, y, 0, size, 3, world) {}
    virtual ~Star() {}
    virtual void doSomething();
};


// EXPLOSION definition ----------------------------------------------------------------------------
class Explosion : public Actor {
public:
    Explosion(double x, double y, StudentWorld* world);
    virtual ~Explosion() {}
    void doSomething();
private:
    int m_age; // keeps track of when to remove the explosion from the screen
};


// SHIP definition ----------------------------------------------------------------------------
class Ship : public Actor {
public:
    Ship(int imageID, double x, double y, int direction, double size, int depth, StudentWorld* world, int maxHP);
    virtual ~Ship() {}
    virtual void doSomething() = 0; // different ships do different things
    
    virtual void kill() {}
    virtual bool canCollide() { return true; } // ships can collide with each other and other objects
    double getHP() { return m_hp; } // returns ship's current hp
    double getMaxHP() { return m_maxHP; }
    void setHP(double maxHP);
    void decreaseHP(double amount);
    void increaseHP(double amount);
    
private:
    double m_maxHP; // the max hit points of a given ship
    double m_hp; // the ship's current hit points
};


// NACHENBLASTER definition ----------------------------------------------------------------------------
class NachenBlaster : public Ship {
public:
    NachenBlaster(StudentWorld* world) : Ship(IID_NACHENBLASTER, 0, 128, 0, 1, 0, world, 50) {
        cabbageEnergy = 30;
        numTorpedoes = 0;
    }
    virtual ~NachenBlaster();
    virtual void doSomething();
    int getCabbageEnergy() { return cabbageEnergy; }
    int getnumTorpedoes() { return numTorpedoes; }
    void increaseNumTorpedoes(int amount) { numTorpedoes += amount; }
    
private:
    int cabbageEnergy; // this is my favorite variable name (stores cabbage energy, whatever that is)
    int numTorpedoes;
};

// ALIEN definition ----------------------------------------------------------------------------
class Alien : public Ship {
public:
    Alien(int imageID, double x, double y, double size, StudentWorld* world, int maxHP, double travelSpeed, double flightPlan, int planLength, int score);
    
    virtual void kill();
    bool mostOfDoSomething(int type); // handles the common parts of dosomething for aliens, returns true if alien dies
    double getSpeed() { return m_travelSpeed; }
    double getPlan() { return m_flightPlan; }
    int getPlanLength() { return m_planLength; }
    void decrementPlanLength() { m_planLength--; }
    void setPlan(double newPlan) { m_flightPlan = newPlan; }
    virtual bool isAlien() { return true; }
    virtual void setNewPlan(int type);
    bool checkCollisions(int score, int damage); // returns true when alien dies as a result of a collision, increases the score and damage to the nachenblaster appropriately
    
private:
    double m_travelSpeed; // keeps track of the travel speed
    double m_flightPlan; // keeps track of the flight plan
    int m_planLength; // the length of the flight plan
    int m_points; // keeps track of the points awarded opon death
};


// SMALLGON definition ----------------------------------------------------------------------------
class Smallgon : public Alien {
public:
    Smallgon(double x, double y, StudentWorld* world);
    virtual void doSomething();
};

// SMOREGON definition ----------------------------------------------------------------------------
class Smoregon : public Alien {
public:
    Smoregon(double x, double y, StudentWorld* world);
    virtual void doSomething();
    void dropGoodies();
};

// SMOREGON definition ----------------------------------------------------------------------------
class Snagglegon : public Alien {
public:
    Snagglegon(double x, double y, StudentWorld* world);
    virtual void doSomething();
    void dropLifeGoodie();
};


// GOODIE definition ------------------------------------------------------------------------------
class Goodie : public Actor {
public:
    Goodie(int imageID, double x, double y, StudentWorld* world) :
    Actor(imageID, x, y, 0, .5, 1, world) { }
    bool pickUp(); // check if the nachenblaster picks up the goodie and handle the common responses
    bool makeMove(); // executes most of the code for do something, returns true if the goodie gets picked up
    
};

// REPAIR GOODIE definition -----------------------------------------------------------------------
class RepairGoodie : public Goodie {
public:
    RepairGoodie(double x, double y, StudentWorld* world) :
    Goodie(IID_REPAIR_GOODIE, x, y, world) { }
    virtual void doSomething();
};

// TORPEDO GOODIE definition -----------------------------------------------------------------------
class TorpedoGoodie : public Goodie {
public:
    TorpedoGoodie(double x, double y, StudentWorld* world) :
    Goodie(IID_TORPEDO_GOODIE, x, y, world) { }
    virtual void doSomething();
};

// EXTRA LIFE GOODIE definition -----------------------------------------------------------------------
class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(double x, double y, StudentWorld* world) :
    Goodie(IID_LIFE_GOODIE, x, y, world) { }
    virtual void doSomething();
};

// PROJECTILE definition -----------------------------------------------------------------------
class Projectile : public Actor {
public:
    Projectile(int imageID, double x, double y, StudentWorld* world, int moveDist, int dir, int damage);
    virtual void doSomething();
    bool checkCollision(int damage);
private:
    int m_moveDist; // holds the number of pixels the projectile moves per turn (negative if it goes left)
    int m_damage; // stores how much damage the projectile does
};

// CABBAGE definition -----------------------------------------------------------------------
class Cabbage : public Projectile {
public:
    Cabbage(double x, double y, StudentWorld* world) :
    Projectile(IID_CABBAGE, x, y, world, 8, 0, 2) { }
};

// TURNIP definition -----------------------------------------------------------------------
class Turnip : public Projectile {
public:
    Turnip(double x, double y, StudentWorld* world) :
    Projectile(IID_TURNIP, x, y, world, -6, 0, 2) { }
};

// TORPEDO definition -----------------------------------------------------------------------
class Torpedo : public Projectile {
public:
    Torpedo(double x, double y, int moveSpeed, int direction, StudentWorld* world) :
    Projectile(IID_TORPEDO, x, y, world, moveSpeed, direction, 8) { }
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
