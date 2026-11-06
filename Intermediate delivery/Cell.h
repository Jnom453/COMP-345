#ifndef CELL_H
#define CELL_H

/**
 * @file Cell.h
 * @brief One square of the game map and its possible types.
 */

/** @brief Scenery cell: towers may be placed here, critters may not walk here. */
const int SCENERY = 0;

/** @brief Path cell: critters walk here, towers may not be placed here. */
const int PATH = 1;

/** @brief The single entry point of the map, where critters appear. It is part of the path. */
const int ENTRY = 2;

/** @brief The single exit point of the map, where critters escape. It is part of the path. */
const int EXIT = 3;

/**
 * @brief One cell of the map grid.
 *
 * Game rule: a cell is either scenery or part of the path. Critters may only
 * move on path cells (including the entry and the exit) and towers may only
 * be placed on scenery cells.
 */
class Cell
{
private:
    /** @brief Cell type: SCENERY, PATH, ENTRY or EXIT. */
    int type;

public:
    /**
     * @brief Creates a cell.
     * @param type Initial type, SCENERY by default.
     */
    Cell(int type = SCENERY);

    /** @brief Returns the cell type (SCENERY, PATH, ENTRY or EXIT). */
    int getType() const;

    /**
     * @brief Changes the cell type.
     * @param newType SCENERY, PATH, ENTRY or EXIT.
     */
    void setType(int newType);

    /**
     * @brief Tells whether critters can walk on this cell.
     * @return true for PATH, ENTRY and EXIT cells.
     */
    bool isPath() const;

    /**
     * @brief Tells whether a tower may be placed on this cell.
     * @return true only for SCENERY cells.
     */
    bool canPlaceTower() const;
};

#endif
