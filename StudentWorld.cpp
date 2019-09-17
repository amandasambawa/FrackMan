// Author: Amanda Sambawa

#include "StudentWorld.h"
#include "GameController.h"
#include "Actor.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <random>
#include <math.h>

#include <cstdlib>
#include <ctime>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld()
{
    delete m_frackman;
    for (int i = 0; i < VIEW_WIDTH; i++)
    {
        for (int j = 0; j < VIEW_HEIGHT; j++)
        {
            if (arr[i][j] != nullptr)
                delete arr[i][j];
        }
    }
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        delete m_actors[i];
    }
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}

void StudentWorld::clearDirt(int x, int y)
{
    for (int j = 0; j < 4; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            int temp_x = x + j;
            int temp_y = y + k;
            if (temp_y < 60 && (temp_x < 30 || temp_x > 33))
            {
                if (arr[temp_x][temp_y] != nullptr)
                {
                    delete arr[temp_x][temp_y];
                    arr[temp_x][temp_y] = nullptr;
                }
            }
            else if (temp_x >= 30 && temp_x <= 33 && temp_y <= 3)
            {
                if (arr[temp_x][temp_y] != nullptr)
                {
                    delete arr[temp_x][temp_y];
                    arr[temp_x][temp_y] = nullptr;
                }
            }
        }
    }
}

int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

void StudentWorld::createDirtField()
{
    // Create the field with dirt
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            //arr[i][j] = new Dirt(i,j);
            if (i < 30 || i > 33 || j < 4)
                arr[i][j] = new Dirt(this, i, j);
            else
                arr[i][j] = nullptr;
            //arr[i][j]->setVisible(false);
        }
    }
}

int StudentWorld::init()
{
    m_ticks = 0;
    
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    
    // Make sure the vector is clear of previous actors
    while (it != m_actors.end())
    {
        delete *it;
        it = m_actors.erase(it);
    }
    
    // Create and display the Frackman
    m_frackman = new FrackMan(this);
    
    createDirtField();
    
    // Set all elements of grid to false (for the population of boulders)
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            grid[i][j] = false;
        }
    }
    
    // Set all elements of the boulder grid to false
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            boulderGrid[i][j] = false;
        }
    }
    
    // Set all elements of the frackman path to false
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            frackmanPath[i][j] = false;
        }
    }
    
    // b boulders in each level:
    int b = fmin(getLevel()/2 + 2, 6);
    
    // Insert the boulders
    for (int i = 0; i < b; i++)
    {
        int t_x = randInt(0, 60);
        int t_y = randInt(20, 56);
        
        while (t_x >= 27 && t_x <= 33)
        {
            t_x = randInt(0, 60);
        }
        
        if (grid[t_y][t_x] == false) // grid[row][column]
        {
            grid[t_y][t_x] = true;

            for (int y = 20; y <= 56; y++)
            {
                for (int x = 0; x <= 60; x++)
                {
                    if ((sqrt(((t_x - x) * (t_x - x)) + ((t_y - y) * (t_y - y)))) <= 6)
                    {
                        grid[y][x] = true;
                    }
                }
            }
            
            m_actors.push_back(new Boulders(this, t_x, t_y));
            
            clearDirt(t_x, t_y);
        }
        else
        {
            while (grid[t_y][t_x] == true)
            {
                t_x = randInt(0, 60);
                t_y = randInt(20, 56);
                
                while (t_x >= 27 && t_x <= 33)
                {
                    t_x = randInt(0, 60);
                }
            }
            
            grid[t_y][t_x] = true;
            
            for (int y = 20; y <= 56; y++)
            {
                for (int x = 0; x <= 60; x++)
                {
                    if ((sqrt(((t_x - x) * (t_x - x)) + ((t_y - y) * (t_y - y)))) <= 6)
                        grid[y][x] = true;
                }
            }
            
            m_actors.push_back(new Boulders(this, t_x, t_y));
            
            clearDirt(t_x, t_y);
        }
    }
    
    // Set boulder coordinates to true in boulderGrid
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (!(m_actors[i]->canActorsPassThroughMe())) // if the actor is a boulder
        {
            int xCoord = m_actors[i]->getX();
            int yCoord = m_actors[i]->getY();
            
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    boulderGrid[xCoord+j][yCoord+k] = true;
                }
            }
        }
    }

    // g gold nuggets in each level:
    int g = fmax(5 - getLevel()/2, 2);

    for (int i = 0; i < g; i++)
    {
        int t_x = randInt(0, 60);
        int t_y = randInt(20, 56);
        
        while (t_x >= 27 && t_x <= 33)
        {
            t_x = randInt(0, 60);
        }
        
        if (grid[t_y][t_x] == false) // grid[row][column]
        {
            grid[t_y][t_x] = true;
            
            for (int y = 20; y <= 56; y++)
            {
                for (int x = 0; x <= 60; x++)
                {
                    if ((sqrt((t_x - x) * (t_x - x) + (t_y - y) * (t_y - y))) <= 6)
                        grid[y][x] = true;
                }
            }
            m_actors.push_back(new GoldNuggetForFrackMan(this, t_x, t_y));
        }
        else
        {
            while (grid[t_y][t_x] == true)
            {
                t_x = randInt(0, 60);
                t_y = randInt(20, 56);
                
                while (t_x >= 27 && t_x <= 33)
                {
                    t_x = randInt(0, 60);
                }
            }
            
            grid[t_y][t_x] = true;
            
            for (int y = 20; y <= 56; y++)
            {
                for (int x = 0; x <= 60; x++)
                {
                    if ((sqrt(t_x - x) * (t_x - x) + (t_y - y) * (t_y - y)) <= 6)
                        grid[y][x] = true;
                }
            }
            m_actors.push_back(new GoldNuggetForFrackMan(this, t_x, t_y));
        }
    }
    
    // l barrels of oil in each level:
    numBarrels = fmin(2 + getLevel(), 20);

    for (int i = 0; i < numBarrels; i++)
    {
        int t_x = randInt(0, 60);
        int t_y = randInt(20, 56);
        
        while (t_x >= 27 && t_x <= 33)
        {
            t_x = randInt(0, 60);
        }
        
        if (grid[t_y][t_x] == false) // grid[row][column]
        {
            grid[t_y][t_x] = true;
            
            for (int y = 20; y <= 56; y++)
            {
                for (int x = 0; x <= 60; x++)
                {
                    if ((sqrt((t_x - x) * (t_x - x) + (t_y - y) * (t_y - y))) <= 6)
                        grid[y][x] = true;
                }
            }
            
            m_actors.push_back(new BarrelOfOil(this, t_x, t_y));
        }
        else
        {
            while (grid[t_y][t_x] == true)
            {
                t_x = randInt(0, 60);
                t_y = randInt(20, 56);
                
                while (t_x >= 27 && t_x <= 33)
                {
                    t_x = randInt(0, 60);
                }
            }
            
            grid[t_y][t_x] = true;
            
            for (int y = 20; y <= 56; y++)
            {
                for (int x = 0; x <= 60; x++)
                {
                    if ((sqrt(t_x - x) * (t_x - x) + (t_y - y) * (t_y - y)) <= 6)
                        grid[y][x] = true;
                }
            }
            
            m_actors.push_back(new BarrelOfOil(this, t_x, t_y));
            
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

//bool StudentWorld::isDirtOrBoulder()
/**
bool StudentWorld::checkDirt(Actor* a, int x, int y)
{
    Direction d = a->getDirection();
    
}
*/
 
bool StudentWorld::checkDirtBelow(int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        if (arr[x+i][y-1] != nullptr)
        {
            return false;
        }
    }
    return true;
}

// Can actor move to x,y?
bool StudentWorld::canFrackmanMoveTo(int x, int y) const
{
    
    if (boulderGrid[x][y] == false)
    {
        return true;
    }
    
    return false;
}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(int x, int y) const
{
    
    if (boulderGrid[x][y] == false && arr[x][y] == nullptr)
    {
        return true;
    }
    
    return false;
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius)
{
    int annoyer_x = annoyer->getX()+2;
    int annoyer_y = annoyer->getY()+2;
    
    int numAnnoyed = 0;
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->huntsFrackMan())
        {
            if ((sqrt((annoyer_x - m_actors[i]->getX()) * (annoyer_x - m_actors[i]->getX()) + (annoyer_y - m_actors[i]->getY()) * (annoyer_y - m_actors[i]->getY()))) <= radius)
            {
                m_actors[i]->annoy(points);
                numAnnoyed++;
                //GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
            }
        }
    }
    
    return numAnnoyed;
}

bool StudentWorld::annoyFrackman(int x, int y, int points, int radius)
{
    int f_x = m_frackman->getX();
    int f_y = m_frackman->getY();
    
    
    if ((sqrt((x - f_x) * (x - f_x) + (y - f_y) * (y - f_y))) <= radius)
    {
            m_frackman->annoy(points);
            return true;
    }
    return false;
}

// Reveal all objects within radius of x,y.
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->canBeRevealed())
        {
            int t_x = m_actors[i]->getX(); // bottom left coord
            int t_y = m_actors[i]->getY(); // bottom left coord
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    if ((sqrt((x - (t_x + k)) * (x - (t_x+k)) + (y - (t_y+j)) * (y - (t_y+j)))) <= radius)
                        m_actors[i]->setVisible(true);
                }
            }
        
        }
    }
}

// If the FrackMan is within radius of a, return a pointer to the
// FrackMan, otherwise null.
Actor* StudentWorld::findNearbyFrackMan(Actor* a, int radius) const
{
    int a_x = a->getX();
    int a_y = a->getY();
    
    int frack_x = m_frackman->getX();
    int frack_y = m_frackman->getY();
    
   if ((sqrt((a_x - frack_x) * (a_x - frack_x) + (a_y - frack_y) * (a_y - frack_y))) <= radius)
       return m_frackman;
    
    return nullptr;
}

int StudentWorld::getCurrentHealth() const
{
    return m_frackman->getHitPoints();
}

void StudentWorld::updateDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    
    ostringstream ss;
    ss << setw(8) << setfill('0') << score;
    string scr = ss.str();
    
    ss.str("");
    ss << setw(2) << setfill(' ') << level;
    string lvl = ss.str();
    
    ss.str("");
    ss << m_frackman->getWaterSquirts();
    string water = ss.str();
    
    ss.str("");
    ss << m_frackman->getHitPoints();
    string health = ss.str();
    
    ss.str("");
    ss << m_frackman->getGoldNuggets();
    string gold = ss.str();
    
    ss.str("");
    ss << m_frackman->getSonarCharge();
    string sonar = ss.str();
    
    ss. str("");
    ss << numBarrels;
    string oil = ss.str();
    
    setGameStatText(" Scr: " + scr + "  Lvl: " + lvl + "  Lives: " + to_string(lives) + "  Hlth: " + health + " Water: " + water +  " Gld: " + gold + " Sonar: " + sonar + " Oil Left: " + oil);
}

void StudentWorld::fireSquirt()
{
    int x = m_frackman->getX();
    int y = m_frackman->getY();
    Actor::Direction d = m_frackman->getDirection();
    if (d == Actor::up)
        m_actors.push_back(new Squirts(this, x,y + 4, d));
    else if (d == Actor::down)
        m_actors.push_back(new Squirts(this, x, y-4, d));
    else if (d == Actor::right)
        m_actors.push_back(new Squirts(this, x + 4, y, d));
    else
        m_actors.push_back(new Squirts(this, x - 4, y, d));
    
}

void StudentWorld::addGoldNugget()
{
    m_actors.push_back(new GoldNuggetForProtester(this, m_frackman->getX(), m_frackman->getY()));
}

void StudentWorld::removeDeadGameObjects()
{
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    
    while (it != m_actors.end())
    {
        if (!(*it)->isAlive())
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
}

/**
bool StudentWorld::thePlayerCompletedTheCurrentLevel() const
{
    
}

bool StudentWorld::canProtesterMoveTo() const
{
    return true;
}
*/

// Returns the number of protesters
int StudentWorld::getNumProtesters() const
{
    int total = 0;
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->huntsFrackMan())
            total++;
    }
    return total;
}

int StudentWorld::move()
{
    m_ticks++;
    
    // Update the Game Status Line
    updateDisplayText();
    
    // There is a 1 in G chance that a new Water Pool or Sonar Kit Goodie will be added to the oil field during any particular tick
    int g = getLevel() * 25 + 300;
    
    if ((rand() % g) < 1)
    {
        if ((rand() % 100) < 20)  // 1/5 chance to add a new sonar kit
        {
            m_actors.push_back(new SonarKit(this, 0, 60));
        }
        else  // else add a water goodie 
        {
            m_actors.push_back(new WaterPool(this, 30,30));
        }
        
    }

    m_frackman->doSomething();
    // Let frackman pick up something
    
    revealAllNearbyObjects(m_frackman->getX()+2, m_frackman->getY()+2, 4);
    
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    
    // Give each Actor a chance to do something
    while (it != m_actors.end())
    {
        if ((*it)->canBePickedUp())
        {
            if ((*it)->needsToBePickedUpToFinishLevel()) // barrel of oil
            {
                int x = m_frackman->getX();
                int y = m_frackman->getY();
                int a_x = (*it)->getX();
                int a_y = (*it)->getY();
                int ctr = false;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x+i == a_x && y+j == a_y)
                        {
                            ctr = true;
                        }
                    }
                }
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x == a_x+i && y == a_y+j)
                        {
                            ctr = true;
                        }
                        
                    }
                }
                if (ctr == true)
                {
                    GameController::getInstance().playSound(SOUND_GOT_GOODIE);
                    numBarrels--;
                    increaseScore(1000);
                    (*it)->setDead();
                }
                it++;
            }
            else if ((*it)->canBeGiven()) // GoldForFrackman
            {
                int x = m_frackman->getX();
                int y = m_frackman->getY();
                int a_x = (*it)->getX();
                int a_y = (*it)->getY();
                int ctr = false;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x+i == a_x && y+j == a_y)
                        {
                            ctr = true;
                        }
 
                    }
                }
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x == a_x+i && y == a_y+j)
                        {
                            ctr = true;
                        }
                        
                    }
                }
                if (ctr == true)
                {
                    GameController::getInstance().playSound(SOUND_GOT_GOODIE);
                    m_frackman->giveGoldNugget();
                    increaseScore(10);
                    (*it)->setDead();
                }
                it++;
            }
            else if ((*it)->isSonar()) // sonar
            {
                int x = m_frackman->getX();
                int y = m_frackman->getY();
                int a_x = (*it)->getX();
                int a_y = (*it)->getY();
                int ctr = false;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x+i == a_x && y+j == a_y)
                        {
                            ctr = true;
                        }
                    }
                }
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x == a_x+i && y == a_y+j)
                        {
                            ctr = true;
                        }
                        
                    }
                }
                if (ctr == true)
                {
                    GameController::getInstance().playSound(SOUND_GOT_GOODIE);
                    m_frackman->giveSonar();
                    increaseScore(75);
                    (*it)->setDead();
                }
                it++;
            }
            else if ((*it)->isWaterPool()) // water pool
            {
                int x = m_frackman->getX();
                int y = m_frackman->getY();
                int a_x = (*it)->getX();
                int a_y = (*it)->getY();
                int ctr = false;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x+i == a_x && y+j == a_y)
                        {
                            ctr = true;
                        }
                    }
                }
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (x == a_x+i && y == a_y+j)
                        {
                            ctr = true;
                        }
                        
                    }
                }
                if (ctr == true)
                {
                    GameController::getInstance().playSound(SOUND_GOT_GOODIE);
                    m_frackman->giveWater();
                    increaseScore(100);
                    (*it)->setDead();
                }
                it++;
            }

            else
                it++;
        }
        else
        {
            it++;
        }
        
    }
    
        // Clears the dirt
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int temp_x = m_frackman->getX() + i;
                int temp_y = m_frackman->getY() + j;
                if (temp_y < 60 && (temp_x < 30 || temp_x > 33))
                {
                    if (arr[temp_x][temp_y] != nullptr)
                    {
                        delete arr[temp_x][temp_y];
                        arr[temp_x][temp_y] = nullptr;
                        GameController::getInstance().playSound(SOUND_DIG);
                    }
                }
                else if (temp_x >= 30 && temp_x <= 33 && temp_y <= 3)
                {
                    if (arr[temp_x][temp_y] != nullptr)
                    {
                        delete arr[temp_x][temp_y];
                        arr[temp_x][temp_y] = nullptr;
                        GameController::getInstance().playSound(SOUND_DIG);
                    }
                }
            }
        }
    
    if (m_frackman->getHitPoints() <= 0)
    {
        // SOUND_PLAYER_GIVE_UP
        GameController::getInstance().playSound(SOUND_PLAYER_GIVE_UP);
        if (getLives() == 0)
        {
            return GWSTATUS_PLAYER_DIED;
        }
        else
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }

    // a new protester may only be added to the oil field after at least t ticks have passed since the last Protester of any type was added, where:
    int t = fmin(25, 200 - getLevel());
    
    // the target number p of protesters that should be on the field is equal to:
    int p = fmin(15, 2 + getLevel()* 1.5);
    
    if (getNumProtesters() < p && m_ticks%t == 0)
    {
        int probabilityOfHardcore = fmin(90, getLevel() * 10 + 30);
        srand((unsigned int)time(NULL));
        if ((rand() % 100) < probabilityOfHardcore)
            m_actors.push_back(new HardCoreProtester(this, 60,60, getLevel()));
        else
            m_actors.push_back(new RegularProtester(this, 60,60, getLevel()));
    }
    
    // Protester waits for a certain amount of tickets:
    int ticksToWaitBetweenMoves = fmax(0, 3 - getLevel()/4);
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->huntsFrackMan())
        {
           if (m_ticks % ticksToWaitBetweenMoves == 0)
               m_actors[i]->doSomething();
        }
        if (!(m_actors[i]->canActorsPassThroughMe())) // if the actor is a boulder
        {
            m_actors[i]->doSomething();
            if (m_actors[i]->getState() == 2)
            {
                // Boulder's coordinates:
                int b_x = m_actors[i]->getX();
                int b_y = m_actors[i]->getY();
                
                // Frackman's coordinates:
              //  int temp_x = m_frackman->getX();
               // int temp_y = m_frackman->getY();
                
                // update bouldergrid
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        boulderGrid[b_x + j][b_y + k] = false;
                    }
                }
                
                if (findNearbyFrackMan(m_actors[i], 3) != nullptr)
                {
                    decLives();
                    return GWSTATUS_PLAYER_DIED;
                }
                
            }
        }
        if (dynamic_cast<Squirts*>(m_actors[i]) != nullptr)
        {
            m_actors[i]->doSomething();
        }
    }
    
    // Remove newly-dead actors after each tick
    removeDeadGameObjects(); // delete dead game objects
    
     // If the player has collected all of the Barrels on the level, then
    // return the result that the player finished the level
    // if (thePlayerCompletedTheCurrentLevel() == true)
    if (numBarrels <= 0)
    {
        GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // the player hasn't completed the current level and hasn't died
    // let them continue playing the current level
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_frackman;
    for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            if (arr[i][j] != nullptr)
                delete arr[i][j];
        }
    }
    
}
