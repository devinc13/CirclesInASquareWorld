#include <SFML/Graphics.hpp>
#include <CirclePeople.h>
#include<fstream>
#include<iostream>
#include<cctype>
#include <Thread.hpp>

using namespace std;
using namespace sf;
int main()
{
    {
        ContextSettings settings;
        settings.antialiasingLevel = 32;
        std::ifstream openfile("Map1.txt");

        sf::Vector2i map[100][100];
        sf::Vector2i loadCounter(0, 0);
        sf::Vector2i mapSize;
        sf::Texture tileTexture;
        sf::Sprite tiles;
        int counter1L = 0;
        int counter1R = 0;
        int counter2L = 0;
        int counter2R = 0;
        int counter1U = 0;
        int counter2U = 0;
        int counter1D = 0;
        int counter2D = 0;
        int falling1 = 0;
        int falling2 = 0;
        bool allowUp1 = false;
        bool allowUp2 = false;
        bool allowDown1 = false;
        bool allowDown2 = false;
        bool allowRight1 = false;
        bool allowRight2 = false;
        bool allowLeft1 = false;
        bool allowLeft2 = false;

        if(openfile.is_open())
        {
            std::string tileLocation;
            openfile >> tileLocation;

            Image tilesImg;
            if (!tilesImg.loadFromFile(tileLocation))
            {
                printf("Load tile texture error!\n");
            }
            tilesImg.createMaskFromColor(Color(160,0,160),0);
            tileTexture.loadFromImage(tilesImg);
            tiles.setTexture(tileTexture);

            while(!openfile.eof())
            {
                std::string tile;
                openfile >> tile;
                char x = tile[0], y = tile[2];
                if(!isdigit(x) || !isdigit(y))
                {
                    map[loadCounter.x][loadCounter.y] = sf::Vector2i(-1, -1);
                }
                else
                {
                    map[loadCounter.x][loadCounter.y] = sf::Vector2i(x - '0', y - '0');
                }
                if(openfile.peek() == '\n')
                {
                    loadCounter.x = 0;
                    loadCounter.y++;
                    std::cout << std::endl;
                }
                else    loadCounter.x++;
            }
            mapSize = loadCounter;
            mapSize.y++;
        }

        sf::RenderWindow Window(sf::VideoMode(1024, 480, 32), "We are on our way.");
        Window.setFramerateLimit(100);
        View ourView = Window.getView();
        ourView.setCenter(512,240);
        Window.setView(ourView);

        Texture texture;
        Image img;
        if (!img.loadFromFile("sprites.png"))
        {
            printf("Load player texture error!\n");
        }
        img.createMaskFromColor(Color(160,0,160),0);
        texture.loadFromImage(img);
        texture.setSmooth(true);
        CirclePeople player1(Vector2f(64,352), Vector2f(0,32));
        player1.sprite.setTexture(texture);

        CirclePeople player2(Vector2f(32,352), Vector2f(0,96));
        player2.sprite.setTexture(texture);

        while(Window.isOpen())
        {
            sf::Event Event;
            while(Window.pollEvent(Event))
            {
                switch (Event.type)
                {
                case sf::Event::Closed:
                    Window.close();
                    break;
                }
            }

            Window.clear(Color(82,176,237));
            int pos1y = player1.sprite.getPosition().y;
            int pos2y = player2.sprite.getPosition().y;
            int posavgy = (pos1y+pos2y)/2;
            ourView.setCenter(512,posavgy);
            Window.setView(ourView);
            for(int i = 0; i < mapSize.x; i++)
            {
                for(int j = 0; j < mapSize.y; j++)
                {
                    if(map[i][j].x != -1 && map[i][j].y != -1)
                    {
                        tiles.setPosition(i * 32, j * 32);
                        tiles.setTextureRect(sf::IntRect(map[i][j].x * 32, map[i][j].y * 32, 32, 32));
                        Window.draw(tiles);
                    }
                }
            }
            if (counter1L==0&&counter1R==0&&counter1U==0&&counter1D==0&&falling1==0)
            {
                if(Keyboard::isKeyPressed(Keyboard::SemiColon))//place ladder when on same square
                {
                    player1.setLadder(1,map);
                }
                if(Keyboard::isKeyPressed(Keyboard::L))//pick up ladder when on same square
                {
                    player1.getLadder(1,map);
                }
            }
            if (counter2L==0&&counter2R==0&&counter2U==0&&counter2D==0&&falling2==0)
            {
                if(Keyboard::isKeyPressed(Keyboard::C))
                {
                    player2.getLadder(2,map);
                }
                if(Keyboard::isKeyPressed(Keyboard::V))
                {
                    player2.setLadder(2,map);
                }
            }

            if ((counter1L==0&&counter1R==0&&counter1U==0&&counter1D==0)||(falling1>0))
            {
                falling1 = player1.checkFall(Window,mapSize,map,tiles,falling1);
            }
            if ((counter2L==0&&counter2R==0&&counter2U==0&&counter2D==0)||(falling2>0))
            {
                falling2 = player2.checkFall(Window,mapSize,map,tiles,falling2);
            }
            if (counter1L==0&&counter1R==0&&counter1U==0&&counter1D==0)
            {
                if(Keyboard::isKeyPressed(Keyboard::Right))
                {
                    counter1R = 1;
                    player1.facing = 'r';
                }
                else if(Keyboard::isKeyPressed(Keyboard::Left))
                {
                    counter1L = 1;
                    player1.facing = 'l';
                }
                else if(Keyboard::isKeyPressed(Keyboard::Up))
                {
                    counter1U = 1;
                }
                else if(Keyboard::isKeyPressed(Keyboard::Down)&&falling1==0)
                {
                    counter1D = 1;
                }
            }

            if (counter2L==0&&counter2R==0&&counter2U==0&&counter2D==0)
            {
                if(Keyboard::isKeyPressed(Keyboard::D))
                {
                    counter2R = 1;
                    player2.facing = 'r';
                }
                else if(Keyboard::isKeyPressed(Keyboard::A))
                {
                    counter2L = 1;
                    player2.facing = 'l';
                }
                else if(Keyboard::isKeyPressed(Keyboard::W))
                {
                    counter2U = 1;
                }
                else if(Keyboard::isKeyPressed(Keyboard::S)&&falling2==0)
                {
                    counter2D = 1;
                }
            }

            if (counter1U!=0)
            {
                counter1U++;
                allowUp1 = player1.climbUp(Window,mapSize,map,tiles,counter1U);
                if (!allowUp1) counter1U = 0;
                if (counter1U == 33) counter1U = 0;
            }
            if (counter2U!=0)
            {
                counter2U++;
                allowUp2 = player2.climbUp(Window,mapSize,map,tiles,counter2U);
                if (!allowUp2) counter2U = 0;
                if (counter2U == 33) counter2U = 0;
            }
            if (counter1D!=0)
            {
                counter1D++;
                allowDown1 = player1.climbDown(Window,mapSize,map,tiles,counter1D);
                if (!allowDown1) counter1D = 0;
                if (counter1D == 33) counter1D = 0;
            }
            if (counter2D!=0)
            {
                counter2D++;
                allowDown2 = player2.climbDown(Window,mapSize,map,tiles,counter2D);
                if (!allowDown2) counter2D = 0;
                if (counter2D == 33) counter2D = 0;
            }
            if (counter1R!=0)
            {
                counter1R++;
                allowRight1 = player1.walkRight(Window,mapSize,map,tiles,counter1R,falling1);
                if (!allowRight1) counter1R = 0;
                if (counter1R == 33) counter1R = 0;
            }
            if (counter1L!=0)
            {
                counter1L++;
                allowLeft1 = player1.walkLeft(Window,mapSize,map,tiles,counter1L,falling1);
                if (!allowLeft1) counter1L = 0;
                if (counter1L == 33) counter1L = 0;
            }
            if (counter2R!=0)
            {
                counter2R++;
                allowRight2 = player2.walkRight(Window,mapSize,map,tiles,counter2R,falling2);
                if (!allowRight2) counter2R = 0;
                if (counter2R == 33) counter2R = 0;
            }
            if (counter2L!=0)
            {
                counter2L++;
                allowLeft2 = player2.walkLeft(Window,mapSize,map,tiles,counter2L,falling2);
                if (!allowLeft2) counter2L = 0;
                if (counter2L == 33) counter2L = 0;
            }



            Window.draw(player1.sprite);
            Window.draw(player2.sprite);
            Window.display();
        }

        return 0;
    }
}
