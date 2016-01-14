#ifndef CLASS_UNIT_H
#define CLASS_UNIT_H

#include "SFML/Graphics.hpp"
#include "class_map.h"

enum STATE {Stay, Right, Left, Jump, Fall};

using namespace sf;

class CLASS_UNIT
{
public:
    CLASS_UNIT(float, float, Texture&);
    ~CLASS_UNIT();

    //methods
    void draw(RenderWindow&);
    void update(float time, CLASS_MAP &map);
    void die();
    void setPosition(float, float);
    void kick();

    //inline functions
    inline bool isLive()const{return this->life;}
    inline FloatRect getRect()const{return this->rect;}
    inline bool isOnGround()const{return this->onGround;}
    inline STATE getState()const{return this->state;}

protected:
    //методы защищенные
    TYPE_OF_TILE collision(int dir, CLASS_MAP &map); // столкновения
    virtual void animation(float time){}//animation of units
    virtual void AI(TYPE_OF_TILE colTileX, TYPE_OF_TILE colTileY){}//AI of units

    //поля
    float dx, dy; // скорость
    FloatRect rect; // одна координата вместо четырёх
    bool onGround; // на земле находимся или нет
    Sprite sprite;//картинка на вывод на экран
    float currentFrame; // текущий кадр для анимации
    Texture texture;//текстура для хранения картинки
    bool life;//жизнь юнита
    STATE state;
};

#endif // CLASS_UNIT_H
