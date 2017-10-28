#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class HumanPlayer : public Player
{
    public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
: Player(nm, g){}

bool HumanPlayer::isHuman() const
{
    return true;
}

bool HumanPlayer::placeShips(Board& b)
{
    
    cout << name() << " must place " <<game().nShips()<<" ships."<<endl;
    b.display(false);
    
    for(int i= 0; i<game().nShips();i++)
    {
        char direction;
        
        do
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            cin >> direction;
            cin.ignore(10000, '\n');
            if(direction!='v'&&direction!='h')
                cout << "Direction must be h or v." << endl;
        } while(direction != 'v' && direction != 'h');
        
        Direction dir;
        if(direction=='v')
            dir=VERTICAL;
        else
            dir=HORIZONTAL;
        
        bool stopSign = false;
        do
        {
            cout<<"Enter row and column of leftmost cell (e.g. 3 5): ";
            int r,c;
            if(!getLineWithTwoIntegers(r, c))
            {
                cout << "You must enter two integers." << endl;
                stopSign = false;
            }
            else
            {
                if(b.placeShip(Point(r, c), i, dir))
                {
                    b.display(false);
                    stopSign = true;
                }
                else
                {
                    cout << "The ship can not be placed there." << endl;
                    stopSign = false;
                }
            }
        } while(stopSign != true);
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    bool stopSign = false;
    do
    {
        cout<<"Enter the row and column to attack (e.g, 3 5): ";
        int r,c;
        if(!getLineWithTwoIntegers(r, c))
        {
            cout << "You must enter two integers." << endl;
            stopSign = false;
        }
        else
        {
            stopSign = true;
            return Point(r,c);
        }
    } while(stopSign != true);
    
    return Point(-1,-1);
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
    // HumanPlayer completely ignores the result of any attack
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
    // HumanPlayer completely ignores what the opponent does
}




//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
    bool place(int nShips, Board& b);
    int m_state;
    char m_grid[MAXROWS][MAXCOLS];
    
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g)
{
    m_state = 1;
    for (int i=0; i<game().rows(); i++)
        for(int k=0; k<game().cols(); k++)
        {
            m_grid[i][k] = 'A';
        }
}

bool MediocrePlayer::isHuman() const
{
    return false;
}

bool MediocrePlayer::place(int nShips, Board& b)
{
    if (nShips==0)
    {
        return true;
    }
    
    for (int i=0; i<game().rows(); i++)
        for(int k=0; k<game().cols(); k++)
        {
            if(b.placeShip(Point(i,k), game().nShips()-nShips, VERTICAL))
            {
                
                if(place(nShips-1,b))
                    return true;
                else
                    b.unplaceShip(Point(i,k), game().nShips()-nShips, VERTICAL);
            }
            
            if(b.placeShip(Point(i,k), game().nShips()-nShips, HORIZONTAL))
            {
                
                if(place(nShips-1,b))
                    return true;
                else
                    b.unplaceShip(Point(i,k), game().nShips()-nShips, HORIZONTAL);
            }
        }
    
    return false;
}

bool MediocrePlayer::placeShips(Board& b)
{
    int count = 0;
    while(count < 50)
    {
        b.unblock();
        b.block();
        if(place(game().nShips(),b))
        {
            b.unblock();
            return true;
        }
        count++;
    }
    return false;
}

Point MediocrePlayer::recommendAttack()
{
    if(m_state == 1)
    {
        do
        {
            Point p = game().randomPoint();
            if(m_grid[p.r][p.c]=='A')
            {
                m_grid[p.r][p.c]='N';
                return p;
            }
        }while(m_state == 1);
        
    }
    
    if(m_state == 2)
    {
        
        do
        {
            Point p = game().randomPoint();
            if(m_grid[p.r][p.c]=='+')
            {
                m_grid[p.r][p.c]='N';
                return p;
            }
        }while(m_state == 2);
    }
    
    return Point(-1,-1);
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if(m_state == 1)
    {
        if(!validShot)
            return;
        
        if(validShot && !shotHit)
            m_state = 1;
        
        if(validShot && shotHit && shipDestroyed )
            m_state = 1;
        
        
        if(validShot && shotHit && !shipDestroyed)
        {
            m_state = 2;
            
            for (int i=p.r-4; i<p.r+5; i++)
            {
                if(i>=0 && i<game().rows())
                {
                    if(m_grid[i][p.c] == 'A')
                        m_grid[i][p.c] = '+';
                }
               
            }
            
            for (int i=p.c-4; i<p.c+5; i++)
            {
                if(i>=0 && i<game().cols())
                {
                    if(m_grid[p.r][i] == 'A')
                        m_grid[p.r][i] = '+';
                }
            }
        }
        
        for (int i=0; i<game().rows(); i++){
            for(int k=0; k<game().cols(); k++)
            { cout << m_grid[i][k];}
            cout << endl;}
        
        return;
    }
    
    
    if(m_state == 2)
    {
        if(!shotHit)
        {
            m_state = 2;
        }
        
        if(shotHit && !shipDestroyed)
        {
            m_state = 2;
        }
        
        if(shotHit && shipDestroyed)
        {
            m_state = 1;
            for(int i=0; i<game().rows(); i++)
                for(int k=0; k<game().cols();k++)
                {
                    
                    if(m_grid[i][k]=='+')
                    {
                        m_grid[i][k]='A';
                    }
                }
        }
        for (int i=0; i<game().rows(); i++){
            for(int k=0; k<game().cols(); k++)
            { cout << m_grid[i][k];}
            cout << endl;}
        
        return;
    }
}

void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{
    // MediocrePlayer completely ignores what the opponent does
}


//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
    bool place(int nShips, Board& b);
    int m_state;
    char m_grid[MAXROWS][MAXCOLS];
    bool thereIsPlusAndP() const;
    Point m_lastRandomHitted;
    
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g)
{
    m_state = 1;
    for (int i=0; i<game().rows(); i++)
        for(int k=0; k<game().cols(); k++)
        {
            m_grid[i][k] = 'A';
        }
}

bool GoodPlayer::isHuman() const
{
    return false;
}


bool GoodPlayer::thereIsPlusAndP() const
{
    for(int i=0; i<game().rows(); i++)
        for(int k=0; k<game().cols();k++)
        {
            if(m_grid[i][k]=='+')
            {
                return true;
            }
            if(m_grid[i][k]=='P')
                return true;
        }
    
    return false;
}

bool GoodPlayer::place(int nShips, Board& b)
{
    if (nShips==0)
    {
        return true;
    }
    
    for (int i=0; i<game().rows(); i++)
        for(int k=0; k<game().cols(); k++)
        {
            if(b.placeShip(Point(i,k), game().nShips()-nShips, VERTICAL))
            {
                
                if(place(nShips-1,b))
                    return true;
                else
                    b.unplaceShip(Point(i,k), game().nShips()-nShips, VERTICAL);
            }
            
            if(b.placeShip(Point(i,k), game().nShips()-nShips, HORIZONTAL))
            {
                
                if(place(nShips-1,b))
                    return true;
                else
                    b.unplaceShip(Point(i,k), game().nShips()-nShips, HORIZONTAL);
            }
        }
    
    return false;
}

bool GoodPlayer::placeShips(Board& b)
{
    int count = 0;
    while(count < 50)
    {
        b.unblock();
        b.block();
        if(place(game().nShips(),b))
        {
            b.unblock();
            return true;
        }
        count++;
    }
    return false;
}

Point GoodPlayer::recommendAttack()
{
    if(m_state == 1)
    {
        if(thereIsPlusAndP())
        {
            do
            {
                Point p = game().randomPoint();
                if(m_grid[p.r][p.c]=='+'||m_grid[p.r][p.c]=='P')
                {
                    m_grid[p.r][p.c]='N';
                    
                    return p;
                }
            }while(m_state == 1);
        }
        else
        {
            do
            {
                /*Point p = game().randomPoint();
                if(m_grid[p.r][p.c]=='A'&&(p.r%2)==(p.c%2))
                {
                    m_grid[p.r][p.c]='N';
                    return p;
                }*/
              for (int i=0; i<game().rows(); i++)
              {
                for(int k=0; k<game().cols(); k++)
                {
                    if(m_grid[i][k]=='A'&&(i%2)==(k%2))
                    {
                        m_grid[i][k]='N';
                        return Point(i,k);
                    }
                }
              }
                
            }while(m_state == 1);
        }
        
    }
    
    if(m_state == 2)
    {
        for (int i=0; i<game().rows(); i++)
            for(int k=0; k<game().cols(); k++)
            {
                if(m_grid[i][k]=='P')
                {
                    m_grid[i][k]='N';
                    return Point(i,k);
                    
                }
            }

        do
            {
                Point p = game().randomPoint();
                if(m_grid[p.r][p.c]=='+')
                {
                    m_grid[p.r][p.c]='N';
                    return p;
                }
            }while(m_state == 2);

    }
    
    return Point(-1,-1);
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if(m_state == 1)
    {
        if(!validShot)
            return;
        
        if(validShot && !shotHit)
            m_state = 1;
        
        if(validShot && shotHit && shipDestroyed )
        {
            m_state = 1;
            
        }
        
        
        if(validShot && shotHit && !shipDestroyed)
        {
            m_state = 2;
            
            if((p.r+1)<game().rows()&&m_grid[p.r+1][p.c]=='A')
                m_grid[p.r+1][p.c]='+';
            if((p.r-1)>=0&&m_grid[p.r-1][p.c]=='A')
                m_grid[p.r-1][p.c]='+';
            if((p.c+1)<game().cols()&&m_grid[p.r][p.c+1]=='A')
                m_grid[p.r][p.c+1]='+';
            if((p.c-1)>=0&&m_grid[p.r][p.c-1]=='A')
                m_grid[p.r][p.c-1]='+';
            
            
            
            m_lastRandomHitted  = p;
        }
        
        for (int i=0; i<game().rows(); i++){
            for(int k=0; k<game().cols(); k++)
            { cout << m_grid[i][k];}
            cout << endl;}
        
        return;
    }
    
    
    if(m_state == 2)
    {
        
        if(!shotHit)
        {
            m_state = 2;
        }
        
        if(shotHit && !shipDestroyed)
        {
            m_state = 2;

            if((p.r+1)<game().rows()&&m_grid[p.r+1][p.c]=='A')
                m_grid[p.r+1][p.c]='+';
            if((p.r-1)>=0&&m_grid[p.r-1][p.c]=='A')
                m_grid[p.r-1][p.c]='+';
            if((p.c+1)<game().cols()&&m_grid[p.r][p.c+1]=='A')
                m_grid[p.r][p.c+1]='+';
            if((p.c-1)>=0&&m_grid[p.r][p.c-1]=='A')
                m_grid[p.r][p.c-1]='+';
            
            if(p.c==m_lastRandomHitted.c)
            {
                for(int i=0;i<game().rows();i++)
                {
                    if(m_grid[i][p.c]=='+')
                        m_grid[i][p.c]='P';
                }
            }
            
            if(p.r==m_lastRandomHitted.r)
            {
                for(int i=0;i<game().cols();i++)
                {
                    if(m_grid[p.r][i]=='+')
                        m_grid[p.r][i]='P';
                }
            }
            
            m_lastRandomHitted=p;
        }
        
        if(shotHit && shipDestroyed)
        {
            m_state = 1;

        }
        
      
        
        for (int i=0; i<game().rows(); i++){
            for(int k=0; k<game().cols(); k++)
            { cout << m_grid[i][k];}
            cout << endl;}
        
        return;
    }
}

void GoodPlayer::recordAttackByOpponent(Point /* p */)
{
    // MediocrePlayer completely ignores what the opponent does
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
