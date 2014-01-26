#include <SFML/Graphics.hpp>
#include <CirclePeople.h>
#include<fstream>
#include<iostream>
#include<cctype>


using namespace std;
using namespace sf;

CirclePeople::CirclePeople(Vector2f position, Vector2f startload)
{
    sprite.setPosition(position);
    sprite.setTextureRect(IntRect(startload.x, startload.y, 32,32));
    spritepos = startload;
    hasLadder = false;
    facing = 'r';
}

CirclePeople::~CirclePeople()
{
    //dtor
}

bool CirclePeople::climbUp(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i)
{
    int x = sprite.getPosition().x/32;
    int y = sprite.getPosition().y/32;

    if (map[x][y].y==0&&i==2&&(map[x][y-1].y==0||map[x][y-1].y==-1||map[x][y-1].y==2))
    {
        sprite.move(0,-1);
        return true;
    }
    if (i>2)
    {
        sprite.move(0,-1);
        if (i==3)sprite.setTextureRect(IntRect(spritepos.x+128, spritepos.y-32, 32,32));
        if (i==16)sprite.setTextureRect(IntRect(spritepos.x+96, spritepos.y-32, 32,32));
        if (i==32)sprite.setTextureRect(IntRect(spritepos.x+128, spritepos.y-32, 32,32));
        if (i==33&&map[x][y].y==-1||i==32&&map[x][y].y==2)
            {
                switch(facing){
                    case'l': sprite.setTextureRect(IntRect(spritepos.x, spritepos.y-32, 32,32)); break;
                    case'r': sprite.setTextureRect(IntRect(spritepos.x, spritepos.y, 32,32)); break;
                        }
            }
        return true;
    }
    return false;
}

bool CirclePeople::climbDown(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i)
{
    int x = sprite.getPosition().x/32;
    int y = sprite.getPosition().y/32;

    if (i==2&&(map[x][y+1].y==0||map[x][y+1].y==-1&&map[x][y].y==0||map[x][y+1].y==2&&map[x][y].y==0))
    {
        sprite.move(0,1);
        return true;
    }
    if (i>2)
    {
        sprite.move(0,1);
        if (i==3)sprite.setTextureRect(IntRect(spritepos.x+128, spritepos.y-32, 32,32));
        if (i==16)sprite.setTextureRect(IntRect(spritepos.x+96, spritepos.y-32, 32,32));
        if (i==32)sprite.setTextureRect(IntRect(spritepos.x+128, spritepos.y-32, 32,32));
        return true;
    }
    return false;
}

int CirclePeople::checkFall(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i)
{
    int x = sprite.getPosition().x/32;
    int y = sprite.getPosition().y/32;

    if (i==0&&map[x][y].y!=0&&(map[x][y+1].y==0||map[x][y+1].y==1)){
            switch(facing)
            {
                case'l': sprite.setTextureRect(IntRect(spritepos.x, spritepos.y-32, 32,32)); break;
                case'r': sprite.setTextureRect(IntRect(spritepos.x, spritepos.y, 32,32)); break;
            }
        }

    if ((map[x][y+1].y==-1||map[x][y+1].y==2)&&map[x][y].y!=0&&i==0)
    {
        sprite.move(0,1);
        switch(facing)
            {
                case'l': sprite.setTextureRect(IntRect(spritepos.x+128, spritepos.y, 32,32)); break;
                case'r': sprite.setTextureRect(IntRect(spritepos.x+96, spritepos.y, 32,32)); break;
            }
        i++;
    }
    if (i>0)
    {
        sprite.move(0,1);
        i++;
    }
    if (i==32)
    {
        i=0;
    }
    return i;
}

bool CirclePeople::walkRight(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i,int falling)
{
    int x = sprite.getPosition().x/32;
    int y = sprite.getPosition().y;
    if (falling>0)sprite.setTextureRect(IntRect(spritepos.x+96, spritepos.y, 32,32));
    if (y%32==0)
    {
        y = y/32;

        if(map[x+1][y].y!=1&&i==2)
        {
            sprite.move(1,0);
            return true;
        }
        if (i>2)
        {
            sprite.move(1,0);
            if (i==11&&falling==0&&(map[x][y+1].y==0||map[x][y+1].y==1))sprite.setTextureRect(IntRect(spritepos.x+32, spritepos.y, 32,32));
            if (i==22&&falling==0&&(map[x][y+1].y==0||map[x][y+1].y==1))sprite.setTextureRect(IntRect(spritepos.x+64, spritepos.y, 32,32));
            if (i==32&&falling==0&&(map[x][y+1].y==0||map[x][y+1].y==1))sprite.setTextureRect(IntRect(spritepos.x, spritepos.y, 32,32));
            return true;
        }
        return false;
    }
    else
    {
        y = (y/32);

        if(map[x+1][y].y!=1&&map[x+1][y+1].y!=1&&i==2)
        {
            sprite.move(1,0);
            return true;
        }
        if (i>2)
        {
            sprite.move(1,0);
            if (i==11&&falling==0&&map[x][y+1].y!=-1&&map[x][y+1].y!=3)sprite.setTextureRect(IntRect(spritepos.x+32, spritepos.y, 32,32));
            if (i==22&&falling==0&&map[x][y+1].y!=-1&&map[x][y+1].y!=3)sprite.setTextureRect(IntRect(spritepos.x+64, spritepos.y, 32,32));
            if (i==32&&falling==0&&map[x][y+1].y!=-1&&map[x][y+1].y!=3)sprite.setTextureRect(IntRect(spritepos.x, spritepos.y, 32,32));
            return true;
        }
        return false;
    }
}

bool CirclePeople::walkLeft(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i,int falling)
{
    int x = sprite.getPosition().x/32;
    int y = sprite.getPosition().y;
    if (falling>0)sprite.setTextureRect(IntRect(spritepos.x+128, spritepos.y, 32,32));
    if (y%32==0)
    {
        y = y/32;
        if(map[x-1][y].y!=1&&i==2)
        {
            sprite.move(-1,0);
            return true;
        }
        if (i>2)
        {
            sprite.move(-1,0);
            if (i==11&&falling==0&&(map[x][y+1].y==0||map[x][y+1].y==1))sprite.setTextureRect(IntRect(spritepos.x+32, spritepos.y-32, 32,32));
            if (i==22&&falling==0&&(map[x][y+1].y==0||map[x][y+1].y==1))sprite.setTextureRect(IntRect(spritepos.x+64, spritepos.y-32, 32,32));
            if (i==32&&falling==0&&(map[x][y+1].y==0||map[x][y+1].y==1))sprite.setTextureRect(IntRect(spritepos.x, spritepos.y-32, 32,32));
            return true;
        }
        return false;
    }
    else
    {
        y = (y/32);
        if(map[x-1][y].y!=1&&map[x-1][y+1].y!=1&&i==2)
        {
            sprite.move(-1,0);
            return true;
        }
        if (i>2)
        {
            sprite.move(-1,0);
            if (i==11&&falling==0&&map[x][y+1].y!=-1&&map[x][y+1].y!=3)sprite.setTextureRect(IntRect(spritepos.x+32, spritepos.y-32, 32,32));
            if (i==22&&falling==0&&map[x][y+1].y!=-1&&map[x][y+1].y!=3)sprite.setTextureRect(IntRect(spritepos.x+64, spritepos.y-32, 32,32));
            if (i==32&&falling==0&&map[x][y+1].y!=-1&&map[x][y+1].y!=3)sprite.setTextureRect(IntRect(spritepos.x, spritepos.y-32, 32,32));
            return true;
        }
        return false;
    }
}
void CirclePeople::getLadder(int player,Vector2i map[100][100])
{
    int x = sprite.getPosition().x/32;
    int y = sprite.getPosition().y/32;

    if (!hasLadder)
    {
        //pick up ladder if on ladder square and don't have ladder
        switch (player)
        {
        case 1:
            if (map[x][y].x==2&&map[x][y].y==0)
            {
                map[x][y].x = -1;
                map[x][y].y = -1;
                hasLadder = true;
            }
            break;
        case 2:
            if (map[x][y].x==1&&map[x][y].y==0)
            {
                map[x][y].x = -1;
                map[x][y].y = -1;
                hasLadder = true;
            }
            break;
        }
    }
}

void CirclePeople::setLadder(int player,Vector2i map[100][100])
{
    int x = sprite.getPosition().x/32;
    int y = sprite.getPosition().y/32;

    if (hasLadder)
    {
        //place ladder if you have one
        switch (player)
        {
        case 1:
            if (map[x][y].x==-1&&map[x][y].y==-1)
            {
                map[x][y].x = 2;
                map[x][y].y = 0;
                hasLadder = false;
            }
            break;
        case 2:
            if (map[x][y].x==-1&&map[x][y].y==-1)
            {
                map[x][y].x = 1;
                map[x][y].y = 0;
                hasLadder = false;
            }
            break;
        }
    }
}
