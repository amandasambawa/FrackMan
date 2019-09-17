// Author: Amanda Sambawa

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// #include "StudentWorld.h"
class StudentWorld;

#include <iostream>
#include <math.h>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// Base class for all the game's objects
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : GraphObject(imageID, startX, startY, dir, size, depth), m_world(world), m_alive(true) { setVisible(true); };
    
    virtual ~Actor(){};
    
    // Get this actor's world
    StudentWorld* getWorld() const
    {
        return m_world;
    }
    
    // Is this actor alive?
    bool isAlive() const { return m_alive; }
    
    // Mark this actor as dead.
    void setDead() { m_alive = false; }
    
    // Can this actor be revealed?
    virtual bool canBeRevealed() const { return false; }
    
    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const { return true; }
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const { return false; }
    
    // Can this actor be picked up?
    virtual bool canBePickedUp() const { return false; }
    
    // Can this actor be given to a protester?
    virtual bool canBeGiven() const { return false; }
    
    // Annoy this actor.
    virtual bool annoy(unsigned int amt) { return false; }
    
    // Does this actor hunt the FrackMan?
    virtual bool huntsFrackMan() const { return false; }
    
    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const { return false; }
    
    virtual bool canGetAnnoyed() { return false; }
    
    virtual int getState() const { return 0; }
    
    virtual bool isSonar() const { return false; }
    
    virtual bool isWaterPool() const { return false; }
    
    virtual void doSomething() = 0;
    
private:
    StudentWorld* m_world;
    bool m_alive;
};

// Dirt class
class Dirt : public Actor
{
public:
    Dirt(StudentWorld* s, int x, int y) : Actor(s, IID_DIRT, x, y, right, .25, 3 ) { }
    virtual ~Dirt() {}
    
    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const { return false; }
    
    virtual void doSomething() {}
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* s, int imageID, int x, int y, Direction dir = right) : Actor(s, imageID, x, y, dir), m_annoyed(false) {}
    
    virtual ~Agent() {}
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const { return true; }
    
    virtual void doSomething() = 0;
    void setAnnoyed() { m_annoyed = true; };
    bool checkAnnoyed() const { return m_annoyed; }
    
    virtual bool canGetAnnoyed() { return true; }
    virtual bool annoy(unsigned int amt) = 0;
    
private:
    bool m_annoyed;
};

// FrackMan class
class FrackMan : public Agent
{
public:
    FrackMan(StudentWorld* s) : Agent(s, IID_PLAYER, 30, 60), m_hitPoints(10), m_waterSquirts(5), m_sonarCharge(1), m_goldNuggets(0) {}
    
    virtual ~FrackMan() {}
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const { return true; }
    
    virtual bool annoy(unsigned int amt)
    {
        m_hitPoints-= amt;
        return true;
    }
    
    int getHitPoints() const
    {
        return m_hitPoints;
    }
    
    void giveGoldNugget()
    {
        m_goldNuggets++;
    }

    int getGoldNuggets() const
    {
        return m_goldNuggets;
    }
    
    void giveSonar()
    {
        m_sonarCharge++;
    }
    
    void giveWater()
    {
        m_waterSquirts++;
    }
    
    int getSonarCharge() const
    {
        return m_sonarCharge;
    }
    
    virtual void doSomething();
    
    int getWaterSquirts() const
    {
        return m_waterSquirts;
    }
    
private:
    int m_hitPoints;
    int m_waterSquirts;
    int m_sonarCharge;
    int m_goldNuggets;
};

// Protester class
class Protester : public Agent
{
public:
    Protester(StudentWorld* s, int imageID, int x, int y, int level) : Agent(s, imageID, x, y, left), m_hitPoints(5), m_isResting(false), m_leaveOilField(false)
    {
        // N = max(50, 100 – current_level_number * 10)
        m_restingCount = fmax(50, 100 - level * 10);
    }
    virtual ~Protester() {}
    
    int getRestingCount() const { return m_restingCount; } 
    
    // Does this actor hunt the FrackMan?
    virtual bool huntsFrackMan() const { return true; }
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const { return true; }
    
    virtual void doSomething() = 0;
    
    virtual bool annoy(unsigned int amt);
    
    int getHitPoinst() const { return m_hitPoints; }
    
    bool isResting() { return m_isResting; } 
    
    void setResting() { m_isResting = true; }
    
    void setNotResting() { m_isResting = false; }
    
    void incTicks() { m_ticks++; }
    
    int getTicks() const { return m_ticks; }
    
private:
    int m_hitPoints;
    bool m_isResting;
    int m_ticks;
    bool m_leaveOilField;
    int m_restingCount;
};

// Regular Protestor class
class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* s, int x, int y, int level) : Protester(s, IID_PROTESTER, x, y, level) {}
    virtual ~RegularProtester() {}
    virtual void doSomething();
private:
};

// Hardcore Protester class
class HardCoreProtester : public Protester
{
public:
    HardCoreProtester(StudentWorld* s, int x, int y, int level) : Protester(s, IID_HARD_CORE_PROTESTER, x, y, level){}
    virtual ~HardCoreProtester() {}
    virtual void doSomething();
};

class WaterPool : public Actor
{
public:
    WaterPool(StudentWorld* s, int x, int y) : Actor(s, IID_WATER_POOL, x, y, right, 1.2, 2) {}
    virtual ~WaterPool() {}
    
    // Can this actor be picked up?
    virtual bool canBePickedUp() const { return true; }
    
    virtual void doSomething() {}

    virtual bool isWaterPool() const { return true; }
    
};

// Boulder class
class Boulders : public Actor
{
public:
    Boulders(StudentWorld* s, int x, int y) : Actor (s, IID_BOULDER, x, y, down, 1.0, 1), m_alive(true), m_state(0), m_waitingCount(30) {}
    virtual ~Boulders() {}
    
    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const { return false; }
    
    virtual void doSomething();

    virtual int getState() const { return m_state; }
    int getWaitingCount() const { return m_waitingCount; }
private:
    bool m_alive;
    int m_state;  // 0 = stable, 1 = waiting, 2 = falling, 3 = dead
    int m_waitingCount;
};

// Goodies base class
class Goodies: public Actor
{
public:
    Goodies(StudentWorld* s, int imageID, int x, int y, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Actor (s, imageID, x, y, dir, size, depth) {}
    virtual ~Goodies() {}
    
    virtual void doSomething() = 0;
    bool isActive() const { return m_active; }
    
private:
    bool m_active;
};

// Squirts class
class Squirts : public Goodies
{
public:
    Squirts(StudentWorld* s, int x, int y, Direction dir) : Goodies(s, IID_WATER_SPURT, x, y, dir), m_waitingCount(4) {}
    virtual ~Squirts() {}
    virtual void doSomething();
private:
    int m_waitingCount;
};

class SonarKit : public Goodies
{
public:
    SonarKit(StudentWorld* s, int x, int y) : Goodies(s, IID_SONAR, x, y, right, 1.0, 2) {}
    virtual ~SonarKit() {}
    
    // Can this actor be picked up?
    virtual bool canBePickedUp() const { return true; }
    
    virtual bool isSonar() const { return true; }
    
    virtual void doSomething() {}
};

// Gold nugget that is able to be picked up by Frackman
class GoldNuggetForFrackMan : public Goodies
{
public:
    GoldNuggetForFrackMan(StudentWorld* s, int x, int y) : Goodies(s, IID_GOLD, x, y, right, 1.0, 2) { setVisible(false); }
    virtual ~GoldNuggetForFrackMan() {}
    
    virtual bool canBeRevealed() const { return true; }
    
    // Can this actor be picked up?
    virtual bool canBePickedUp() const { return true; }
    
    // Can this actor be given to a protester?
    virtual bool canBeGiven() const { return true; }
    
    virtual void doSomething() {}
};

// Gold nugget that is able to be picked up by Protester
class GoldNuggetForProtester : public Goodies
{
public:
    GoldNuggetForProtester(StudentWorld* s, int x, int y) : Goodies(s, IID_GOLD, x, y, right, 1.0, 2) {}
    virtual ~GoldNuggetForProtester() {}
    
    virtual void doSomething() {}
};

// Oil Barrel class 
class BarrelOfOil : public Goodies
{
public:
    BarrelOfOil(StudentWorld* s, int x, int y) : Goodies(s, IID_BARREL, x, y, right, 1.0, 2) { setVisible(false); }
    virtual ~BarrelOfOil() {}
    
    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const { return true; }
    
    // Can this actor be picked up?
    virtual bool canBePickedUp() const { return true; }
    
    virtual bool canBeRevealed() const { return true; }
    
    virtual void doSomething() {}
};


#endif // ACTOR_H_
