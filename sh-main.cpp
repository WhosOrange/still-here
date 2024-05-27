/** [Still Here]    ***
***   Main File     ***
***                 ***/
/* Ambitions:
    = create a map loader:
    = create an entity loader: using a numbered array in the executable where input text gets included as 
    = make an executable that can be run on the Windows XP but doesn't have to be recompiled to add stuff
    = a new game can be create for every vinyl weekend with new maps, enemies and features
    
    TODO for 5/15:
    = create a window
    = create graphics for enemies
    = create graphics for defenses
    = create graphics for geos
    = add player to map
    = make enemies move
    = make defenses rotate

*/

/* Libraries **/
#include <iostream>     // console input & output
#include <fstream>      // reading and saving map data
#include <cstdlib>      // for C Standard Libary functions like rand (RNG), abs (absolute value), getenv (system vars and path getting)
#include <string>       // for to_string (requires c++11)
//#include "raylib.h"     // reading images, displaying images, OpenGL, sounds

/* Global Variables **/

/* Game Classes */
struct Player
{
    int posX, posY;
    int droneLevel;
};

// Enemy class
struct Enemy
{
    std::string name; //allow virutal maybe
    std::string graphicsFile;
    int posX, posY;
    
    Enemy();
    Enemy(std::string inName, std::string inFile, int inPosX, int inPosY);
    void analyze();
};

Enemy::Enemy()
{
    name = "BLANKENEMY";
    graphicsFile = "gfx/blank.gfx";
    posX = 0;
    posY = 0;
}

Enemy::Enemy(std::string inName, std::string inFile, int inPosX, int inPosY)
{
    name = inName;
    graphicsFile = inFile;
    posX = inPosX;
    posY = inPosY;
}

void Enemy::analyze()
{
     std::cout << name << " is initialized at " << posX << ", " << posY << ". \n\tGRAPHICS FILE: " << graphicsFile << "\n";
}

// Defense Class
struct Defense
{
    int posX, posY;
    std::string name;
    std::string graphicsFile;
    int level;
    int attack;
    
    Defense();
    Defense(std::string inName, std::string inFile, int inPosX, int inPosY, int inLevel, int inAttack);
    void analyze();
};

Defense::Defense()
{
    name = "BLANKDEFENSE";
    graphicsFile = "gfx/blank.gfx";
    posX = 0;
    posY = 0;
    level = 1;
    attack = 1;
}

Defense::Defense(std::string inName, std::string inFile, int inPosX, int inPosY, int inLevel, int inAttack)
{
    name = inName;
    graphicsFile = inFile;
    posX = inPosX;
    posY = inPosY;
    level = inLevel;
    attack = inAttack;
}

void Defense::analyze()
{
     std::cout << name << " is initialized at " << posX << ", " << posY << ".\n";
     std::cout << "\tLEVEL: " << level << "\n";
     std::cout << "\tATTACK STRENGTH: " << attack << "\n";
     std::cout << "\tGRAPHICS FILE: " << graphicsFile << "\n\n";
}

// Map Geometery, with collisions
struct MapGeo  
{
    int x, y, w, h;
    char type;
    //virtual void drawOnMap(int inSizeX, int inSizeY);
    MapGeo();
    MapGeo(int inX, int inY);
    
};

MapGeo::MapGeo()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
    type = 'v';
}

MapGeo::MapGeo(int inX, int inY)
{
    x = inX;
    y = inY;
    w = 0;
    h = 0;
    type = 'v';
}

struct GeoRect : public MapGeo
{
    GeoRect();
    GeoRect(int inX, int inY, int inW, int inH);
};

GeoRect::GeoRect()
{
    x = 0;
    y = 0;
    w = 4;
    h = 4;
    type = 'r';
}

GeoRect::GeoRect(int inX, int inY, int inW, int inH)
{
    x = inX;
    y = inY;
    w = inW;
    h = inH;
    type = 'r';
}

// Draw Geometery class
void drawGeo(GeoRect inRect, int inSizeX, int inSizeY, char drawChar)
{
    for (int i = inSizeX; i > 0; i--)
    {
        for (int j = inSizeY; j > 0; j--)
        {
            if (i >= inRect.x && i < inRect.x + inRect.w && j >= inRect.y && j < inRect.y + inRect.h)
            {
                std::cout << "X";
                //drawMap[i][j] = drawChar;
                //isMap[i][j] = true;
            }
            else
            {
                std::cout << " ";
            }
        }
    }
};

void drawPoint(MapGeo inGeo, int inSizeX, int inSizeY, char drawChar)
{
    //char drawMap[inSizeX][inSizeY];
    //bool visMap[inSizeX][inSizeY];
    for (int i = inSizeX; i > 0; i--)
    {
        for (int j = inSizeY; j > 0; j--)
        {
            if (i == inGeo.x && j == inGeo.y)
            {
                std::cout << "X";
                //drawMap[i][j] = drawChar;
                //visMap[i][j] = true;
            }
            else
            {
                std::cout << " ";
            }
        }
    }
    
};

// Game Map class
struct GameMap
{
    int sizeX, sizeY;
    std::string name;
    Enemy arrEnemies[256];
    Defense arrDefenses[128];
    MapGeo arrGeos[256];
    //Player *zaPlayer;
    
    GameMap();
    GameMap(int inSizeX, int inSizeY, std::string inName);
};

GameMap::GameMap()
{
    sizeX = 8;
    sizeY = 8;
    name = "BLANK";
}

GameMap::GameMap(int inSizeX, int inSizeY, std::string inName)
{
    sizeX = inSizeX;
    sizeY = inSizeY;
    name = inName;
}

// TODO: 
GameMap LoadMap(std::string filePath)
{
    // Initialize Variables and Read File
    std::string debugStr;
    std::string inputStr;
    int countEnemies = 0;
    //int countDefenses;
    GameMap outMap(16, 16, "LOADED");
    std::ifstream zaFile;
    
    // Parse loop
    zaFile.open(filePath.c_str());
    while (!zaFile.eof())
    {
        
        if (zaFile.eof())
        {
            break;
        }
        zaFile >> inputStr;
        std::cout << inputStr << "\n";
        
        // If Space
        if (inputStr == "")
        {
            zaFile >> inputStr;
        }
        
        // CREATE New Map
        else if (inputStr == "MAP")
        {
            zaFile >> inputStr;
            
            if (inputStr == "NAME")
            {
                getline(zaFile, outMap.name);
                std::cout << "Map name set as " << outMap.name << "\n"; //DEBUG
            }
            else if (inputStr == "SIZE")
            {
                zaFile >> outMap.sizeX >> outMap.sizeY;
                std::cout << "Size set at " << outMap.sizeX << " by " << outMap.sizeY << "\n"; //DEBUG
            }
            else
            {
                std::cerr << "No map loaded?\n"; //DEBUG
            }
        }
        // CREATE New Enemy
        else if (inputStr == "ENEMY")
        {
            countEnemies++; 
            std::cout << countEnemies;
            getline(zaFile, outMap.arrEnemies[countEnemies].name);
            getline(zaFile, outMap.arrEnemies[countEnemies].graphicsFile);
            zaFile >> outMap.arrEnemies[countEnemies].posX >> outMap.arrEnemies[countEnemies].posY;
            
            std::cout << outMap.arrEnemies[countEnemies].name << " has been created!\n"; //DEBUG
            std::cout << "\tGRAPHICS FILE: " << outMap.arrEnemies[countEnemies].graphicsFile << "\n"; //DEBUG
            std::cout << "\tLOCATION: " << outMap.arrEnemies[countEnemies].posX << ", " << outMap.arrEnemies[countEnemies].posY << "\n"; //DEBUG
        }
        // ADJUST an enemy
        else if (inputStr == "ADJUSTENEMY")
        {
            int inInt;
            zaFile >> inInt;
            zaFile >> outMap.arrEnemies[inInt].posX >> outMap.arrEnemies[inInt].posY;
            std::cout << outMap.arrEnemies[inInt].name << " has been adjusted to be at " << outMap.arrEnemies[inInt].posX << " by " << outMap.arrEnemies[inInt].posX << "\n"; //DEBUG
        }
        
        getline(std::cin, debugStr);
    
    }
    
    zaFile.close();
    return outMap;
    
};

// TODO: GAME SAVING
void SaveGame(int slot)
{
    // TODO: save the game's data to a file
};

// TODO: 
void driver()
{
    std::string playerInput;
    // PART 1: Enter Level Name
    std::string cinLevelName;

    std::cout << "\n\nEnter a Level Name:\n";
    std::getline(std::cin, cinLevelName);
    
    // PART 2: Create Level object
    GameMap level(64, 32, cinLevelName);
    // DEBUG: testing GameMap variables
    std::cout << "\nLevel " << level.name << " has been created!\nIt has a width of " << level.sizeX << " and a length of " << level.sizeY << ".\n";
    
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // PART 3: Create Monsters
    Enemy moe("Moe", "gfx/moehoward.gfx", 14, 20);
    Enemy larry("Larry", "gfx/larryfine.gfx", 2, 5);
    Enemy curly("Curly", "gfx/jeromehoward.gfx", 7, 15);
    // DEBUG: testing Enemy variables
    std::cout << "\n\nList of create enemy types:\n";
    std::cout << "\t" << moe.name << ": is initialized at " << moe.posX << ", " << moe.posY << ". GRAPHICS FILE: " << moe.graphicsFile << ".\n";
    std::cout << "\t" << larry.name << ": is initialized at " << larry.posX << ", " << larry.posY << ". GRAPHICS FILE: " << larry.graphicsFile << ".\n";
    std::cout << "\t" << curly.name << ": is initialized at " << curly.posX << ", " << curly.posY << ". GRAPHICS FILE: " << curly.graphicsFile << ".\n";
    
    std::cout << "\n\nTYPE and ENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // PART 4: Add Monsters To Level
    // DEBUG: Announce Adding monsters;
    int testArraySizeEnemies = 10;
    std::cout << "\n//PREPARING TO ADD " << testArraySizeEnemies << " MONSTERS TO " << level.name << "'s ENEMY ARRAY.\n";
    
    for (int i = testArraySizeEnemies; i > 0; i--)
    {
        Enemy current = moe;    // default to moe
        //if (i % 3 == 0) {current = moe;}
        if (i % 3 == 1) {current = larry;}
        else if (i % 3 == 2) {current = curly;}
        
        // TEST: put 4th enemy in a different spot
        if (i == 7) {current.posX = 17; current.posY = 17;}
        
        // TEST: make last enemy Shemp
        if (i == 1) 
        {
            current.name = "Shemp";
            current.graphicsFile = "gfx/shemphoward.gfx";
            current.posX = 14;
            current.posY = 10;
        } 
        
        level.arrEnemies[i] = current;
        
        std::cout << "ENEMY No." << i << ":";
        level.arrEnemies[i].analyze();   // DEBUG: analyze the arrEnemies 
    }
    
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // PART 5: Create Defeneses
    //Defense(std::string inName, std::string inFile, int inPosX, int inPosY, int inLevel, int inAttack)
    Defense mista("Guido Mista", "gfx/mista_gw.gfx", 4, 4, 4, 4);
    Defense inuyasha("Inuyasha", "gfx/inuyasha.gfx", 0, 9, 3, 20);
    Defense doomguy("DOOM SLAYER", "gfx/doomguy.gfx", 64, 6, 32, 100);
    Defense steve("Steve from Blue's Clues", "gfx/steve_bc.gfx", 1, 7, 99, 1);
    
    // DEBUG: testing Enemy variables
    std::cout << "\n\nList of created defensive characters:\n";
    inuyasha.analyze();
    doomguy.analyze();
    steve.analyze();
    mista.analyze();
    
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // PART 6: Add Defense Characters to Level
    // DEBUG: Announcement to add defenses to array
    int testArraySizeDefenses = 16;
    std::cout << "\n//PREPARING TO ADD " << testArraySizeDefenses << " DEFENSE CHARACTER TO " << level.name << "'s DEFENSES ARRAY.\n";
    
    for (int i = testArraySizeDefenses; i > 0; i--)
    {
        Defense current = inuyasha;    // default to inuyasha, current can be reused due to it's SCOPE ONLY INSIDE the BRACKETS of this for loop
        if (i % 3 == 1) {current = steve;}
        else if (i % 3 == 2) {current = doomguy;}
        if (i % 4 == 0) {current = mista;}
        
        // TEST: put 9th defense in a different spot
        if (i == 9) {current.posX = 27; current.posY = 36;}
        
        // TEST: make last defense special
        if (i == 1) 
        {
            current.name = "Dante from Devil May Cry";
            current.graphicsFile = "gfx/dante_dmc.gfx";
            current.posX = 63;
            current.posY = 30;
        } 
        
        level.arrDefenses[i] = current;
        
        std::cout << "DEFENSE No." << i << ":";
        level.arrDefenses[i].analyze();   // DEBUG: analyze the arrEnemies 
    }
    
    // STOP GAP: ask to continue
    std::cout << "\n\nDo You Wish To Continue?: ";
    std::getline(std::cin, playerInput);
    
    
    // PART 7: Add Geometery To The Level
    int testArraySizeGeo = 5;
    {
        GeoRect current(0, 0, 16, 16);
        level.arrGeos[5] = current;
        level.arrGeos[1] = (GeoRect){20, 20, 6, 6};
        level.arrGeos[2] = (GeoRect){14, 14, 3, 2};
        level.arrGeos[3] = (GeoRect){40, 2, 22, 7};
        level.arrGeos[4] = (MapGeo){62, 31};         
    }
    
    
    // PART 8: Build and Draw the Map with Geometery represented by ASCII
    char asciiGeo[] = {'w', 'v', 'm', 'n', '^', '_'};    // characters to draw for each plane of geometery
    char drawnMap[level.sizeX][level.sizeY] = {(char)0x20};               // initialize map to draw
    bool colMap[level.sizeX][level.sizeY] = {0};                    // initialize map for collisions
    
    // PART 8.1: Draw each individual shape and set up the drawn and collision arrays
    for (int k = testArraySizeGeo; k > 0; k--)
    {
        
        //DEBUG: Announce current geometery
        std::cout << "/////////////////////////////\n" << level.name << ", GEO No." << k << " at " << level.arrGeos[k].x << ", " << level.arrGeos[k].y << "\n";
        
        switch (level.arrGeos[k].type)
        {
        case 'r':
            for (int i = level.sizeX; i > 0; i--)
            {
                std::cout << "\n";
                for (int j = level.sizeY; j > 0; j--)
                {
                    if (i >= level.arrGeos[k].x && i < level.arrGeos[k].x + level.arrGeos[k].w && j >= level.arrGeos[k].y && j < level.arrGeos[k].y + level.arrGeos[k].h)
                    {
                        std::cout << "X";
                        drawnMap[i][j] = asciiGeo[k];
                        colMap[i][j] = true;
                    }
                    else
                    {
                        std::cout << " ";
                    }
                }
            }
            break;
        default:    //default to a single x,y point
            for (int i = level.sizeX; i > 0; i--)
            {
                std::cout << "\n";
                for (int j = level.sizeY; j > 0; j--)
                {
                    if (i == level.arrGeos[k].x && j == level.arrGeos[k].y)
                    {
                        std::cout << "X";
                        drawnMap[i][j] = asciiGeo[k];
                        colMap[i][j] = true;
                    }
                    else
                    {
                        std::cout << " ";
                    }
                }
            }
            break;
        }
        std::cout << "\n\n////////////////////////////\n";
        std::cout << "ENTER NEXT\n";
        std::getline(std::cin, playerInput);
    }
    
    // PART 8.2: Drawing the Collisions
    std::cout << "////DRAWING " << level.name << "'s COLISION MAP:\n";
    
    for (int i = level.sizeX; i > 0; i--)
    {
        std::cout << "\n";
        for (int j = level.sizeY; j > 0; j--)
        {
            if (colMap[i][j] == true) {std::cout << "X";}
            else {std::cout << " ";}
        }
        
    }
    
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // PART 8.3: Drawing the ASCII Map
    std::cout << "////DRAWING " << level.name << "'s VISIBLE MAP:\n";
    
    for (int i = level.sizeX; i > 0; i--)
    {
        std::cout << "\n";
        for (int j = level.sizeY; j > 0; j--)
        {
            std::cout << drawnMap[i][j];
        }
        
    }
    
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // PART 8.4: Config the ASCII Map with Enemies and Defenses
    
    for (int k = testArraySizeEnemies; k > 0; k--)
    {
        drawnMap[level.arrEnemies[k].posX][level.arrEnemies[k].posY] = 'E';
    }
    
    for (int k = testArraySizeDefenses; k > 0; k--)
    {
        drawnMap[level.arrDefenses[k].posX][level.arrDefenses[k].posY] = 'O';
    }
    
    // PART 8.5 Drawing The ASCII Map with Enemies and Defenses
    std::cout << "////DRAWING " << level.name << "'s VISIBLE MAP (WITH OBJECTS):\n";
    
    for (int i = level.sizeX; i > 0; i--)
    {
        for (int j = level.sizeY; j > 0; j--)
        {
            std::cout << drawnMap[i][j];
        }
        std::cout << "\n";
    }
    
    /*
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // PART 9: Random Map
    // DEBUG Testing LoadMap function
    std::cout << "//DEBUG for funsies: creating a new random map\n\n";
    GameMap level2 = LoadMap("loaded.map");
    std::cout << "LOADED LEVEL TEST: " << level2.name << "\n. \tSIZE: " << level2.sizeX << " x " << level2.sizeY << "\n\n";
    
    // Change Map Variables: testing rand() and array.size()
    level2.sizeX = (((rand() % 4)) + 1) * 16;
    level2.sizeY = (((rand() % 3)) + 1) * 16;
    char drawnMap2[level2.sizeX * level2.sizeY] = {(char)0x20}; // 1D drawn map for funsies
    
    // DEBUG: 1D Array Test
    std::cout << "\n//DEBUG: creating a random drawn map\n\n";
    int arrCount2[level2.sizeX * level2.sizeY];
    
    for (int i = level2.sizeX * level2.sizeY; i > 0; i--)
    {
        arrCount2[i] = rand() % 5;
        drawnMap2[i] = asciiGeo[arrCount2[i]];
    }
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // Create random enemies and put them random places
    std::cout << "\n//DEBUG: creating new enemies\n\n";
    int level2EnemyCount = (rand() % 12) + 1;
    for (int i = level2EnemyCount; i > 0; i--)
    {
        Enemy current("Rando", "gfx/random.gfx", rand() % level2.sizeX, rand() & level2.sizeY); 
        level2.arrEnemies[i] = current;
        level2.arrEnemies[i].analyze();
        arrCount2[(level2.arrEnemies[i].posX * level2.sizeY) + level2.arrEnemies[i].posY] += 9; // Density of the position
        drawnMap2[(level2.arrEnemies[i].posX * level2.sizeY) + level2.arrEnemies[i].posY] = 'E';  // 1D posistion
    }
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // Create random defenses
    std::cout << "\n//DEBUG: creating a new defenses\n\n";
    int level2DefenseCount = (rand() % 8) + 1;
    for (int i = level2DefenseCount; i > 0; i--)
    {
        Defense current("Anonymous Defender", "gfx/random.gfx", rand() % level2.sizeX, rand() & level2.sizeY, (rand() % 50) + 1, (rand() % 100) + 1); 
        level2.arrDefenses[i] = current;
        level2.arrDefenses[i].analyze();
        arrCount2[(level2.arrDefenses[i].posX * level2.sizeY) + level2.arrDefenses[i].posY] += 5;   // Denisity of the Position
        drawnMap2[(level2.arrDefenses[i].posX * level2.sizeY) + level2.arrDefenses[i].posY] = 'O';  // 1D posistion
    }
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    // Draw the Map
    std::cout << "\n//DEBUG: Drawing the Random Map!\n\n";
    for (int i = level2.sizeX * level2.sizeY; i > 0; i--)
    {
        if (i % level2.sizeY == 0)
        {
            std::cout << "\n";
        }
        std::cout << drawnMap2[i];
    }
    
    // DEBUG: get user input to continue
    std::cout << "\n\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
    std::cout << "\n//DEBUG: Determining the density of the Map!\n\n//Most dense points:\n";
    for (int i = level2.sizeX * level2.sizeY; i > 0; i--)
    {
        if (i > 5)
        {
            std::cout << "\nPoint: " << (i - (i % level2.sizeY))/level2.sizeY << ", " << i % level2.sizeY << ":\n";
            std::cout << "\tEnemy Density: " << (arrCount2[i] - (arrCount2[i] % 9))/9 << "\tDefensive Density: " << (arrCount2[i] - (arrCount2[i] % 5))/5;
        }
    }
    
    */
    // FINAL DEBUG: get user input to continue
    std::cout << "\n\nDriver Is Finshed\nENTER to Contiune.";
    std::getline(std::cin, playerInput);
    
};

/* Main Loop **/
int main()
{
    std::cout << "It's time to test opening a map.\n\n";
    GameMap test = LoadMap("john's fun dungeon.map");
    //driver();
    return 0;
}

/*
g++ "H:\Musical Workshop and Self Reflection\Project 2023\May RR\Still Here\sh-main.cpp" -o "H:\Musical Workshop and Self Reflection\Project 2023\May RR\Still Here\TEST-OpenMAP.exe" -s -static -Os -Wall 
*/