#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Cell.h"
#include "Observable.h"

class Map : public Observable
{
private:
    int width;
    int height;

    std::vector<std::vector<Cell> > cells;

    int entryRow;
    int entryColumn;

    int exitRow;
    int exitColumn;

public:
    Map(int width, int height);

    int getWidth() const;
    int getHeight() const;

    void setCell(int row, int column, int type);
    int getCellType(int row, int column) const;

    bool setEntry(int row, int column);
    bool setExit(int row, int column);

    int getEntryRow() const;
    int getEntryColumn() const;

    int getExitRow() const;
    int getExitColumn() const;

    bool isInside(int row, int column) const;

    bool isValid() const;

    void display() const;
};

#endif