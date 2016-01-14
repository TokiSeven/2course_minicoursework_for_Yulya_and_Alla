#include "class_map.h"

CLASS_MAP::CLASS_MAP()
{
    this->height = 0;
    this->width = 0;
}

CLASS_MAP::CLASS_MAP(const CLASS_MAP &map)
{
    this->setMap(map);
}

CLASS_MAP::CLASS_MAP(const QString map[], int size)
{
    this->setMap(map, size);
}

CLASS_MAP::~CLASS_MAP()
{
    //this->tiles.clear();
    //delete this;
}

void CLASS_MAP::setMap(const QString str[], int size)
{
    this->height = size;
    this->width = str[0].length();
    for (int i = 0; i < size; i++)
        for (int j = 0; j < str[i].length(); j++)
        {
            CLASS_TILE *tl = new CLASS_TILE;
            TYPE_OF_TILE type = Nothing;

            if (str[i][j] == 'B')
                type = Wall;
            else if (str[i][j] == '0')
                type = Coin;
            else if (str[i][j] == '3')
                type = Level;
            else if (str[i][j] == 'C')
                type = Life;
            else if (str[i][j] == 'M')
                type = Mine;
            else
                type = Nothing;

            tl->setTile(type);
            tl->setPosition(j*tl->getRect().width, i*tl->getRect().height);

            this->tiles.append(*tl);
        }
}

void CLASS_MAP::setMap(const CLASS_MAP &map)
{
    for (int i = 0; i < map.getHeight(); i++)
        for (int j = 0; j < map.getWidth(); j++)
            this->tiles.append(map.getTile(i, j));
    this->width = map.getWidth();
    this->height = map.getHeight();
}

CLASS_TILE CLASS_MAP::getTile(int y, int x)const
{
    int num = (y) * this->getWidth() + x;
    if (num >= this->tiles.count() || num >= this->tiles.size() || num < 0)
        num = 0;
    return this->tiles[num];
}

CLASS_TILE* CLASS_MAP::getTile2(int y, int x)
{
    int num = (y) * this->getWidth() + x;
    if (num >= this->tiles.count() || num >= this->tiles.size() || num < 0)
        num = 0;
    return &this->tiles[num];
}

void CLASS_MAP::draw(RenderWindow &window)
{
    for (int i = 0; i < this->tiles.size(); i++)
        tiles[i].draw(window);
}

void CLASS_MAP::update()
{
    coins.clear();
    lifes.clear();
    mines.clear();
    for (int i = 0; i < tiles.size(); i++)
        if (tiles[i].getType() == Coin)
            coins.append(&tiles[i]);
        else if (tiles[i].getType() == Life)
            lifes.append(&tiles[i]);
        else if (tiles[i].getType() == Level)
            nextLevel = &tiles[i];
        else if (tiles[i].getType() == Mine)
            mines.append(&tiles[i]);
}
