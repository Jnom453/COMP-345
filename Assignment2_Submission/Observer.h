#ifndef OBSERVER_H
#define OBSERVER_H

/** 
 * Interface for objects that want to observe changes 
 */
class Observer 
{
    public:
        virtual ~Observer() {}

        /** Called when observed object changes **/
        virtual void update() = 0;
};

#endif
