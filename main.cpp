#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <utility>
#include "CellularAutomata.h"

const std::size_t screenWidthInPx = 400;
const std::size_t screenHeigthInPx = 400;
const std::size_t cellSizeInPx = 10;
const std::size_t gGridWidth = screenWidthInPx / cellSizeInPx;
const std::size_t gGridHeight = screenHeigthInPx / cellSizeInPx;

struct CCell
{
    sf::RectangleShape mShape;
    int mState = 0;
};

CGrid<CCell, gGridHeight, gGridWidth> gGrid;
CGrid<CCell, gGridHeight, gGridWidth> gNextGrid;

std::pair<int, int> getGridPos(const sf::Vector2i &pos)
{
    return std::make_pair(pos.x / cellSizeInPx, pos.y / cellSizeInPx);
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(screenWidthInPx, screenHeigthInPx), "SFML window");

    sf::Clock clock; // starts the clock
    auto sizeInPixel = window.getSize();

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < gGrid.size(); i++)
    {
        for (int j = 0; j < gGrid[i].size(); j++)
        {
            gGrid[i][j].mShape.setSize({ static_cast<float>(cellSizeInPx),static_cast<float>(cellSizeInPx)});
            gGrid[i][j].mShape.setPosition({static_cast<float>(i)*10, static_cast<float>(j)*10});
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

	auto curGrid = &gGrid;
	auto nextGrid = &gNextGrid;

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

		if (gridPos.first < gGridWidth && gridPos.second < gGridHeight && gridPos.first >= 0 && gridPos.second >= 0)
		{
			if (leftMouseIsPressed)
			{
				(*curGrid)[gridPos.first][gridPos.second].mState = 1;
				(*curGrid)[gridPos.first][gridPos.second].mShape.setFillColor(sf::Color::Black);
			}
			if (rightMouseIsPressed)
			{
				(*curGrid)[gridPos.first][gridPos.second].mState = 0;
				(*curGrid)[gridPos.first][gridPos.second].mShape.setFillColor(sf::Color::White);
			}
		}
        // Clear screen
        window.clear();
        // Draw the rects
        for (int i = 0; i < (*curGrid).size(); i++)
        {
            for (int j = 0; j < (*curGrid)[i].size(); j++)
            {
                window.draw((*curGrid)[i][j].mShape);
            }
        }

        sf::Time elapsed1 = clock.getElapsedTime();
        if (elapsed1.asMilliseconds() > 60 && !leftMouseIsPressed && !rightMouseIsPressed)
        {
            clock.restart();

            for (int i = 0; i < (*curGrid).size(); i++)
            {
                for (int j = 0; j < (*curGrid)[i].size(); j++)
                {
                    auto neighbors = getNeighbors((*curGrid), i, j);
                    //std::cout << "getNeighbors returned for " << i << "," << j << " is:" << neighbors.size() << " neighbors\n";
                    int aliveNeighborCount = 0;
                    for (const auto &e : neighbors)
                    {
                        if (e.mState == 1)
                            aliveNeighborCount++;
                    }
                    if ((*curGrid)[i][j].mState && (aliveNeighborCount < 2 || aliveNeighborCount > 3))
                    {
                        (*nextGrid)[i][j].mState = 0;
                        (*nextGrid)[i][j].mShape.setFillColor(sf::Color::White);
                    }
                    else if (((*curGrid)[i][j].mState == 0) && aliveNeighborCount == 3)
                    {
                        (*nextGrid)[i][j].mState = 1;
                        (*nextGrid)[i][j].mShape.setFillColor(sf::Color::Black);
                    }
                    else
                    {
                        (*nextGrid)[i][j].mState = (*curGrid)[i][j].mState;
                        (*nextGrid)[i][j].mShape.setFillColor((*curGrid)[i][j].mState ? sf::Color::Black : sf::Color::White);
                    }
                }
            }
			std::swap(curGrid, nextGrid);
        }

        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
