#ifndef CLASS_TILE_H
#define CLASS_TILE_H
#include "SFML/Graphics.hpp"

using namespace sf;

enum TYPE_OF_TILE {Wall, Mine, Life, Level, Coin, Nothing};//���� �������

class CLASS_TILE
{
public:
    CLASS_TILE();
    CLASS_TILE(const CLASS_TILE&);
    CLASS_TILE(FloatRect, Texture, TYPE_OF_TILE);
    ~CLASS_TILE();

    void setTexture(Texture);//��������� �������� ��� �������
    void setRectangle(FloatRect);//��������� �������/�������� �������
    void setPosition(float, float);//��������� ������� �������
    void setType(TYPE_OF_TILE);//��������� ���� ������� (����, ������� � �.�.)
    void setSize(float, float);//��������� ������� �������

    //set Tile as type
    void setTile(TYPE_OF_TILE);//����������� �������� �����, ����� ��� �� ��� ���������

    inline FloatRect getRect()const{return rect;}//������ 4-�� ������ ������� (x, y, width, height)
    inline Texture getText()const{return text;}//������ �������� �������
    inline Sprite getSprite()const{return sp;}//������ ������� �������
    inline TYPE_OF_TILE getType()const{return type;}//������ ���� �������

    void draw(RenderWindow&)const;//��������� ������� �� �����
private:
    FloatRect rect;//4-� ������� ������� (x, y, width, height)
    Texture text;//�������� �������
    Sprite sp;//������ �������
    TYPE_OF_TILE type;//��� �������
};

#endif // CLASS_TILE_H
