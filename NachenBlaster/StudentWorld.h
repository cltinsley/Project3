#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
    std::vector<Actor*> actors; // vector of each actor that exists within the screen
    Actor* m_nachenBlaster; // pointer to our NachenBlaster
};

#endif // STUDENTWORLD_H_
