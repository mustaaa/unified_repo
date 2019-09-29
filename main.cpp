#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <utility>
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

std::pair<int, int> getGridPos(const sf::Vector2i &pos)
{
    return std::make_pair(pos.x / 10, pos.y / 10);
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Clock clock; // starts the clock
    auto sizeInPixel = window.getSize();

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < gGrid.size(); i++)
    {
        for (int j = 0; j < gGrid[i].size(); j++)
        {
            gGrid[i][j].mShape.setSize({10, 10});
            gGrid[i][j].mShape.setPosition({i * 10, j * 10});
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

    bool leftMouseIsPressed = false;
    bool rightMouseIsPressed = false;
    
    std::cout << "Init complete\n";
    // Start the game loop
    while (window.isOpen())
    {

        auto mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        auto gridPos = getGridPos(localPosition);

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (gridPos.first < gGridWidth && gridPos.second < gGridHeight && gridPos.first >= 0 && gridPos.second >= 0)
            {

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        leftMouseIsPressed = true;
                    }
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        rightMouseIsPressed = true;
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        leftMouseIsPressed = false;
                    }
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        rightMouseIsPressed = false;
                    }
                }


            
            }
        }

        if (leftMouseIsPressed)
        {
            gGrid[gridPos.first][gridPos.second].mState = 1;
            gGrid[gridPos.first][gridPos.second].mShape.setFillColor(sf::Color::Black);
        }
        if (rightMouseIsPressed)
        {
            gGrid[gridPos.first][gridPos.second].mState = 0;
            gGrid[gridPos.first][gridPos.second].mShape.setFillColor(sf::Color::White);
        }

        // Clear screen
        window.clear();
        // Draw the rects
        for (int i = 0; i < gGrid.size(); i++)
        {
            for (int j = 0; j < gGrid[i].size(); j++)
            {
                window.draw(gGrid[i][j].mShape);
            }
        }

        sf::Time elapsed1 = clock.getElapsedTime();
        if (elapsed1.asMilliseconds() > 120 && !leftMouseIsPressed && !rightMouseIsPressed)
        {
            clock.restart();

            for (int i = 0; i < gGrid.size(); i++)
            {
                for (int j = 0; j < gGrid[i].size(); j++)
                {
                    auto neighbors = getNeighbors(gGrid, i, j);
                    //std::cout << "getNeighbors returned for " << i << "," << j << " is:" << neighbors.size() << " neighbors\n";
                    int aliveNeighborCount = 0;
                    for (const auto &e : neighbors)
                    {
                        if (e.mState == 1)
                            aliveNeighborCount++;
                    }
                    if (gGrid[i][j].mState && (aliveNeighborCount < 2 || aliveNeighborCount > 3))
                    {
                        gNextGrid[i][j].mState = 0;
                        gNextGrid[i][j].mShape.setFillColor(sf::Color::White);
                    }
                    else if ((gGrid[i][j].mState == 0) && aliveNeighborCount == 3)
                    {
                        gNextGrid[i][j].mState = 1;
                        gNextGrid[i][j].mShape.setFillColor(sf::Color::Black);
                    }
                    else
                    {
                        gNextGrid[i][j].mState = gGrid[i][j].mState;
                        gNextGrid[i][j].mShape.setFillColor(gGrid[i][j].mState ? sf::Color::Black : sf::Color::White);
                    }
                }
            }
            gGrid = gNextGrid;
        }

        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
