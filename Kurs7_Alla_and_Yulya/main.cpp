#include "SFML/Graphics.hpp"
#include "class_map.h"
#include "class_player.h"
#include "class_enemy.h"
#include <QString>
#include <QDebug>

using namespace sf;

struct window_structure
{
    int windowWidth = 1280;
    int windowHeight = 720;
    QString windowName = "Game";
};

void writeInformation(RenderWindow&, CLASS_PLAYER&, View&);
void loseGame(RenderWindow&, CLASS_PLAYER&);
void victoryGame(RenderWindow&, CLASS_PLAYER&);
void nextLevel(QList<CLASS_UNIT*>&, int&, CLASS_PLAYER *, CLASS_TILE, float*, float*, Texture&, window_structure&);
int menuGame(window_structure);

int main()
{
    window_structure winStr;//some main structure
    int buffer = menuGame(winStr);//for start, go in menu
    if (buffer)
        return 0;
    RenderWindow window(VideoMode(winStr.windowWidth, winStr.windowHeight), winStr.windowName.toStdString());//create window
    View view;//camera for player
    int currentLevel = 1;//select current level (1-3)
    QList<CLASS_UNIT*> units;//create container for all units (player, enemys...)
    Clock clock; // переменная для привязки ко времени

    const int H = 12;
    const int W = 40;

    QString TileMap[3][H] =
    {
        {
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "B           0000000       B         3  B",
            "B C        BBBBBBBBB      B        BBB B",
            "BBBBB                     B            B",
            "B         00           000B    BB      B",
            "B         00         BBBBBB            B",
            "B       BBBBBB            B   000  BB  B",
            "B 000                     B            B",
            "BBBBBB                         BB      B",
            "B                                      B",
            "B                        BB            B",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        },
        {
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "B                    0              3  B",
            "B 0 0 0 0                    0     BBB B",
            "BBBBBBBBBBB         BBB      BB        B",
            "B              C                    0  B",
            "B   0 0       BBB          BB          B",
            "B  0 0 0                         BB    B",
            "B         BB        0 0 0              B",
            "BBBBBBBBBBBBBB               BB        B",
            "B                          BB     BBBBBB",
            "B                        BB   0 0 0 0 0B",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        },
        {
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "B   000              0              3  B",
            "B                                  BBB B",
            "BBBBBBBBBBB         BBB      B         B",
            "B              C                       B",
            "B   0 0       BBB           BBB        B",
            "B  0   0           0 0           C     B",
            "B                               BBB    B",
            "B   BBB       BBBBBBBBBBBB             B",
            "B              0 0 BB 0 0           BBBB",
            "B             0 0 0BB0 0 0             B",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        }
    };

    //set all maps from QString maps
    CLASS_MAP MAP[3];
    for (int i = 0; i < 3; i++)
        MAP[i].setMap(TileMap[i], H);

    float offsetX = winStr.windowWidth / 2;
    float offsetY = MAP[0].getHeight()*MAP[0].getTile(0, 0).getRect().height - winStr.windowHeight / 2; // offset – смещение игрока

    //create players and texture for him
    Image im, en;
    im.loadFromFile("img/player.png");
    en.loadFromFile("img/enemy.png");
    Texture t, te;
    t.loadFromImage(im);
    te.loadFromImage(en);
    CLASS_PLAYER Player(1*MAP[currentLevel - 1].getTile(0, 0).getRect().width, 10*MAP[currentLevel - 1].getTile(0, 0).getRect().height, t);

    //set camera
    view.reset(FloatRect(0, 0, winStr.windowWidth, winStr.windowHeight));

    //append to container our units
    units.append(&Player);
    units.append(new CLASS_ENEMY(10 * MAP[currentLevel - 1].getTile(0, 0).getRect().width, 9 * MAP[currentLevel - 1].getTile(0, 0).getRect().height, te));
    units.append(new CLASS_ENEMY(20 * MAP[currentLevel - 1].getTile(0, 0).getRect().width, 9 * MAP[currentLevel - 1].getTile(0, 0).getRect().height, te));
    units.append(new CLASS_ENEMY(30 * MAP[currentLevel - 1].getTile(0, 0).getRect().width, 9 * MAP[currentLevel - 1].getTile(0, 0).getRect().height, te));

    //gamecicle
    while (window.isOpen())
    {
        //прошедшее время
        float time = clock.getElapsedTime().asMicroseconds();

        //перезагружаем часы
        clock.restart();

        //регулировка скорости игры
        time = time/1;
        if (time > 60)
            time = 60;

        //обрабатываем событие закрытия окна
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //events with keyboard (or mouse) & player
        if (Keyboard::isKeyPressed(Keyboard::A))
            Player.left();
        if (Keyboard::isKeyPressed(Keyboard::D))
            Player.right();
        if (Keyboard::isKeyPressed(Keyboard::W))
            Player.jump();
        if (Keyboard::isKeyPressed(Keyboard::F))
        {
            if (Player.getScore() >= 10)
            {
                Player.removeScore(5);
                int numX = Player.getRect().left / MAP[0].getTile(0, 0).getRect().width;
                int numY = Player.getRect().top / MAP[0].getTile(0, 0).getRect().height;
                CLASS_TILE *tile = MAP[currentLevel - 1].getTile2(numY, numX);
                tile->setTile(Mine);
            }
        }


        for (int i = 1; i < units.size(); i++)
            for (int j = 0; j < MAP[currentLevel - 1].getMinesSize(); j++)
                if (units[i]->getRect().intersects(MAP[currentLevel - 1].getMine(j)->getRect()))
                {
                    units[i]->die();
                    MAP[currentLevel - 1].getMine(j)->setTile(Nothing);
                    Player.addScore(10);
                }
        //if player intersecting with units
        for (int i = 1; i < units.size(); i++)
        {
            if (units[i]->isLive())
            {
                float x1 = units[0]->getRect().left;
                float x2 = units[0]->getRect().width + x1;
                float y1 = units[0]->getRect().top;
                float y2 = units[0]->getRect().height + y1;
                if (units[i]->getRect().intersects(Player.getRect()) && Player.isFall())
                {
                    units[i]->die();
                    Player.kick();
                    Player.addScore(10);
                }
                else
                {
                    if (units[i]->getRect().intersects(Player.getRect()))
                    {
                        Player.damage();
                        Player.kick();
                        units[i]->kick();
                    }
                }
            }
        }

        //remove died enemys
        for (int i = 1; i < units.size();)
            if (!units[i]->isLive())
                units.removeAt(i);
            else
                i++;

        MAP[currentLevel - 1].update();

        //update all units
        for (int i = 0; i < units.size(); i++)
            units[i]->update(time, MAP[currentLevel - 1]);

        for (int i = 0; i < MAP[currentLevel - 1].getCoinsSize(); i++)
            if (Player.getRect().intersects(MAP[currentLevel - 1].getCoin(i)->getRect()))
            {
                MAP[currentLevel - 1].getCoin(i)->setTile(Nothing);
                Player.addScore();
            }

        for (int i = 0; i < MAP[currentLevel - 1].getLifeSize(); i++)
            if (Player.getRect().intersects(MAP[currentLevel - 1].getLife(i)->getRect()))
            {
                MAP[currentLevel - 1].getLife(i)->setTile(Nothing);
                Player.heal();
            }

        //смещаем камеру
        if (Player.getRect().left >= winStr.windowWidth / 2 && Player.getRect().left <= MAP[currentLevel - 1].getWidth() * MAP[currentLevel - 1].getTile(0,0).getRect().width - winStr.windowWidth / 2)
            offsetX = Player.getRect().left;
        if (Player.getRect().top >= winStr.windowHeight / 2 && Player.getRect().top <= MAP[currentLevel - 1].getHeight() * MAP[currentLevel - 1].getTile(0,0).getRect().height - winStr.windowHeight / 2)
            offsetY = Player.getRect().top;

        view.setCenter(offsetX, offsetY);

        window.setView(view);
        //очищаем экран и рисуем то, что надо
        window.clear(Color::Blue);
        MAP[currentLevel - 1].draw(window);
        for (int i = 0; i < units.size(); i++)
            units[i]->draw(window);
        writeInformation(window, Player, view);
        window.display();

        //intersects with next level
        if (Player.getRect().intersects(MAP[currentLevel - 1].getNextLevel()->getRect()))
            nextLevel(units, currentLevel, &Player, MAP[currentLevel - 1].getTile(0, 0), &offsetX, &offsetY, te, winStr);

        if (currentLevel > 3)
            victoryGame(window, Player);
        if (!Player.isLive())
            loseGame(window, Player);
    }

    return 0;
}

void writeInformation(RenderWindow &window, CLASS_PLAYER &pl, View &view)
{
    RectangleShape rect;
    Font font;//шрифт
    font.loadFromFile("fonts/CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
    Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
    text.setColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

    QString str;
    str = "Lifes: " + QString::number(pl.getLife()) + "\nScore: " + QString::number(pl.getScore());

    text.setString(str.toStdString());
    text.setPosition(view.getCenter().x - window.getSize().x / 2 + 50, view.getCenter().y - window.getSize().y / 2 + 20);

    rect.setFillColor(Color::White);
    rect.setSize(Vector2f(110, 50));
    rect.setPosition(text.getPosition().x - 5, text.getPosition().y - 4);
    rect.setOutlineColor(Color::Green);
    rect.setOutlineThickness(3);

    window.draw(rect);
    window.draw(text);
}

void nextLevel(QList<CLASS_UNIT *> &units, int &currLvl, CLASS_PLAYER *pl, CLASS_TILE needForWandH, float *ofX, float *ofY, Texture &te, window_structure &winStr)
{
    units.clear();
    if (currLvl > 3)
        currLvl = 1;
    else
        currLvl++;
    pl->setPosition(1*needForWandH.getRect().width, 10*needForWandH.getRect().height);
    pl->heal(3);
    units.append(pl);
    units.append(new CLASS_ENEMY(10 * needForWandH.getRect().width, 9 * needForWandH.getRect().height, te));
    units.append(new CLASS_ENEMY(20 * needForWandH.getRect().width, 9 * needForWandH.getRect().height, te));
    units.append(new CLASS_ENEMY(30 * needForWandH.getRect().width, 9 * needForWandH.getRect().height, te));
    (*ofX) = winStr.windowWidth / 2;
    (*ofY) = 12*needForWandH.getRect().height - winStr.windowHeight / 2;
}

void loseGame(RenderWindow &window2, CLASS_PLAYER &pl)
{
    window2.close();
    RenderWindow window(VideoMode(1280, 720), "Game");

    int menuNum = 0;

    while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        Font font;//шрифт
        font.loadFromFile("fonts/CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
        Text text("", font, 60);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
        text.setColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
        text.setStyle(sf::Text::Bold);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

        QString str;
        str = "Game end. You can only:\n\nExit from this game =(\n\nYour score: " + QString::number(pl.getScore());

        text.setString(str.toStdString());
        text.setPosition(window.getSize().x / 2 - 300, window.getSize().y / 2 - 100);

        RectangleShape rect1;
        rect1.setFillColor(Color::White);
        rect1.setSize(Vector2f(630, 70));
        rect1.setPosition(text.getPosition().x - 5, text.getPosition().y + 10);
        rect1.setOutlineColor(Color::Green);
        rect1.setOutlineThickness(3);

        RectangleShape rect2 = rect1;
        rect2.setPosition(rect1.getPosition().x, rect1.getPosition().y + 110);

        RectangleShape rect3 = rect2;
        rect3.setPosition(rect2.getPosition().x, rect2.getPosition().y + 110);


        if (IntRect(rect1.getPosition().x, rect1.getPosition().y, rect1.getSize().x, rect1.getSize().y).contains(Mouse::getPosition(window)))
            rect1.setFillColor(Color::Red);
        if (IntRect(rect2.getPosition().x, rect2.getPosition().y, rect2.getSize().x, rect2.getSize().y).contains(Mouse::getPosition(window)))
            rect2.setFillColor(Color::Red);
        if (IntRect(rect3.getPosition().x, rect3.getPosition().y, rect3.getSize().x, rect3.getSize().y).contains(Mouse::getPosition(window)))
            rect3.setFillColor(Color::Red);

        float x = Mouse::getPosition(window).x;
        float y = Mouse::getPosition(window).y;
        if (rect1.getPosition().x <= x && rect1.getPosition().x + rect1.getSize().x >= x && rect1.getPosition().y <= y && rect1.getPosition().y + rect1.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;
        if (rect2.getPosition().x <= x && rect2.getPosition().x + rect2.getSize().x >= x && rect2.getPosition().y <= y && rect2.getPosition().y + rect2.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;
        if (rect3.getPosition().x <= x && rect3.getPosition().x + rect3.getSize().x >= x && rect3.getPosition().y <= y && rect3.getPosition().y + rect3.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;

        window.clear(Color(240, 240, 240));
        window.draw(rect1);
        window.draw(rect2);
        window.draw(rect3);
        window.draw(text);
        window.display();

        switch(menuNum)
        {
        case 1:
            window.close();
            break;
        }
    }
}

void victoryGame(RenderWindow &window2, CLASS_PLAYER &pl)
{
    window2.close();
    RenderWindow window(VideoMode(1280, 720), "Game");

    int menuNum = 0;

    while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        Font font;//шрифт
        font.loadFromFile("fonts/CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
        Text text("", font, 60);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
        text.setColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
        text.setStyle(sf::Text::Bold);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

        QString str;
        str = "You did it! You win!\n\nNow you can only:\n\nExit from this game =)\n\nYour score: " + QString::number(pl.getScore());

        text.setString(str.toStdString());
        text.setPosition(window.getSize().x / 2 - 300, window.getSize().y / 2 - 200);

        RectangleShape rect1;
        rect1.setFillColor(Color::White);
        rect1.setSize(Vector2f(630, 70));
        rect1.setPosition(text.getPosition().x - 5, text.getPosition().y + 10);
        rect1.setOutlineColor(Color::Green);
        rect1.setOutlineThickness(3);

        RectangleShape rect2 = rect1;
        rect2.setPosition(rect1.getPosition().x, rect1.getPosition().y + 110);

        RectangleShape rect3 = rect2;
        rect3.setPosition(rect2.getPosition().x, rect2.getPosition().y + 110);

        RectangleShape rect4 = rect3;
        rect4.setPosition(rect3.getPosition().x, rect3.getPosition().y + 110);


        if (IntRect(rect1.getPosition().x, rect1.getPosition().y, rect1.getSize().x, rect1.getSize().y).contains(Mouse::getPosition(window)))
            rect1.setFillColor(Color::Red);
        if (IntRect(rect2.getPosition().x, rect2.getPosition().y, rect2.getSize().x, rect2.getSize().y).contains(Mouse::getPosition(window)))
            rect2.setFillColor(Color::Red);
        if (IntRect(rect3.getPosition().x, rect3.getPosition().y, rect3.getSize().x, rect3.getSize().y).contains(Mouse::getPosition(window)))
            rect3.setFillColor(Color::Red);
        if (IntRect(rect4.getPosition().x, rect4.getPosition().y, rect4.getSize().x, rect4.getSize().y).contains(Mouse::getPosition(window)))
            rect4.setFillColor(Color::Red);

        float x = Mouse::getPosition(window).x;
        float y = Mouse::getPosition(window).y;
        if (rect1.getPosition().x <= x && rect1.getPosition().x + rect1.getSize().x >= x && rect1.getPosition().y <= y && rect1.getPosition().y + rect1.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;
        if (rect2.getPosition().x <= x && rect2.getPosition().x + rect2.getSize().x >= x && rect2.getPosition().y <= y && rect2.getPosition().y + rect2.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;
        if (rect3.getPosition().x <= x && rect3.getPosition().x + rect3.getSize().x >= x && rect3.getPosition().y <= y && rect3.getPosition().y + rect3.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;
        if (rect4.getPosition().x <= x && rect4.getPosition().x + rect4.getSize().x >= x && rect4.getPosition().y <= y && rect4.getPosition().y + rect4.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;

        window.clear(Color(240, 240, 240));
        window.draw(rect1);
        window.draw(rect2);
        window.draw(rect3);
        window.draw(rect4);
        window.draw(text);
        window.display();

        switch(menuNum)
        {
        case 1:
            window.close();
            break;
        }
    }
}

int menuGame(window_structure winStr)
{
    RenderWindow window(VideoMode(winStr.windowWidth, winStr.windowHeight), winStr.windowName.toStdString());
    window.setTitle("Menu");

    int menuNum = 0;

    while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return 1;
            }
        }
        Font font;//шрифт
        font.loadFromFile("fonts/CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
        Text text("", font, 60);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
        text.setColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
        text.setStyle(sf::Text::Bold);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

        QString str;
        str = "Start game\n\nExit";

        text.setString(str.toStdString());
        text.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 100);

        RectangleShape rect1;
        rect1.setFillColor(Color::White);
        rect1.setSize(Vector2f(320, 70));
        rect1.setPosition(text.getPosition().x - 5, text.getPosition().y + 10);
        rect1.setOutlineColor(Color::Green);
        rect1.setOutlineThickness(3);

        RectangleShape rect2 = rect1;
        rect2.setPosition(rect1.getPosition().x, rect1.getPosition().y + 110);


        if (IntRect(rect1.getPosition().x, rect1.getPosition().y, rect1.getSize().x, rect1.getSize().y).contains(Mouse::getPosition(window)))
            rect1.setFillColor(Color::Red);
        if (IntRect(rect2.getPosition().x, rect2.getPosition().y, rect2.getSize().x, rect2.getSize().y).contains(Mouse::getPosition(window)))
            rect2.setFillColor(Color::Red);

        float x = Mouse::getPosition(window).x;
        float y = Mouse::getPosition(window).y;
        if (rect1.getPosition().x <= x && rect1.getPosition().x + rect1.getSize().x >= x && rect1.getPosition().y <= y && rect1.getPosition().y + rect1.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 1;
        if (rect2.getPosition().x <= x && rect2.getPosition().x + rect2.getSize().x >= x && rect2.getPosition().y <= y && rect2.getPosition().y + rect2.getSize().y >= y)
            if (Mouse::isButtonPressed(Mouse::Left))
                menuNum = 2;

        window.clear(Color(240, 240, 240));
        window.draw(rect1);
        window.draw(rect2);
        window.draw(text);
        window.display();

        switch(menuNum)
        {
        case 1:
            window.close();
            break;
        case 2:
            window.close();
            break;
        }
    }
    switch(menuNum)
    {
    case 1:
        return 0;
        break;
    case 2:
        return 1;
        break;
    }
}


