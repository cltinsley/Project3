#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    StudentWorld* getWorld() { return this; }

private:
    std::vector<Actor*> actors; // vector of each actor that exists within the screen
    Actor* m_nachenBlaster; // pointer to our NachenBlaster
    int level;
    int aliensRemaining;
};

#endif // STUDENTWORLD_H_
