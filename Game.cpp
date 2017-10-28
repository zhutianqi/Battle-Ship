#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int m_rows;
    int m_cols;
    int m_nShips;
    class Ship
    {
    public:
        Ship(int length, char symbol, string name, int ID) {m_length = length; m_symbol = symbol; m_name = name; m_ID = ID;};
        int length() {return m_length;};
        char symbol() {return m_symbol;};
        string name() {return m_name;};
        int ID() {return m_ID;};
    private:
        int m_length;
        char m_symbol;
        string m_name;
        int m_ID;
    };
    vector<Ship*> shipContainer;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    m_nShips = 0;
    // This compiles but may not be correct
}

int GameImpl::rows() const
{
    return m_rows;  // This compiles but may not be correct
}

int GameImpl::cols() const
{
    return m_cols;  // This compiles but may not be correct
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if( length > 0 && length <= m_rows && length <= m_cols && symbol != 'X' && symbol != 'o')
    {
        for(int i =0; i < m_nShips; i++ )
        if(symbol == shipContainer[i]->symbol())
            return false;
        
        shipContainer.push_back(new Ship(length,symbol,name,m_nShips));
        m_nShips++;
        return true;
    }
    
    else
    return false;  // This compiles but may not be correct
}

int GameImpl::nShips() const
{
    return m_nShips;  // This compiles but may not be correct
}

int GameImpl::shipLength(int shipId) const
{
    return shipContainer[shipId]->length();  // This compiles but may not be correct
}

char GameImpl::shipSymbol(int shipId) const
{
    return shipContainer[shipId]->symbol();  // This compiles but may not be correct
}

string GameImpl::shipName(int shipId) const
{
    return shipContainer[shipId]->name();  // This compiles but may not be correct
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(!p1->placeShips(b1))
        return nullptr;
    
    if(!p2->placeShips(b2))
        return nullptr;
    
    //Gameplay
    bool shotsOnly1 = false;
    bool shotsOnly2 = false;
    bool shotHit1 = false;
    bool shotHit2 = false;
    bool shipDestroyed1 = false;
    bool shipDestroyed2 = false;
    int shipId1 = -1;
    int shipId2 = -1;
    bool stopSign = false;
    
    if (p1->isHuman())
    {
        shotsOnly2 = true;
    }
    
    if (p2->isHuman())
    {
        shotsOnly1 = false;
    }
    
    do
    {
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        b2.display(shotsOnly2);
        
        Point p1Attack = p1->recommendAttack();
        if (b2.attack(p1Attack, shotHit2, shipDestroyed2, shipId2)==true)
        {
            
            if(shotHit2 && !shipDestroyed2)
                cout << p1->name() << " attacked (" << p1Attack.r << "," <<  p1Attack.c<< ") and hit something, resulting in:" << endl;
            if(!shotHit2 && !shipDestroyed2)
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and missed, resulting in:" << endl;
            if(shotHit2 && shipDestroyed2)
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and destroyed the "<< shipName(shipId2)<< " resulting in:" << endl;
        
            
            b2.display(shotsOnly2);
            p1->recordAttackResult(p1Attack, isValid(p1Attack), shotHit2, shipDestroyed2, shipId2);
        }
        else
        {
            cout << p1->name() << " wasted a shot at (" << p1Attack.r << "," << p1Attack.c <<")." << endl;
            p1->recordAttackResult(p1Attack, isValid(p1Attack), false, false, -1);
        }
        
        p2->recordAttackByOpponent(p1Attack);
        
        if(b2.allShipsDestroyed())
        {
            cout << p1->name() << " wins!" << endl;
            if(p2->isHuman())
            {
                cout << "Winner's Board:" << endl;
                b1.display(false);
            }
            return p1;
        }
        
        if(shouldPause == true)
            waitForEnter();
        
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        b1.display(shotsOnly1);
        
        Point p2Attack = p2->recommendAttack();
        if (b1.attack(p2Attack, shotHit1, shipDestroyed1, shipId1)==true)
        {
            if(shotHit1 && !shipDestroyed1)
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and hit something, resulting in:" << endl;
            if(!shotHit1 && !shipDestroyed1)
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and missed, resulting in:" << endl;
            if(shotHit1 && shipDestroyed1)
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and destroyed the "<<shipName(shipId1)<< " resulting in:" << endl;
            
            
            b1.display(shotsOnly1);
            p2->recordAttackResult(p2Attack, isValid(p2Attack), shotHit1, shipDestroyed1, shipId1);
        }
        else
        {
            cout << p2->name() << " wasted a shot at (" << p2Attack.r << "," << p2Attack.c <<")." << endl;
            p2->recordAttackResult(p2Attack, isValid(p2Attack), false, false, -1);
        }
        
        p1->recordAttackByOpponent(p2Attack);
        
        if(b1.allShipsDestroyed())
        {
            cout << p2->name() << " wins!" << endl;
            if(p1->isHuman())
            {
                cout << "Winner's Board:" << endl;
                b2.display(false);
            }
            return p2;
        }
        
        if(shouldPause == true)
            waitForEnter();
        
    }while(stopSign != true);
    
    return nullptr;  // This compiles but may not be correct
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

