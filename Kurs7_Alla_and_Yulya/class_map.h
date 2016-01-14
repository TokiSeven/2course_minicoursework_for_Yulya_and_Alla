#ifndef CLASS_MAP_H
#define CLASS_MAP_H
#include <QDebug>
#include "class_tile.h"
#include <QList>
#include <QString>

class CLASS_MAP
{
public:
    CLASS_MAP();
    CLASS_MAP(const CLASS_MAP&);
    CLASS_MAP(const QString str[], int size);
    ~CLASS_MAP();

    void setMap(const QString str[], int size);
    void setMap(const CLASS_MAP&);

    inline int getWidth()const{return width;}
    inline int getHeight()const{return height;}
    CLASS_TILE getTile(int, int)const;
    CLASS_TILE* getTile2(int, int);
    inline CLASS_TILE* getCoin(int i)const{return coins[i];}
    inline CLASS_TILE* getLife(int i)const{return lifes[i];}
    inline CLASS_TILE* getMine(int i)const{return mines[i];}
    inline CLASS_TILE* getNextLevel()const{return nextLevel;}
    inline int getCoinsSize()const{return coins.size();}
    inline int getLifeSize()const{return lifes.size();}
    inline int getMinesSize()const{return mines.size();}

    void update();

    void draw(RenderWindow&);
private:
    int width, height;
    QList<CLASS_TILE> tiles;
    QList<CLASS_TILE*> coins;
    QList<CLASS_TILE*> lifes;
    QList<CLASS_TILE*> mines;
    CLASS_TILE *nextLevel;
};

#endif // CLASS_MAP_H
