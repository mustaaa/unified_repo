#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "CellularAutomata.h"

const std::size_t gGridWidth = 800 / 10;
const std::size_t gGridHeight = 600 / 10;

struct CCell
{
    sf::RectangleShape mShape;
    int mState = 0;
};

CGrid<CCell, gGridHeight, gGridWidth> gGrid;
CGrid<CCell, gGridHeight, gGridWidth> gNextGrid;


int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    auto sizeInPixel = window.getSize();
    
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0 ; i < gGrid.size(); i++)
    {
        for (int j = 0; j < gGrid[i].size(); j++)
        {
            gGrid[i][j].mShape.setSize({10,10});
            gGrid[i][j].mShape.setPosition({i*10, j*10});
            gGrid[i][j].mShape.setOutlineColor(sf::Color::Green);
            gGrid[i][j].mShape.setOutlineThickness(1.0);
            gGrid[i][j].mShape.setFillColor(sf::Color::White);
            if (dis(gen))
            {
                gGrid[i][j].mShape.setFillColor(sf::Color::Black);
                gGrid[i][j].mState = 1;
            }        
        }
    }
    gNextGrid = gGrid;
     
    std::cout << "Init complete\n";
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            
        }
        auto mousePos = sf::Mouse::getPosition(window);
       
        for (int i = 0; i < gGrid.size(); i++) 
        {
            for (int j = 0; j < gGrid[i].size(); j++) 
            {
                auto neighbors = getNeighbors(gGrid, i, j);
                //std::cout << "getNeighbors returned " << neighbors.size() << " neighbors\n";
                int aliveNeighborCount = 0;
                for (const auto& e : neighbors) {
                    if (e.mState == 1) 
                        aliveNeighborCount++;
                }
                if (gGrid[i][j].mState && (aliveNeighborCount < 2 || aliveNeighborCount > 3) ) 
                {
                    gNextGrid[i][j].mState = 0;
                    gNextGrid[i][j].mShape.setFillColor(sf::Color::White);
                }
                else if ((gGrid[i][j].mState == 0) && (aliveNeighborCount == 2 || aliveNeighborCount == 3) )
                {
                    gNextGrid[i][j].mState = 1;
                    gNextGrid[i][j].mShape.setFillColor(sf::Color::Black);
                }
                else {
                    gNextGrid[i][j].mState = gGrid[i][j].mState;
                    gNextGrid[i][j].mShape.setFillColor(gGrid[i][j].mState ? sf::Color::Black : sf::Color::White);
                }
            }
        }
        


        // Clear screen
        window.clear();
        // Draw the sprite
        for (int i = 0; i < gGrid.size(); i++) 
        {
            for (int j = 0; j < gGrid[i].size(); j++) 
            {
                window.draw(gGrid[i][j].mShape);
            }
        }

        gGrid = gNextGrid;

        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}