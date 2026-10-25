#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "Observer.h"
#include <vector>
#include <algorithm>

/**
 * Base clears for objects that notify observers of changes
*/
class Observable
{
    private:
        std::vector<Observer*> observers;

    public:
        virtual ~Observable() {}

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
        void notifyObservers{}
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