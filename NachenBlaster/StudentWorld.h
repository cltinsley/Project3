#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class NachenBlaster;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void introduceNewObjects();
    void updateText();
    int countAliens();
    void createStar(double x, double y);
    void createExplosion(double x, double y);
    void createRepairGoodie(double x, double y);
    void createTorpedoGoodie(double x, double y);
    void createExtraLifeGoodie(double x, double y);
    void createCabbage(double x, double y);
    std::vector<Actor*> getAliens();
    
    StudentWorld* getWorld() { return this; }
    NachenBlaster* getNachenBlaster() { return m_nachenBlaster; }
    void decreaseRemainingAliens() { m_aliensRemaining--; }

private:
    std::vector<Actor*> actors; // vector of each actor that exists within the screen
    NachenBlaster* m_nachenBlaster = nullptr; // pointer to our NachenBlaster
    int m_aliensRemaining;
};

#endif // STUDENTWORLD_H_
