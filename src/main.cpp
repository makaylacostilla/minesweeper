#include <iostream>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "board.h"
using namespace std;

void setText(sf::Text &text, float x, float y)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x,y));
}

//reads the file
void readConfig(int& height, int& width, int& mines){
    string tWidth;
    string tHeight;
    string tMines;
    ifstream file("files/board_config.cfg");
    if (file.is_open())
    {
        getline(file, tWidth);
        getline(file, tHeight);
        getline(file, tMines);
        width = stoi(tWidth);
        height = stoi(tHeight);
        mines = stoi(tMines);
        file.close();
    }
}

string createWelcome(float height, float width){
    //create the window
    sf::RenderWindow welcome(sf::VideoMode((int)width, (int)height), "Welcome Window", sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")){
        cout << "uh oh" << endl;
    }

    sf::Text welcomeText("WELCOME TO MINESWEEPER", font, 24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width, height);
    welcomeText.setPosition(width/2.0f, (height/2.0f) - 150);

    sf::Text namePrompt("Enter your name:", font, 20);
    namePrompt.setStyle(sf::Text::Bold);
    setText(namePrompt, width, height);
    namePrompt.setPosition(width/2.0f, (height/2.0f) - 75);

    sf::Text output("|", font, 18);
    output.setFillColor(sf::Color::Yellow);
    output.setStyle(sf::Text::Bold);
    setText(output, width, height);
    output.setPosition(width/2.0f, (height/2.0f) - 45);
    string input;

    while (welcome.isOpen()){
        sf::Event event{};
        while (welcome.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                welcome.close();
                //if closed return empty string so that it know not to start the game
                return "";
            }
            if (event.type == sf::Event::TextEntered){
                if (input.length() < 10 && isalpha(event.text.unicode)){
                    if (input.length() == 0) {
                        input.push_back(toupper(event.text.unicode));
                    }
                    else {
                        input.push_back(tolower(event.text.unicode));
                    }
                    output.setString(input + "|");
                    setText(output, width, height);
                    output.setPosition(width/2.0f, (height/2.0f) - 45);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && input.length() != 0){
                    input.pop_back();
                    output.setString(input + "|");
                    setText(output, width, height);
                    output.setPosition(width/2.0f, (height/2.0f) - 45);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && input.length() != 0){
                    welcome.close();
                    return input;
                }
            }
        }
        welcome.clear(sf::Color::Blue);
        welcome.draw(welcomeText);
        welcome.draw(namePrompt);
        welcome.draw(output);
        welcome.display();
    }
}

int main()
{
    int rows = 0;
    int columns = 0;
    int mines = 0;
    readConfig(rows, columns, mines);
    float height = (float)(rows * 32) + 100;
    float width = (float)columns * 32;

    //get the input from the welcome string and check if anything was entered
    string name = createWelcome(height, width);
    if (name == ""){
        return 0;
    }
    else {
        //play game
        Board board(columns, rows, mines, name);
        sf::RenderWindow gameWindow(sf::VideoMode(columns * 32, (rows * 32) + 100), "Minesweeper", sf::Style::Close);
        while (gameWindow.isOpen()) {
            sf::Event event{};
            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                }
                //deal with each possible event
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        board.leftClick(event.mouseButton.x, event.mouseButton.y);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        board.rightClick(event.mouseButton.x, event.mouseButton.y);
                    }
                }
            }
            gameWindow.clear(sf::Color::White);
            board.Draw(gameWindow);
            gameWindow.display();

            //check if leaderboard has been opened
            board.leaderHelper();
        }
    }
}

