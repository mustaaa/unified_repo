#pragma once
#include <stddef.h>
#include <array>
#include <vector>

template <typename T, std::size_t rows, std::size_t cols>
using CGrid = std::array<std::array<T, rows>, cols>;

template <typename TGrid>
std::vector<typename TGrid::value_type::value_type> getNeighbors(const TGrid &grid, int x, int y)
{
    std::vector<typename TGrid::value_type::value_type> neighbors;
    std::size_t width = 0;
    std::size_t height = 0;
    width = grid.size();
    if (width)
    {
        height = grid[0].size();
    }
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (!(i == 0 && j == 0))
            {
                int xIndex = x + i;
                int yIndex = y + j;
                if (xIndex >= 0) 
                {
                    if (xIndex < grid.size())
                    {
                        if (yIndex >= 0)
                        {
                            if (yIndex < grid[xIndex].size())
                            {
                                neighbors.push_back(grid[xIndex][yIndex]); 
                            }
                        }
                    }
                }
            }
        }
    }
    return neighbors;
}
