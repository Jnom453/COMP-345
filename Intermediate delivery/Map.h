#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Cell.h"
#include "Observable.h"

/**
 * @file Map.h
 * @brief The game map: a grid of cells with one entry and one exit.
 */

/**
 * @brief A rectangular grid of cells with an entry point and an exit point.
 *
 * Game rules:
 * - Every cell is scenery (towers go here) or path (critters walk here).
 * - A map has exactly one entry and exactly one exit. Both must be path cells.
 * - A map is valid only if the entry is connected to the exit by a chain of
 *   path cells (moving up, down, left or right).
 *
 * Map is an Observable: every change to a cell, the entry or the exit
 * notifies the attached observers (see MapObserver).
 *
 * Cells are addressed by (row, column), with row 0 at the top and
 * column 0 on the left.
 */
class Map : public Observable
{
private:
    /** @brief Number of columns. */
    int width;
    /** @brief Number of rows. */
    int height;

    /** @brief The grid, indexed as cells[row][column]. */
    std::vector<std::vector<Cell> > cells;

    /** @brief Row of the entry cell, or -1 if no entry is set. */
    int entryRow;
    /** @brief Column of the entry cell, or -1 if no entry is set. */
    int entryColumn;

    /** @brief Row of the exit cell, or -1 if no exit is set. */
    int exitRow;
    /** @brief Column of the exit cell, or -1 if no exit is set. */
    int exitColumn;

public:
    /**
     * @brief Creates a blank map where every cell is scenery.
     * @param width  Number of columns.
     * @param height Number of rows.
     */
    Map(int width, int height);

    /** @brief Returns the number of columns. */
    int getWidth() const;
    /** @brief Returns the number of rows. */
    int getHeight() const;

    /**
     * @brief Sets the type of one cell and notifies the observers.
     *
     * Does nothing if (row, column) is outside the map. If the cell was the
     * entry or the exit and gets a different type, that entry or exit is
     * cleared.
     *
     * @param row    Row of the cell.
     * @param column Column of the cell.
     * @param type   SCENERY, PATH, ENTRY or EXIT.
     */
    void setCell(int row, int column, int type);

    /**
     * @brief Returns the type of a cell.
     * @return The cell type, or SCENERY if (row, column) is outside the map.
     */
    int getCellType(int row, int column) const;

    /**
     * @brief Sets the entry point and notifies the observers.
     *
     * The cell must already be a path cell. A previous entry becomes a normal
     * path cell again, so there is always at most one entry.
     *
     * @return true if the entry was set, false if the cell is outside the map
     *         or is not a path cell.
     */
    bool setEntry(int row, int column);

    /**
     * @brief Sets the exit point and notifies the observers.
     *
     * The cell must already be a path cell. A previous exit becomes a normal
     * path cell again, so there is always at most one exit.
     *
     * @return true if the exit was set, false if the cell is outside the map
     *         or is not a path cell.
     */
    bool setExit(int row, int column);

    /** @brief Returns the row of the entry, or -1 if none is set. */
    int getEntryRow() const;
    /** @brief Returns the column of the entry, or -1 if none is set. */
    int getEntryColumn() const;

    /** @brief Returns the row of the exit, or -1 if none is set. */
    int getExitRow() const;
    /** @brief Returns the column of the exit, or -1 if none is set. */
    int getExitColumn() const;

    /** @brief Tells whether (row, column) is inside the grid. */
    bool isInside(int row, int column) const;

    /**
     * @brief Checks the map rules.
     *
     * A map is valid when an entry and an exit are set, there is exactly one
     * of each, and a breadth-first search over path cells reaches the exit
     * from the entry.
     *
     * @return true if the map is valid.
     */
    bool isValid() const;

    /**
     * @brief Prints the map as text.
     *
     * Uses E for the entry, X for the exit, # for path cells and . for
     * scenery cells.
     */
    void display() const;
};

#endif
