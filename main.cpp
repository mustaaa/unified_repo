#include <SFML/Graphics.hpp>
#include <iostream>

const int maxIterations = 500;
const float convergenceLimit = 160.0f;

float mapTo(int inBegin, int inEnd, int x, float outBegin, float outEnd)
{
    float out; 
    out = outBegin + (outEnd - outBegin) * (static_cast<float>(x) / static_cast<float>(inEnd - inBegin)); 
	return out;
}

int checkConvergence(const sf::Vector2f& point)
{
    int iters = 0;
    sf::Vector2f lastPoint = point;
    for (; iters < maxIterations; iters++)
    {
        auto save = lastPoint.x;
        lastPoint.x = (lastPoint.x * lastPoint.x) - (lastPoint.y * lastPoint.y) + point.x;
        lastPoint.y = 2 * (save * lastPoint.y) + point.y;
        if (lastPoint.x + lastPoint.y > convergenceLimit)
        {
            break;
        }
    }
    return iters;
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Vector2f rangeBegin(-2.0, -2.0);
    sf::Vector2f rangeEnd(2.0, 2.0);

    auto sizeInPixel = window.getSize();
    
    sf::Image img;
    img.create(sizeInPixel.x, sizeInPixel.y, sf::Color::Black);

    sf::Texture texture;
    texture.loadFromImage(img);

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
            
            if (event.type == sf::Event::Resized)
            {
                sizeInPixel = window.getSize();
                sf::Image newImg;
                std::cout << "newsize is " << sizeInPixel.x << "," << sizeInPixel.y << "\n";
                newImg.create(sizeInPixel.x, sizeInPixel.y, sf::Color::Black);
                img = newImg;
            }
        }
        auto mousePos = sf::Mouse::getPosition(window);
        auto mouseX = mapTo(0, sizeInPixel.x, mousePos.x, rangeBegin.x, rangeEnd.x);
        auto mouseY = mapTo(0, sizeInPixel.y, mousePos.y, rangeBegin.y, rangeEnd.y);
        
        std::cout << "mouse x,y:" << mouseX << "," << mouseY << "\n";
        std::cout << "img size is  x,y:" << img.getSize().x << "," << img.getSize().y << "\n";
        

        for (int i = 0; i < sizeInPixel.x; i++)
        {
            for (int j = 0; j < sizeInPixel.y; j++)
            {
                float x = mapTo(0, sizeInPixel.x, i, rangeBegin.x, rangeEnd.x);
                float y = mapTo(0, sizeInPixel.y, j, rangeBegin.y, rangeEnd.y);
                auto iters = checkConvergence({x,y});
                auto ratio = (iters*255)/maxIterations;
                // if (iters == maxIterations)
                // {
                //     ratio = 0; 
                // }
                //std::cout << x << ","<< y<< "\n";
                img.setPixel(i, j, sf::Color(0,0,ratio));
                                
            }
        }
        texture.loadFromImage(img);
        sf::Sprite sprite;
        sprite.setTexture(texture, true);
        sprite.setPosition({0.0, 0.0});

        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(sprite);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}