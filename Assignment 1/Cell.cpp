#include "Cell.h"

Cell::Cell(int type)
{
    this->type = type;
}

int Cell::getType() const
{
    return type;
}

void Cell::setType(int newType)
{
    type = newType;
}

bool Cell::isPath() const
{
    return type == PATH ||
           type == ENTRY ||
           type == EXIT;
}

bool Cell::canPlaceTower() const
{
    return type == SCENERY;
}