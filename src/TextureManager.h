//
// Created by makay on 8/2/2023.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;

class TextureManager{
    static unordered_map<string, sf::Texture> textures;
    static void loadTexture(string textureName);
public:
    static sf::Texture& getTexture(string textureName);
};
