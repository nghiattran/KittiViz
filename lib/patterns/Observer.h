#ifndef OBSERVER_H
#define OBSERVER_H

template <class T>

class Observer {
    public:
        virtual void update(T data) = 0;
    protected:
};

#endif // OBSERVER_H
