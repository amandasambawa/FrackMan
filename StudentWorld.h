// Author: Amanda Sambawa

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
class Actor;
class FrackMan;
class Dirt;

#include <string>
#include <vector>

#include <iostream>
#include <math.h>

using namespace std;

class StudentWorld : public GameWorld
{
public:
    enum Direction { none, up, down, left, right };
    
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{ }
    
    virtual ~StudentWorld();
    
    // Add an actor to the world.
    void addActor(Actor* a);
    
    // Clear a 4x4 region of dirt
    void clearDirt(int x, int y);
    
    bool checkDirt(Actor* a, int x, int y);
    
    // Checks for dirt below for boulders 
    bool checkDirtBelow(int x, int y);
    
    // Can actor move to x,y?
    bool canActorMoveTo(int x, int y) const;
    
    bool canFrackmanMoveTo(int x, int y) const;
    
    // Annoy all other actors within radius of annoyer, returning the
    // number of actors annoyed.
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);
    
    // Annoy the frackman
    bool annoyFrackman(int x, int y, int points, int radius);
    
    // Reveal all objects within radius of x,y.
    void revealAllNearbyObjects(int x, int y, int radius);
    
    // If the FrackMan is within radius of a, return a pointer to the
    // FrackMan, otherwise null.
    Actor* findNearbyFrackMan(Actor* a, int radius) const;
    
    // Creates the dirt field
    void createDirtField();
    
    virtual int init();
    
    // Updates the display text
    void updateDisplayText();
    
    void removeDeadGameObjects();
    
    void fireSquirt();
    
    void addGoldNugget();
    
    bool thePlayerCompletedTheCurrentLevel() const;
    
    int getNumProtesters() const;

    virtual int move();

    virtual void cleanUp();
    
    int getCurrentHealth() const;

private:
    vector<Actor*> m_actors;
    FrackMan* m_frackman;
    Dirt* arr[VIEW_HEIGHT][VIEW_WIDTH];
    
    bool grid[VIEW_HEIGHT][VIEW_WIDTH]; // For placing the boulders
    bool boulderGrid[VIEW_HEIGHT][VIEW_WIDTH]; // where the boulders are
    
    bool frackmanPath[VIEW_HEIGHT][VIEW_WIDTH];
    bool protesterPath[VIEW_HEIGHT][VIEW_WIDTH];
    
    int numBarrels;
    int m_ticks;
};

#endif // STUDENTWORLD_H_
