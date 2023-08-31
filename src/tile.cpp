//
// Created by makay on 7/28/2023.
//

#include "tile.h"
using namespace std;

Tile::Tile(int x, int y){
    this->x = x;
    this->y = y;
    mine = false;
    flag = false;
    revealed = false;
    nearbyMines = 0;
}

void Tile::setFlag(){
    if (!revealed){
        flag = !flag;
    }
}

void Tile::Draw(sf::RenderWindow& window, bool paused, bool debugging, bool lost, bool won) {
    float X = (float) x * 32;
    float Y = (float) y * 32;
    if (!paused) {
        if (revealed) { //all possible revealed combinations
            bottom.setTexture(TextureManager::getTexture("tile_revealed"));
            bottom.setPosition(X, Y);
            window.draw(bottom);
            if (mine){
                middle.setTexture(TextureManager::getTexture("mine"));
                middle.setPosition(X, Y);
                window.draw(middle);
            }
            if (nearbyMines != 0 && !mine) {
                middle.setTexture(TextureManager::getTexture("number_" + to_string(nearbyMines)));
                middle.setPosition(X, Y);
                window.draw(middle);
            }
        }
        else { //all possible hidden combinations
            bottom.setTexture(TextureManager::getTexture("tile_hidden"));
            bottom.setPosition(X, Y);
            window.draw(bottom);
            if (flag) {
                middle.setTexture(TextureManager::getTexture("flag"));
                middle.setPosition(X, Y);
                window.draw(middle);
            }
        }
    } //paused just has revealed tiles
    if (paused) {
        top.setTexture(TextureManager::getTexture("tile_revealed"));
        top.setPosition(X, Y);
        window.draw(top);
    }
    if (debugging && mine) { //show the mines on top
        top.setTexture(TextureManager::getTexture("mine"));
        top.setPosition(X, Y);
        window.draw(top);
    }
    if (lost && mine) { //show the mines
        top.setTexture(TextureManager::getTexture("mine"));
        top.setPosition(X, Y);
        window.draw(top);
    }
    if (won && mine){ //place flag on mines if won
        top.setTexture(TextureManager::getTexture("flag"));
        top.setPosition(X, Y);
        window.draw(top);
    }
}

void Tile::revealTile(){
    //can not reveal a tile with a flag
    if (!flag){
        revealed = true;
    }
}