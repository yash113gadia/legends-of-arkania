#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

class Map {
private:
    std::vector<std::vector<char>> grid;
    int width;
    int height;
    std::string regionName;
    std::string filename;
    
    char getTile(int x, int y) const;
    void setTile(int x, int y, char tile);

public:
    Map();
    Map(const std::string& mapFile);
    
    bool loadFromFile(const std::string& mapFile);
    bool saveToFile(const std::string& mapFile) const;
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::string getRegionName() const { return regionName; }
    char getTileAt(int x, int y) const;
    
    // Movement and interaction
    bool isValidPosition(int x, int y) const;
    bool canMoveTo(int x, int y) const;
    std::string getTileDescription(char tile) const;
    
    // Display
    void display(int playerX, int playerY) const;
    // Styled display: if useEmoji is true, the map will use emoji/glyphs for tiles.
    void displayStyled(int playerX, int playerY, bool useEmoji = false) const;
    void displayFull() const;
    void displayMinimap(int playerX, int playerY, int viewRange = 5) const;
    
    // Map generation
    void generateDefaultMap(const std::string& region);
};

#endif

