#ifndef CELL_H
#define CELL_H

const int SCENERY = 0;
const int PATH = 1;
const int ENTRY = 2;
const int EXIT = 3;

class Cell
{
private:
    int type;

public:
    Cell(int type = SCENERY);

    int getType() const;
    void setType(int newType);

    bool isPath() const;
    bool canPlaceTower() const;
};

#endif