#include "class_enemy.h"
#include <QTimer>

CLASS_ENEMY::CLASS_ENEMY(float x, float y, Texture &image) : CLASS_UNIT(x, y, image)
{
    counter = 1;
    dirDx = true;
    myDir = true;
    right();
}

CLASS_ENEMY::~CLASS_ENEMY()
{
    delete this;
}

void CLASS_ENEMY::right()
{
    dx = 0.25;
    this->state = Right;
    dirDx = true;
}

void CLASS_ENEMY::left()
{
    dx = -0.25;
    this->state = Left;
    dirDx = false;
}

void CLASS_ENEMY::animation(float time)
{
    // загрузим анимацию
    // скорость анимации 0.005
    currentFrame += 0.005*time;
    if (currentFrame > 2)
        currentFrame -= 2; // два кадров
    if (dx > 0)
        sprite.setTexture(texture);
    if (dx < 0)
        sprite.setTexture(texture);
}

void CLASS_ENEMY::AI(TYPE_OF_TILE colTileX, TYPE_OF_TILE colTileY)
{
    if (colTileX == Wall)
    {
        if (dirDx)
        {
            left();
            dirDx = false;
        }
        else
        {
            right();
            dirDx = true;
        }
    }
    else
    {
        if (dirDx)
        {
            right();
            dirDx = true;
        }
        else
        {
            left();
            dirDx = false;
        }
    }
}
