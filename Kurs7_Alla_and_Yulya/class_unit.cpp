#include "class_unit.h"
#include <QDebug>
CLASS_UNIT::CLASS_UNIT(float x, float y, Texture &image)
{
    texture = image;
    sprite.setTexture(texture);
    rect.top = y;
    rect.left = x;
    rect.width = image.getSize().x;
    rect.height = image.getSize().y;
    dx = dy = 0;
    currentFrame = 0;
    sprite.setPosition(x, y);
    this->life = true;
}

CLASS_UNIT::~CLASS_UNIT()
{
    delete this;
}

void CLASS_UNIT::draw(RenderWindow &window)
{
    window.draw(sprite);
}

TYPE_OF_TILE CLASS_UNIT::collision(int dir, CLASS_MAP &map)
{
    TYPE_OF_TILE typeT = Nothing;

    CLASS_TILE tile = map.getTile(0, 0);
    float tW = tile.getRect().width;
    float tH = tile.getRect().height;
    for (int i = rect.top/tH; i < (rect.top + rect.height)/tH; i++)
    {
        for (int j = rect.left/tW; j < (rect.left + rect.width)/tW; j++)
        {
            CLASS_TILE title = map.getTile(i, j);
            typeT = title.getType();

            AI(typeT, typeT);

            if (dx == 0 && dy == 0)
                this->state = Stay;
            if (dx > 0)
                this->state = Right;
            if (dx < 0)
                this->state = Left;
            if (dy < 0)
                this->state = Jump;
            if (dy > 0)
                this->state = Fall;

            switch(title.getType())
            {
            case Wall:
                if (!dir)
                {
                    if (dx > 0)
                        rect.left = j*tW - rect.width, this->state = Stay;
                    if (dx < 0)
                        rect.left = j*tW + tW, this->state = Stay;
                }
                else
                {
                    if (dy > 0)
                        rect.top = i*tH - rect.height, dy = 0, onGround = true, this->state = Stay;
                    if (dy < 0)
                        rect.top = i*tH + tH, dy = 0, this->state = Stay;
                }
                break;
            }
        }
    }
    return typeT;
}

void CLASS_UNIT::update(float time, CLASS_MAP &map)
{
    rect.left += dx*time; // x-координата; перемещаем её на dx

    TYPE_OF_TILE collisionTileX = Nothing;
    TYPE_OF_TILE collisionTileY = Nothing;

    // взаимодействие с игровым миром (столкновения)
    collisionTileX = collision(0, map);

    if (!onGround)
        dy = dy + 0.0005*time; // если не на земле, то падаем с ускорением и прибавляем к скорости
    rect.top += dy*time; // y-координата
    onGround = false;
    collisionTileY = collision(1, map);

    sprite.setPosition(rect.left, rect.top);
    animation(time);
    dx = 0;
    AI(collisionTileX, collisionTileY);
}

void CLASS_UNIT::die()
{
    this->life = false;
}

void CLASS_UNIT::setPosition(float x, float y)
{
    this->rect.left = x;
    this->rect.top = y;
    this->dx = 0;
    this->dy = 0;
    this->state = Stay;
}

void CLASS_UNIT::kick()
{
    if (dx > 0)
        dx = -7;
    else if (dx < 0)
        dx = 7;
    if (dy > 0)
        dy = -2;
}
