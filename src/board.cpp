//
// Created by makay on 7/28/2023.
//

#include "board.h"
using namespace std;

Board::Board(int columns, int rows, int mines, string name){
    this->columns = columns;
    this->rows = rows;
    this->mines = mines;
    this->name = name;
    remaining = mines;
    //fill the vector full of default tiles
    for (int i = 0; i < columns*rows; i++){
        Tile temp(i%columns, i/columns);
        tiles.push_back(temp);
    }
    //set the mines and number of nearby mines
    setMines();
    numNearMines();
    start = chrono::high_resolution_clock::now();
}

void Board::setMines(){
    //chooses a random number and checks if that index is a mine
    int i;
    for(int a = 0; a < mines; a++) {
        i = rand() % (rows * columns);
        if (tiles[i].mine){
            a--; //do it again if it already is
        }
        else{
            //set it to true if it isn't
            tiles[i].mine = true;
        }
    }
}

//this works yippee
void Board::numNearMines(){
    //check all locations around a tile for mines and add it up
    for (int i = 0; i < tiles.size(); i++) {
        //up
        if (i/columns > 0){
            if (tiles[i-columns].mine){
                tiles[i].nearbyMines++;
            }
            //up left
            if (i%columns > 0){
                if (tiles[i-columns-1].mine){
                    tiles[i].nearbyMines++;
                }
            }
            //up right
            if (i%columns < columns-1){
                if (tiles[i-columns+1].mine){
                    tiles[i].nearbyMines++;
                }
            }
        }
        //down
        if (i/columns < rows-1){
            if (tiles[i+columns].mine){
                tiles[i].nearbyMines++;
            }
            //down left
            if (i%columns > 0){
                if (tiles[i+columns-1].mine){
                    tiles[i].nearbyMines++;
                }
            }
            //down right
            if (i%columns < columns-1){
                if (tiles[i+columns+1].mine){
                    tiles[i].nearbyMines++;
                }
            }
        }
        //left
        if (i%columns > 0){
            if (tiles[i-1].mine){
                tiles[i].nearbyMines++;
            }
        }
        //right
        if (i%columns < columns-1){
            if (tiles[i+1].mine){
                tiles[i].nearbyMines++;
            }
        }
    }
}

//this works yippee
void Board::rightClick(float X, float Y){
    //make sure it can be applied
    if (!paused && !winState && !loseState && !lb) {
        int x = (int) X / 32;
        int y = (int) Y / 32;
        int i = ((y * columns) + x);

        //make sure it is on a tile
        if (Y < (float) rows * 32) {

            //apply flag
            if (!tiles[i].revealed) {
                if (tiles[i].flag) {
                    flagged--;
                    remaining++;
                } else {
                    flagged++;
                    remaining--;
                }
                tiles[i].setFlag();
            }
        }
    }
}

void Board::leftClick(float X, float Y){
    if (!lb) { //should not be able to click anything while leaderboard is open
        if (!paused) { //can not do anything if paused
            if (Y < (float) rows * 32) {
                checkTile(X, Y);
                gameState();
            }
            if (smileyFace.getGlobalBounds().contains(X, Y)) {
                reset();
            }
            if (!winState && !loseState && !paused) {
                if (debug.getGlobalBounds().contains(X, Y)) {
                    debugged = !debugged;
                }
            }
            if (leader.getGlobalBounds().contains(X, Y)) {
                lb = true;
                if (!winState && !loseState){
                    paused = true;
                }
            }
        }
        if (!winState && !loseState) {
            if (pause.getGlobalBounds().contains(X, Y)) {
                paused = !paused;
                setPause();
            }
        }
    }
}

void Board::checkTile(float X, float Y) {
    int x = (int) X / 32;
    int y = (int) Y / 32;
    int i = ((y * columns) + x);

    //dont if nothing is supposed to happen
    if (!paused && !winState && !loseState) {

        //reveal if good to go
        if (!tiles[i].revealed && !tiles[i].flag) {
            revealed++;
            tiles[i].revealTile();

            //reveal all those around if there are no nearby mines
            if (tiles[i].nearbyMines == 0 && !tiles[i].mine) {
                revealNear(X, Y);
            }
        }
    }
}


//ok I literally changed nothing, but it works now I guess
void Board::revealNear(float X, float Y){
    int x = (int)X/32;
    int y = (int)Y/32;

    //check each direction
    if (y > 0) {
        checkTile(X, Y - 32);
        if (x > 0){
            checkTile(X-32, Y-32);
        }
        if (x < columns-1){
            checkTile(X+32, Y-32);
        }
    }
    if (x > 0){
        checkTile(X-32, Y);
    }
    if (x < columns-1){
        checkTile(X+32, Y);
    }
    if (y < rows-1){
        checkTile(X, Y+32);
        if (x > 0){
            checkTile(X-32, Y+32);
        }
        if (x < columns-1){
            checkTile(X+32, Y+32);
        }
    }
}

//draw everything
void Board::Draw(sf::RenderWindow& window){
    for (auto & tile : tiles){
        tile.Draw(window, paused, debugged, loseState, winState);
    }

    //calculate each position
    int d1 = abs(remaining)/100;
    int d2 = (abs(remaining)%100)/10;
    int d3 = (abs(remaining)%100)%10;

    digit1.setTexture(TextureManager::getTexture("digits"));
    digit2.setTexture(TextureManager::getTexture("digits"));
    digit3.setTexture(TextureManager::getTexture("digits"));
    neg.setTexture(TextureManager::getTexture("digits"));

    digit1.setTextureRect(sf::IntRect(d1*21, 0, 21, 32));
    digit2.setTextureRect(sf::IntRect(d2*21, 0, 21, 32));
    digit3.setTextureRect(sf::IntRect(d3*21, 0, 21, 32));
    neg.setTextureRect(sf::IntRect(210, 0,21, 32));

    digit1.setPosition(33, 32 * ((float)rows + 0.5f) + 16);
    digit2.setPosition(54, 32 * ((float)rows + 0.5f) + 16);
    digit3.setPosition(75, 32 * ((float)rows + 0.5f) + 16);
    neg.setPosition(12, 32 * ((float)rows + 0.5f) + 16);

    window.draw(digit1);
    window.draw(digit2);
    window.draw(digit3);
    if (remaining < 0){
        window.draw(neg);
    }

    debug.setTexture(TextureManager::getTexture("debug"));
    debug.setPosition((float)(columns * 32) - 304, ((float)rows + 0.5f) * 32);
    window.draw(debug);

    if (paused){
        pause.setTexture(TextureManager::getTexture("play"));
    }
    else{
        pause.setTexture(TextureManager::getTexture("pause"));
    }
    pause.setPosition((float)(columns * 32) - 240, ((float)rows + 0.5f) * 32);
    window.draw(pause);

    leader.setTexture(TextureManager::getTexture("leaderboard"));
    leader.setPosition((float)(columns * 32) - 176, ((float)rows + 0.5f) * 32);
    window.draw(leader);

    if (winState){
        smileyFace.setTexture(TextureManager::getTexture("face_win"));
    }
    else if (loseState){
        smileyFace.setTexture(TextureManager::getTexture("face_lose"));
    }
    else{
        smileyFace.setTexture(TextureManager::getTexture("face_happy"));
    }
    smileyFace.setPosition((float)(columns/2.0) * 32, ((float)rows + 0.5f) * 32);
    window.draw(smileyFace);

    //consistently updates the timer
    if (!winState && !loseState) {
        setTimer();
    }
    int m1 = abs((numSeconds/60)/10);
    int m2 = abs((numSeconds/60)%10);
    int s1 = abs((numSeconds%60)/10);
    int s2 = abs((numSeconds%60)%10);

    min1.setTexture(TextureManager::getTexture("digits"));
    min2.setTexture(TextureManager::getTexture("digits"));
    sec1.setTexture(TextureManager::getTexture("digits"));
    sec2.setTexture(TextureManager::getTexture("digits"));

    min1.setTextureRect(sf::IntRect(m1*21, 0, 21, 32));
    min2.setTextureRect(sf::IntRect(m2*21, 0, 21, 32));
    sec1.setTextureRect(sf::IntRect(s1*21, 0, 21, 32));
    sec2.setTextureRect(sf::IntRect(s2*21, 0, 21, 32));

    min1.setPosition((float)(columns * 32) - 97, 32 * ((float)rows + 0.5f) + 16);
    min2.setPosition((float)(columns * 32) - 76, 32 * ((float)rows + 0.5f) + 16);
    sec1.setPosition((float)(columns * 32) - 54, 32 * ((float)rows + 0.5f) + 16);
    sec2.setPosition((float)(columns * 32) - 33, 32 * ((float)rows + 0.5f) + 16);

    window.draw(min1);
    window.draw(min2);
    window.draw(sec1);
    window.draw(sec2);
}

void Board::gameState() {
    //check the game state and if it is over in any way
    int count = 0;
    for (auto & tile : tiles){
        if (tile.revealed && tile.mine){
            loseState = true;
        }
        if (tile.revealed && !tile.mine){
            count++;
        }
    }
    //get the number revealed and see if it is all of them
    if (count == (columns*rows) - mines){
        winState = true;
    }
    if (winState){ //if it is won, add the name and time if applicable
        leaderboard.addLine(name, numSeconds);
    }
}

void Board::reset(){
    //reset everything for new board
    for (int i = 0; i < rows*columns; i++){
        tiles.clear();
    }
    winState = false;
    loseState = false;
    revealed = 0;
    flagged = 0;
    for (int i = 0; i < columns*rows; i++){
        Tile temp((i%columns), (i/columns));
        tiles.push_back(temp);
    }
    setMines();
    numNearMines();
    remaining = mines;
    numSeconds = 0;
    numPaused = 0;
    start = chrono::high_resolution_clock::now();
}

void Board::setTimer(){
    //if not paused, update the total time passed, if it is paused, nothing will be changed
    if (!paused) {
        auto end = chrono::high_resolution_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(end - start);

        int num = elapsedTime.count();
        numSeconds = num - numPaused;
    }
}

void Board::setPause() {
    if (paused) { //if it is now paused, track the start time
        startPause = chrono::high_resolution_clock::now();
    }
    else { //if no longer paused, track end time and find total time paused
        endPause = chrono::high_resolution_clock::now();
        auto pauseTime = chrono::duration_cast<chrono::seconds>(endPause - startPause);
        numPaused += pauseTime.count();
    }
}

void Board::leaderHelper() {
    //if lb is pressed, set the pause timer and then open the window
    if (lb) {

        if (paused) {
            setPause();
        }

        //will return true if it is closed
        if (leaderboard.createBoard(columns, rows)) {
            //once closed, set to false and get the time
            lb = false;
            paused = false;
            setPause();
        }
    }
}

