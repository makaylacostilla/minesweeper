//
// Created by makay on 7/28/2023.
//

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "tile.h"
#include "leaderboard.h"
using namespace std;

class Board{
    sf::Sprite debug;
    sf::Sprite smileyFace;
    sf::Sprite pause;
    sf::Sprite leader;
    sf::Sprite digit1;
    sf::Sprite digit2;
    sf::Sprite digit3;
    sf::Sprite neg;
    sf::Sprite min1;
    sf::Sprite min2;
    sf::Sprite sec1;
    sf::Sprite sec2;
    bool winState = false;
    bool loseState = false;
    bool debugged = false;
    bool paused = false;
    bool lb = false;
    int revealed = 0;
    int flagged = 0;
    int numPaused = 0;
    int numSeconds = 0;
    int remaining;
    int columns;
    int rows;
    int mines;
    string name;
    LeaderBoard leaderboard;
    chrono::time_point<chrono::system_clock> start;
    chrono::time_point<chrono::system_clock> startPause;
    chrono::time_point<chrono::system_clock> endPause;

public:
    vector<Tile> tiles;
    Board(int columns, int rows, int mines, string name);
    void setMines();
    void setTimer();
    void setPause();
    void numNearMines();
    void rightClick(float X, float Y);
    void leftClick(float X, float Y);
    void checkTile(float X, float Y);
    void revealNear(float X, float Y);
    void Draw(sf::RenderWindow& window);
    void gameState();
    void reset();
    void leaderHelper();
};
