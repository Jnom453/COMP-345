#ifndef OBSERVER_H
#define OBSERVER_H

/**
 * @file Observer.h
 * @brief Observer side of the Observer design pattern.
 */

/**
 * @brief Interface for objects that want to be told when an Observable changes.
 *
 * Every view in the project (MapObserver, TowerObserver, CritterObserver,
 * CritterGroupObserver) implements this interface. An observer registers
 * itself with an Observable through Observable::attach(). From then on the
 * Observable calls update() each time its state changes.
 *
 * @see Observable
 */
class Observer
{
    public:
        virtual ~Observer() {}

        /**
         * @brief Called by the observed object whenever its state changes.
         *
         * Each concrete observer decides how to react. The views in this
         * project only mark themselves as out of date; the window is redrawn
         * on its next frame.
         */
        virtual void update() = 0;
};

#endif
