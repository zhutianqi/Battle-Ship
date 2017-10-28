#include "Board.h"
#include "Game.h"
#include "globals.h"
#include "vector"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    char m_grid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
    int m_nships;
    class Ship
    {
    public:
        Ship(int id,int length,char symbol) {m_health = length; m_Id = id;m_symbol = symbol;};
        int Id() {return m_Id;};
        char symbol() {return m_symbol;};
        int m_health;
    private:
        int m_Id;
        char m_symbol;
    };
    vector<Ship*> placedShip;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    m_rows = g.rows();
    m_cols = g.cols();
    m_nships = g.nShips();
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_grid[r][c] = '.';
        }
    }
    // This compiles, but may not be correct
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_grid[r][c] = '.';
        }
    }
    // This compiles, but may not be correct
}

void BoardImpl::block()
{
    /*for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            m_grid[r][c] = '#';
    
    
    m_grid[1][3] = '.';
    m_grid[2][3] = '.';
    m_grid[3][3] = '.';
    m_grid[4][3] = '.';
    m_grid[4][2] = '.';
    m_grid[4][4] = '.';
    m_grid[4][5] = '.';
    m_grid[2][5] = '.';
    m_grid[3][5] = '.';
    m_grid[4][5] = '.';*/
    
    
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                m_grid[r][c] = '#'; // TODO:  Replace this with code to block cell (r,c)
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if(m_grid[r][c] == '#')
            {
                m_grid[r][c] = '.';
            }
            // TODO:  Replace this with code to unblock cell (r,c) if blocked
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId < 0 || shipId >= m_nships)
        return false;
    
    for(int i =0; i < placedShip.size();i++)
    {
        if(placedShip[i]->Id() == shipId)
        {
            return false;
        }
    }
    
    if(dir == VERTICAL)
    {
        if(topOrLeft.r <0 || topOrLeft.c < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c >= m_game.cols() )
            return false;
        
        if(topOrLeft.r + m_game.shipLength(shipId) > m_game.rows()) // insted of >= you cover from the beginning topOrleft.r
            return false;
        
        for(int i = topOrLeft.r; i < topOrLeft.r +  m_game.shipLength(shipId); i++)
        {
            if(m_grid[i][topOrLeft.c] != '.')
                return false;
        }
        
        for(int k = topOrLeft.r; k < topOrLeft.r +  m_game.shipLength(shipId); k++)
        {
            m_grid[k][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }
    
    if(dir == HORIZONTAL)
    {
        if(topOrLeft.r <0 || topOrLeft.c < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c >= m_game.cols() )
            return false;
        
        if(topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()) // insted of >= you cover from the beginning topOrleft.r
            return false;
        
        for(int i = topOrLeft.c; i < topOrLeft.c +  m_game.shipLength(shipId); i++)
        {
            if(m_grid[topOrLeft.r][i] != '.')
                return false;
        }
        
        for(int k = topOrLeft.c; k < topOrLeft.c +  m_game.shipLength(shipId); k++)
        {
            m_grid[topOrLeft.r][k] = m_game.shipSymbol(shipId);
        }
    }
    
    placedShip.push_back(new Ship(shipId,m_game.shipLength(shipId),m_game.shipSymbol(shipId)));
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId < 0 || shipId >= m_nships)
        return false;
    
    if(dir == VERTICAL)
    {
        //Check if the board contain the entire ship at the indicated locations.
        if(topOrLeft.r <0 || topOrLeft.c < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c >= m_game.cols() )
            return false;
        
        if(topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
            return false;
        
        for(int i = topOrLeft.r; i < topOrLeft.r +  m_game.shipLength(shipId); i++)
        {
            if(m_grid[i][topOrLeft.c] != m_game.shipSymbol(shipId))
                return false;
        }
        
        
        for(int k = topOrLeft.r; k < topOrLeft.r +  m_game.shipLength(shipId); k++)
        {
            m_grid[k][topOrLeft.c] = '.';
        }
    }
    
    if(dir == HORIZONTAL)
    {
        if(topOrLeft.r <0 || topOrLeft.c < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c >= m_game.cols() )
            return false;
        
        if(topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
            return false;
        
        for(int i = topOrLeft.c; i < topOrLeft.c +  m_game.shipLength(shipId); i++)
        {
            if(m_grid[topOrLeft.r][i] != m_game.shipSymbol(shipId))
                return false;
        }
        
        for(int k = topOrLeft.c; k < topOrLeft.c +  m_game.shipLength(shipId); k++)
        {
            m_grid[topOrLeft.r][k] = '.';
        }
    }
    
    for(int i = 0; i < placedShip.size();i++)
    {
        if(placedShip[i]->Id() == shipId)
            placedShip.erase(placedShip.begin()+i);
    }
    
    return true; // This compiles, but may not be correct
}

void BoardImpl::display(bool shotsOnly) const
{
    cout <<"  ";
    for(int i=0; i< m_game.cols();i++)
        cout << i;
    cout << endl;
    
    for(int i=0; i< m_game.rows();i++)
    {
        for(int k=0; k< m_game.cols();k++)
        {
            if(k == 0)
                cout << i << " ";
            
            if(shotsOnly == false)
                cout << m_grid[i][k];
            
            else if(shotsOnly == true)
            {
                if(m_grid[i][k]!='X' && m_grid[i][k]!='o' && m_grid[i][k]!='.')
                    cout << '.';
                else
                    cout << m_grid[i][k];
            }
        }
        cout << endl;
    }
    // This compiles, but may not be correct
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if(p.r <0 || p.c < 0 || p.r >= m_game.rows() || p.c >= m_game.cols() )
        return false;
    if(m_grid[p.r][p.c]== 'X' ||m_grid[p.r][p.c]== 'o')
        return false;
    
    if(m_grid[p.r][p.c]!= 'X' &&m_grid[p.r][p.c]!= 'o'&& m_grid[p.r][p.c]!='.')
    {
        shotHit = true;
        char temp = m_grid[p.r][p.c];
        m_grid[p.r][p.c] = 'X';
        for(int i =0; i< placedShip.size();i++)
        {
            if(placedShip[i]->symbol() == temp)
            {
                placedShip[i]->m_health--;
                
                if(placedShip[i]->m_health == 0)
                {
                    shipDestroyed = true;
                    shipId = placedShip[i]->Id();
                    m_nships--;
                    break;
                }
                
                else if (placedShip[i]->m_health != 0)
                {
                    shipDestroyed = false;
                    break;
                }
            }
        }
    }
    else if(m_grid[p.r][p.c]=='.')
    {
        shotHit = false;
        m_grid[p.r][p.c] = 'o';
    }
    return true; // This compiles, but may not be correct
}

bool BoardImpl::allShipsDestroyed() const
{
    if(m_nships==0)
        return true;
    else
        return false;
    // This compiles, but may not be correct
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
