#ifndef CirclePeople_H
#define CirclePeople_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>

using namespace sf;

class CirclePeople : public Drawable
{
public:
    Sprite sprite;
    Vector2f spritepos;
    CirclePeople(Vector2f position, Vector2f startload);
    char facing;
    virtual ~CirclePeople();
    bool climbUp(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i);
    bool climbDown(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i);
    int checkFall(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i);
    bool walkRight(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i,int falling);
    bool walkLeft(RenderWindow& window,Vector2i mapSize,Vector2i map[100][100],Sprite tiles,int i,int falling);
    void getLadder(int player,Vector2i map[100][100]);
    void setLadder(int player,Vector2i map[100][100]);
protected:
private:
    bool hasLadder;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // You can draw other high-level objects
        target.draw(m_sprite, states);
        // ... or use the low-level API
        states.texture = &m_texture;
        target.draw(m_vertices, states);
    }
    Sprite m_sprite;
    Texture m_texture;
    VertexArray m_vertices;

};

#endif // CirclePeople_H
