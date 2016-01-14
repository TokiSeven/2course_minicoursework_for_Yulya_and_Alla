#include "class_player.h"

CLASS_PLAYER::CLASS_PLAYER(float x, float y, Texture &image) : CLASS_UNIT(x, y, image)
{
    score = 0;
    lifeCount = 3;
    maxLifeCount = 3;
    rect.width = rect.width / 2;
    sprite.setTextureRect(IntRect(0, 0, 64, 63));
}

CLASS_PLAYER::~CLASS_PLAYER()
{
    delete this;
}

void CLASS_PLAYER::jump()
{
    if (this->onGround)
    {
        dy = -0.7;
        this->state = Jump;
    }
}

void CLASS_PLAYER::right()
{
    dx = 0.3;
    this->state = Right;
}

void CLASS_PLAYER::left()
{
    dx = -0.3;
    this->state = Left;
}

void CLASS_PLAYER::animation(float time)
{
    // загрузим анимацию
    // скорость анимации 0.005
    currentFrame += 0.005 * time;
    if (currentFrame > 2)
        currentFrame -= 2; // два кадров
    if (dx > 0)
        sprite.setTextureRect(IntRect(64 * (int)currentFrame, 0, 64, 63));
    if (dx < 0)
        sprite.setTextureRect(IntRect(64 * (int)currentFrame + 64, 0, -64, 63));
}

void CLASS_PLAYER::AI(TYPE_OF_TILE colTileX, TYPE_OF_TILE colTileY)
{
    if (lifeCount <= 0)
        die();
    if (lifeCount > 3)
        lifeCount = 3;
}

void CLASS_PLAYER::damage(int count)
{
    if (lifeCount > 0)
        lifeCount -= count;
}

void CLASS_PLAYER::heal(int count)
{
    if (lifeCount > 0 && lifeCount <= maxLifeCount)
        lifeCount += count;
}
