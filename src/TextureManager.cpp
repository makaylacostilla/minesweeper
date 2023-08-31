//
// Created by makay on 8/2/2023.
//

#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::loadTexture(string fileName){
    string path = "files/images/";
    path += fileName + ".png";
    textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::getTexture(string textureName){
    if (textures.find(textureName) == textures.end()){
        loadTexture(textureName);
    }
    return textures[textureName];
}