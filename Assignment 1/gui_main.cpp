#include "Map.h"
#include "MapObserver.h"
#include "Cell.h"

int main()
{
    // Create a 12 x 10 map
    Map map(12, 10);

    // Create a horizontal path across the middle
    for (int column = 0; column < 12; ++column)
    {
        map.setCell(5, column, PATH);
    }

    // Set entry and exit points
    map.setEntry(5, 0);
    map.setExit(5, 11);

    // Create the graphical observer
    MapObserver mapObserver(map);

    // Register the observer with the map
    map.attach(&mapObserver);

    // Open and run the GUI
    mapObserver.run();

    // Unregister before the observer is destroyed
    map.detach(&mapObserver);

    return 0;
}