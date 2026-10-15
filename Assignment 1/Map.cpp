#include "Map.h"
#include <iostream>
#include <queue>

using namespace std;

Map::Map(int width, int height)
{
    this->width = width;
    this->height = height;

    entryRow = -1;
    entryColumn = -1;

    exitRow = -1;
    exitColumn = -1;

    cells.resize(height);

    for (int row = 0; row < height; row++)
    {
        cells[row].resize(width);

        for (int column = 0; column < width; column++)
        {
            cells[row][column] = Cell(SCENERY);
        }
    }
}

int Map::getWidth() const
{
    return width;
}

int Map::getHeight() const
{
    return height;
}

bool Map::isInside(int row, int column) const
{
    if (row >= 0 && row < height &&
        column >= 0 && column < width)
    {
        return true;
    }

    return false;
}

void Map::setCell(int row, int column, int type)
{
    if (!isInside(row, column))
    {
        return;
    }

    cells[row][column].setType(type);

    if (type != ENTRY &&
        row == entryRow &&
        column == entryColumn)
    {
        entryRow = -1;
        entryColumn = -1;
    }

    if (type != EXIT &&
        row == exitRow &&
        column == exitColumn)
    {
        exitRow = -1;
        exitColumn = -1;
    }
}

int Map::getCellType(int row, int column) const
{
    if (!isInside(row, column))
    {
        return SCENERY;
    }

    return cells[row][column].getType();
}

bool Map::setEntry(int row, int column)
{
    if (!isInside(row, column))
    {
        return false;
    }

    if (cells[row][column].getType() != PATH &&
        cells[row][column].getType() != ENTRY)
    {
        return false;
    }

    if (entryRow != -1)
    {
        cells[entryRow][entryColumn].setType(PATH);
    }

    entryRow = row;
    entryColumn = column;

    cells[row][column].setType(ENTRY);

    return true;
}

bool Map::setExit(int row, int column)
{
    if (!isInside(row, column))
    {
        return false;
    }

    if (cells[row][column].getType() != PATH &&
        cells[row][column].getType() != EXIT)
    {
        return false;
    }

    if (exitRow != -1)
    {
        cells[exitRow][exitColumn].setType(PATH);
    }

    exitRow = row;
    exitColumn = column;

    cells[row][column].setType(EXIT);

    return true;
}

int Map::getEntryRow() const
{
    return entryRow;
}

int Map::getEntryColumn() const
{
    return entryColumn;
}

int Map::getExitRow() const
{
    return exitRow;
}

int Map::getExitColumn() const
{
    return exitColumn;
}

bool Map::isValid() const
{
    // There must be an entry and an exit.
    if (entryRow == -1 || entryColumn == -1)
    {
        return false;
    }

    if (exitRow == -1 || exitColumn == -1)
    {
        return false;
    }

    int entryCount = 0;
    int exitCount = 0;

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            if (cells[row][column].getType() == ENTRY)
            {
                entryCount++;
            }

            if (cells[row][column].getType() == EXIT)
            {
                exitCount++;
            }
        }
    }

    // There must be exactly one entry and one exit.
    if (entryCount != 1 || exitCount != 1)
    {
        return false;
    }

    // We use Breadth-First Search to check
    // whether the entry is connected to the exit.

    vector<vector<bool> > visited(
        height,
        vector<bool>(width, false)
    );

    queue<pair<int, int> > positions;

    positions.push(
        make_pair(entryRow, entryColumn)
    );

    visited[entryRow][entryColumn] = true;

    while (!positions.empty())
    {
        int currentRow = positions.front().first;
        int currentColumn = positions.front().second;

        positions.pop();

        if (currentRow == exitRow &&
            currentColumn == exitColumn)
        {
            return true;
        }

        // Check above.
        if (isInside(currentRow - 1, currentColumn))
        {
            if (!visited[currentRow - 1][currentColumn] &&
                cells[currentRow - 1][currentColumn].isPath())
            {
                visited[currentRow - 1][currentColumn] = true;

                positions.push(
                    make_pair(
                        currentRow - 1,
                        currentColumn
                    )
                );
            }
        }

        // Check below.
        if (isInside(currentRow + 1, currentColumn))
        {
            if (!visited[currentRow + 1][currentColumn] &&
                cells[currentRow + 1][currentColumn].isPath())
            {
                visited[currentRow + 1][currentColumn] = true;

                positions.push(
                    make_pair(
                        currentRow + 1,
                        currentColumn
                    )
                );
            }
        }

        // Check left.
        if (isInside(currentRow, currentColumn - 1))
        {
            if (!visited[currentRow][currentColumn - 1] &&
                cells[currentRow][currentColumn - 1].isPath())
            {
                visited[currentRow][currentColumn - 1] = true;

                positions.push(
                    make_pair(
                        currentRow,
                        currentColumn - 1
                    )
                );
            }
        }

        // Check right.
        if (isInside(currentRow, currentColumn + 1))
        {
            if (!visited[currentRow][currentColumn + 1] &&
                cells[currentRow][currentColumn + 1].isPath())
            {
                visited[currentRow][currentColumn + 1] = true;

                positions.push(
                    make_pair(
                        currentRow,
                        currentColumn + 1
                    )
                );
            }
        }
    }

    return false;
}

void Map::display() const
{
    cout << "\nMap:\n";

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            int type = cells[row][column].getType();

            if (type == SCENERY)
            {
                cout << ". ";
            }
            else if (type == PATH)
            {
                cout << "# ";
            }
            else if (type == ENTRY)
            {
                cout << "E ";
            }
            else if (type == EXIT)
            {
                cout << "X ";
            }
        }

        cout << endl;
    }
}