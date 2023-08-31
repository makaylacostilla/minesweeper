//
// Created by makay on 8/6/2023.
//

#include "leaderboard.h"
using namespace std;

void LeaderBoard::setText(sf::Text &text, float x, float y)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x,y));
}

bool LeaderBoard::createBoard(int columns, int rows) {
    float width = (float)columns * 16;
    float height = ((float)rows * 16) + 50;
    sf::RenderWindow leaderboard(sf::VideoMode((int)width, (int)height), "Leaderboard", sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")){
        cout << "uh oh" << endl;
    }

    sf::Text lb("LEADERBOARD", font, 20);
    lb.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(lb, width, height);
    lb.setPosition(width/2.0f, (height/2.0f) - 120);

    //get the names in the file
    vector<string> players = readBoard();
    vector<string> format;
    string name;
    string time;
    string player;

    //format said names into one big string
    for (int i = 0; i < players.size(); i++){
        int index = (int)players[i].find(',');
        name = players[i].substr(index+1);
        time = players[i].substr(0,index);
        player = to_string(i+1);
        player += ".\t";
        player += time;
        player += "\t";
        player += name;
        format.push_back(player);
    }
    string list = format[0];
    for (int i = 1; i < players.size(); i++){
        list += "\n\n" + format[i];
    }

    //draw the string
    sf::Text playerList(list, font, 18);
    playerList.setStyle(sf::Text::Bold);
    setText(playerList, width, height);
    playerList.setPosition(width/2.0f, (height/2.0f) + 20);

    while (leaderboard.isOpen()){
        sf::Event event{};
        while (leaderboard.pollEvent(event)) {
            leaderboard.requestFocus();
            if (event.type == sf::Event::Closed) {
                leaderboard.close();
                return true;
            }
        }
        leaderboard.clear(sf::Color::Blue);
        leaderboard.draw(lb);
        leaderboard.draw(playerList);
        leaderboard.display();
    }
}

vector<string> LeaderBoard::readBoard() {
    vector<string> players;
    ifstream file("files/leaderboard.txt");
    string line;
    while (file.good()){
        getline(file, line);
        if (!line.empty()){
            players.push_back(line);
        }
    }
    return players;
}

void LeaderBoard::addLine(string name, int time){
    vector<string> players = readBoard();

    //turn the integer into the time
    int min1 = (time/60)/10;
    int min2 = (time/60)%10;
    int sec1 = (time%60)/10;
    int sec2 = (time%60)%10;
    string sTime = to_string(min1) + to_string(min2);
    sTime += ':';
    sTime += to_string(sec1) + to_string(sec2);
    string player = sTime + ',' + name + '*';

    //add the new info and sort it in
    players.push_back(player);
    std::sort(players.begin(), players.end());

    //writing only the first 5
    ofstream file("files/temp.txt");
    for (int i = 0; i < 5; i++){
        file << players[i] + "\n";
    }
    file.close();

    //replace with new file each time to keep it to only 5 people
    remove("files/leaderboard.txt");
    rename("files/temp.txt", "files/leaderboard.txt");
}
