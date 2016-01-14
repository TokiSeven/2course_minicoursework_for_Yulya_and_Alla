#include "class_tile.h"

CLASS_TILE::CLASS_TILE()
{
    FloatRect rect2;
    rect2.top = 0;
    rect2.left = 0;
    rect2.width = 0;
    rect2.height = 0;

    setType(Nothing);
    setRectangle(rect2);
}

CLASS_TILE::CLASS_TILE(const CLASS_TILE &tile)
{
    setRectangle(tile.getRect());
    setType(tile.getType());
    setTexture(tile.getText());
}

CLASS_TILE::CLASS_TILE(FloatRect rect2, Texture textr, TYPE_OF_TILE types)
{
    setRectangle(rect2);
    setTexture(textr);
    setType(types);
}

CLASS_TILE::~CLASS_TILE()
{
    //delete this;
}

void CLASS_TILE::setTexture(Texture text2)
{
    text = text2;
    sp.setTexture(text);
}

void CLASS_TILE::setRectangle(FloatRect rect2)
{
    setPosition(rect2.left, rect2.top);
    setSize(rect2.width, rect2.height);
}

void CLASS_TILE::draw(RenderWindow &window)const
{
    window.draw(sp);
}

void CLASS_TILE::setPosition(float x, float y)
{
    this->rect.left = x;
    this->rect.top = y;
    sp.setPosition(x, y);
}

void CLASS_TILE::setSize(float w, float h)
{
    this->rect.width = w;
    this->rect.height = h;
}

void CLASS_TILE::setType(TYPE_OF_TILE types)
{
    this->type = types;
}

void CLASS_TILE::setTile(TYPE_OF_TILE type2)
{
    Texture textB;

    switch(type2)
    {
    case Wall:
        textB.loadFromFile("img/ground.png");
        break;

    case Mine:
        textB.loadFromFile("img/mine.png");
        break;

    case Life:
        textB.loadFromFile("img/life.png");
        break;

    case Level:
        textB.loadFromFile("img/castle.png");
        break;

    case Coin:
        textB.loadFromFile("img/coin.png");
        break;

    case Nothing:
        textB.loadFromFile("img/water.jpg");
        break;
    }

    this->setType(type2);
    this->setTexture(textB);
    this->setSize(textB.getSize().x, textB.getSize().y);
}
