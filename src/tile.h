//
// Created by makay on 7/28/2023.
//

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;

class Tile{
public:
    sf::Sprite top;
    sf::Sprite middle;
    sf::Sprite bottom;
    bool mine = false;
    bool flag = false;
    bool revealed = false;
    int nearbyMines = 0;
    int x;
    int y;

    Tile(int x, int y);
    void setFlag();
    void Draw(sf::RenderWindow& window, bool paused, bool debugging, bool lost, bool won);
    void revealTile();
};
