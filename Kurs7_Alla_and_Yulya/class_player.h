#ifndef CLASS_PLAYER_H
#define CLASS_PLAYER_H

#include "class_unit.h"
#include <QDebug>

using namespace sf;

class CLASS_PLAYER : public CLASS_UNIT
{
public:
    CLASS_PLAYER(float, float, Texture&);
    ~CLASS_PLAYER();

    void heal(int count = 1);
    void damage(int count = 1);
    void jump();
    void right();
    void left();

    inline void addScore(int count = 1){this->score += (count >= 0) ? count : 0;}
    inline void removeScore(int count = 1){this->score -= (count >= 0) ? count : 0;}
    inline int getScore()const{return this->score;}
    inline int getLife()const{return this->lifeCount;}
    inline bool isFall()const{return (dy > 0) ? true : false;}
private:
    void animation(float time);
    void AI(TYPE_OF_TILE colTileX, TYPE_OF_TILE colTileY);
    int score;
    int lifeCount;
    int maxLifeCount;
signals:
    void win();
};

#endif // CLASS_PLAYER_H
