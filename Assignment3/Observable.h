#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "Observer.h"
#include <vector>
#include <algorithm>

/**
 * @file Observable.h
 * @brief Subject side of the Observer design pattern.
 */

/**
 * @brief Base class for objects that notify observers of changes.
 *
 * Map, Tower, Critter and CritterGroup inherit from this class. It keeps a
 * list of Observer pointers and calls Observer::update() on each of them when
 * the derived class calls notifyObservers().
 *
 * The Observable does not own its observers. It stores raw pointers, so an
 * observer must outlive the subject or be detached before it is destroyed.
 *
 * @see Observer
 */
class Observable
{
    private:
        /** @brief Observers currently registered with this object. */
        std::vector<Observer*> observers;

    public:
        virtual ~Observable() {}

        /**
         * @brief Registers an observer so it is notified of future changes.
         *
         * Null pointers and observers that are already registered are ignored,
         * so an observer is never notified twice for the same change.
         *
         * @param observer The observer to register.
         */
        void attach(Observer* observer)
        {
            if (observer != NULL &&
                std::find(
                    observers.begin(),
                    observers.end(),
                    observer
                ) == observers.end())
            {
                observers.push_back(observer);
            }
        }

        /**
         * @brief Unregisters an observer so it is no longer notified.
         *
         * Does nothing if the observer was not registered.
         *
         * @param Observer The observer to remove.
         */
        void detach(Observer* Observer)
        {
            observers.erase(
                std::remove(
                    observers.begin(),
                    observers.end(),
                    Observer
                ),
                observers.end()
            );
        }

    protected:
        /**
         * @brief Calls update() on every registered observer.
         *
         * Derived classes call this after each change to their state. It
         * loops over a copy of the observer list, so an observer can safely
         * detach itself while it is being notified.
         */
        void notifyObservers()
        {
            std::vector<Observer*> currentObservers = observers;

            for (std::size_t i=0;
                i < currentObservers.size();
                ++i)
            {
                if (currentObservers[i] != NULL)
                {
                    currentObservers[i]->update();
                }
            }
        }
};

#endif
