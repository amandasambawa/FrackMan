// Author: Amanda Sambawa

#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

void FrackMan::doSomething()
{
    int key;
    // Get user input (if any is available)
    if(getWorld()->getKey(key))
    {
    int x = getX();
    int y = getY();
    
    // User presses UP key and square is open, then increase y location by one
    if (key == KEY_PRESS_UP)
    {
        if (getDirection() == up) {
        if (x >= 0 && y >= 0 && x <= VIEW_WIDTH-4 && y < VIEW_HEIGHT-4)
        {
            int new_y = y+1;
            bool ctr = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (!getWorld()->canFrackmanMoveTo(x+i, new_y+j)) // cannot move
                    {
                        ctr = false;
                        break;
                    }
                }
            }
            if (ctr == true)
                moveTo(x, y + 1);
        }
        else
            moveTo(x,y);
        }
        setDirection(up);
    }
    // User presses DOWN key and square is open, then decrease y location by one
    else if (key == KEY_PRESS_DOWN)
    {
        if (getDirection() == down) {
        if (x >= 0 && y > 0 && x <= VIEW_WIDTH-4 && y <= VIEW_HEIGHT-4)
        {
            int new_y = y-1;
            bool ctr = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (!getWorld()->canFrackmanMoveTo(x+i, new_y+j)) // cannot move
                    {
                        ctr = false;
                        break;
                    }
                }
            }
            if (ctr == true)
                moveTo(x, y - 1);
        }
        else
            moveTo(x,y);
        }
        setDirection(down);
    }
    // User presses the LEFT key and square is open, then decrease x location by one
    else if (key == KEY_PRESS_LEFT)
    {
        if (getDirection() == left) {
        if (x > 0 && y >= 0 && x <= VIEW_WIDTH-4 && y <= VIEW_HEIGHT-4)
        {
            int new_x = x - 1;
            bool ctr = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (!getWorld()->canFrackmanMoveTo(new_x+i, y+j)) // cannot move
                    {
                        ctr = false;
                        break;
                    }
                }
            }
            if (ctr == true)
                moveTo(x - 1, y);
        }
        else
            moveTo(x,y);
        }
        setDirection(left);
    }
    // User presses the RIGHT key and quare is open, then increase x location by one
    else if (key == KEY_PRESS_RIGHT)
    {
        if (getDirection() == right) {
        if (x >= 0 && y >= 0 && x < VIEW_WIDTH-4 && y <= VIEW_HEIGHT-4)
        {
            int new_x = x+1;
            bool ctr = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (!getWorld()->canFrackmanMoveTo(new_x+i, y+j)) // cannot move
                    {
                        ctr = false;
                        break;
                    }
                }
            }
            if (ctr == true)
                moveTo(x + 1, y);
        }
        else
            moveTo(x,y);
        }
        setDirection(right);
    }
    // User presses the ESCAPE key, which allows the user to abort the current level
    else if (key == KEY_PRESS_ESCAPE)
    {
        setAnnoyed();
    }
    // If the user pressed the space bar to fire and the Frackman has water, then
    // Introduce a new Squirt object into the game facing the same direction as the player
    else if (key == KEY_PRESS_SPACE)
    {
        if (m_waterSquirts > 0)
        {
            getWorld()->fireSquirt();
            m_waterSquirts--;
        }
    }
    else if (key == 'Z' || key == 'z')
    {
        if (m_sonarCharge > 0)
        {
            // all hidden game objects that are within a radius of 12 must be setVisible(true)
            getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
            m_sonarCharge--;
        }
    }
    else if (key == KEY_PRESS_TAB)
    {
        if (m_goldNuggets > 0)
        {
            getWorld()->addGoldNugget();
        }
    }
        
    }
}

void Boulders::doSomething()
{
    if (m_alive != true)
        return;
    if (m_state == 0)
    {
        if (getWorld()->checkDirtBelow(getX(), getY()))
        {
            m_state = 1;
        }
    }
    else if (m_state == 1)
    {
        m_waitingCount--;
        if (m_waitingCount == 0)
            m_state = 2;
    }
    else if (m_state == 2)
    {
        if (getWorld()->checkDirtBelow(getX(), getY()))
        {
            moveTo(getX(), getY()-1);
        }
        else
            m_state = 3;
    }
    else if (m_state == 3)
    {
        setVisible(false);
    }
}

void Squirts::doSomething()
{
    int x = getWorld()->annoyAllNearbyActors(this, 2, 2);
    if (x > 0)
    {
        setDead();
        return;
    }
    if (m_waitingCount <= 4)
    {
        Direction d = getDirection();
        if (d == up)
        {
            if (getWorld()->canActorMoveTo(getX(), getY()+1) && getWorld()->canActorMoveTo(getX()+1, getY()+1) && getWorld()->canActorMoveTo(getX()+2, getY()+1) && getWorld()->canActorMoveTo(getX()+3, getY()+1))
                moveTo(getX(),getY() + 1);
        }
        else if (d == down)
        {
            if (getWorld()->canActorMoveTo(getX(), getY()-1) && getWorld()->canActorMoveTo(getX()+1, getY()-1) && getWorld()->canActorMoveTo(getX()+2, getY()-1)  && getWorld()->canActorMoveTo(getX()+3, getY()-1))
                    moveTo(getX(), getY()- 1);
            
        }
        else if (d == right)
        {
            if (getWorld()->canActorMoveTo(getX()+1, getY()) && getWorld()->canActorMoveTo(getX()+1, getY()+1) && getWorld()->canActorMoveTo(getX()+1, getY()+2) && getWorld()->canActorMoveTo(getX()+1, getY()+3))
                moveTo(getX() + 1, getY());
        }
        else
        {
            if (getWorld()->canActorMoveTo(getX()-1, getY()) && getWorld()->canActorMoveTo(getX()-1, getY()+1) && getWorld()->canActorMoveTo(getX()-1, getY()+2) && getWorld()->canActorMoveTo(getX()-1, getY()+3))
                moveTo(getX() - 1, getY());
        }
        
        m_waitingCount--;
    }
    if (m_waitingCount == 0 || x > 0)
    {
        setDead();
    }
}

bool Protester::annoy(unsigned int amt)
{
    m_hitPoints-= amt;
    if (m_hitPoints < 0)
    {
        setDead();
        return false;
    }
    setResting();
    return true;
}

void RegularProtester::doSomething()
{
    if (getHitPoinst() <= 0)
    {
        setDead();
        return;
    }
    
    if (!isAlive())
        return;
    
    if (isResting())
    {
        if (getTicks() < getRestingCount())
            incTicks();
        else
            setNotResting();
        return;
    }
    
    if (!isResting())
    {
        if (getWorld()->annoyFrackman(getX(), getY(), 2, 4))
            setResting();
    }
    
    Direction d = getDirection();
    if (d == up)
    {
        if (getWorld()->canActorMoveTo(getX(), getY()+1) && getWorld()->canActorMoveTo(getX()+1, getY()+1) && getWorld()->canActorMoveTo(getX()+2, getY()+1) && getWorld()->canActorMoveTo(getX()+3, getY()+1))
            moveTo(getX(),getY() + 1);
    }
    else if (d == down)
    {
        if (getWorld()->canActorMoveTo(getX(), getY()-1) && getWorld()->canActorMoveTo(getX()+1, getY()-1) && getWorld()->canActorMoveTo(getX()+2, getY()-1)  && getWorld()->canActorMoveTo(getX()+3, getY()-1))
            moveTo(getX(), getY()- 1);
        
    }
    else if (d == right)
    {
        if (getWorld()->canActorMoveTo(getX()+1, getY()) && getWorld()->canActorMoveTo(getX()+1, getY()+1) && getWorld()->canActorMoveTo(getX()+1, getY()+2) && getWorld()->canActorMoveTo(getX()+1, getY()+3))
            moveTo(getX() + 1, getY());
    }
    else
    {
        if (getWorld()->canActorMoveTo(getX()-1, getY()) && getWorld()->canActorMoveTo(getX()-1, getY()+1) && getWorld()->canActorMoveTo(getX()-1, getY()+2) && getWorld()->canActorMoveTo(getX()-1, getY()+3))
            moveTo(getX() - 1, getY());
    }
    
    incTicks();
    
    // If I am facing the Frackman and he is next to me, then shout at him
    
    // Else if the Frackman is visible via direct line of sight, then switch direction to face the
    // Frackman, and Move one square in this direction
    
    // Else if I'm about to run into an obstacle, then pick a new direction and
    // Move one square in this direction
    
    // Else
    // Move one square in my current direction
}

void HardCoreProtester::doSomething()
{
    if (getHitPoinst() <= 0)
    {
        setDead();
        return;
    }
    
    if (!isAlive())
        return;
    
    if (isResting())
    {
        if (getTicks() < getRestingCount())
            incTicks();
        else
            setNotResting();
        return;
    }
    
    if (!isResting())
    {
        if (getWorld()->annoyFrackman(getX(), getY(), 2, 4))
            setResting();
    }
    
    Direction d = getDirection();
    if (d == up)
    {
        if (getWorld()->canActorMoveTo(getX(), getY()+1) && getWorld()->canActorMoveTo(getX()+1, getY()+1) && getWorld()->canActorMoveTo(getX()+2, getY()+1) && getWorld()->canActorMoveTo(getX()+3, getY()+1))
            moveTo(getX(),getY() + 1);
    }
    else if (d == down)
    {
        if (getWorld()->canActorMoveTo(getX(), getY()-1) && getWorld()->canActorMoveTo(getX()+1, getY()-1) && getWorld()->canActorMoveTo(getX()+2, getY()-1)  && getWorld()->canActorMoveTo(getX()+3, getY()-1))
            moveTo(getX(), getY()- 1);
        
    }
    else if (d == right)
    {
        if (getWorld()->canActorMoveTo(getX()+1, getY()) && getWorld()->canActorMoveTo(getX()+1, getY()+1) && getWorld()->canActorMoveTo(getX()+1, getY()+2) && getWorld()->canActorMoveTo(getX()+1, getY()+3))
            moveTo(getX() + 1, getY());
    }
    else
    {
        if (getWorld()->canActorMoveTo(getX()-1, getY()) && getWorld()->canActorMoveTo(getX()-1, getY()+1) && getWorld()->canActorMoveTo(getX()-1, getY()+2) && getWorld()->canActorMoveTo(getX()-1, getY()+3))
            moveTo(getX() - 1, getY());
    }
    
    incTicks();
    
    // If I am facing the Frackman and he is next to me, then shout at him
    
    // Else if the Frackman is visible via direct line of sight, then switch direction to face the
    // Frackman, and Move one square in this direction
    
    // Else if I'm about to run into an obstacle, then pick a new direction and
    // Move one square in this direction
    
    // Else
    // Move one square in my current direction
}