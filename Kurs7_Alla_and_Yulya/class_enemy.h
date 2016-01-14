#ifndef CLASS_ENEMY_H
#define CLASS_ENEMY_H
#include <QTimer>

#include "class_unit.h"

class CLASS_ENEMY : public CLASS_UNIT
{
public:
    CLASS_ENEMY(float, float, Texture&);
    ~CLASS_ENEMY();

    void right();
    void left();

private:
    float counter;
    void animation(float time);
    void AI(TYPE_OF_TILE colTileX, TYPE_OF_TILE colTileY);
    bool dirDx;
    bool myDir;
};

#endif // CLASS_ENEMY_H
