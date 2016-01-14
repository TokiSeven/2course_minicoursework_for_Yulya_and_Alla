#ifndef CLASS_TILE_H
#define CLASS_TILE_H
#include "SFML/Graphics.hpp"

using namespace sf;

enum TYPE_OF_TILE {Wall, Mine, Life, Level, Coin, Nothing};//типы тайлика

class CLASS_TILE
{
public:
    CLASS_TILE();
    CLASS_TILE(const CLASS_TILE&);
    CLASS_TILE(FloatRect, Texture, TYPE_OF_TILE);
    ~CLASS_TILE();

    void setTexture(Texture);//установка текстуры для тайлика
    void setRectangle(FloatRect);//установка позиции/размеров тайлика
    void setPosition(float, float);//установка позиции тайлика
    void setType(TYPE_OF_TILE);//установка типа тайлика (мина, моентка и т.д.)
    void setSize(float, float);//установка размера тайлика

    //set Tile as type
    void setTile(TYPE_OF_TILE);//становление тайликом таким, какой тип ты ему передаешь

    inline FloatRect getRect()const{return rect;}//взятие 4-ех вершин тайлика (x, y, width, height)
    inline Texture getText()const{return text;}//взятие текстуры тайлика
    inline Sprite getSprite()const{return sp;}//взятие спрайта тайлика
    inline TYPE_OF_TILE getType()const{return type;}//взятие типа тайлика

    void draw(RenderWindow&)const;//рисование тайлика на экран
private:
    FloatRect rect;//4-е вершины тайлика (x, y, width, height)
    Texture text;//текстура тайлика
    Sprite sp;//спрайт тайлика
    TYPE_OF_TILE type;//тип тайлика
};

#endif // CLASS_TILE_H
