//
// Created by makay on 8/6/2023.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
using namespace std;

class LeaderBoard{
public:
    static void setText(sf::Text &text, float x, float y);
    void addLine(string name, int time);
    static vector<string> readBoard();
    static bool createBoard(int columns, int rows);
};
