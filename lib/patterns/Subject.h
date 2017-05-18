#ifndef SUBJECT_H
#define SUBJECT_H

#include <stdlib.h>
#include <vector>
#include <stdio.h>

#include "Observer.h"

template <typename T>
class Subject {
    public:
        void attach(Observer<T> *obs){
            observers.push_back(obs);
        }
        
        void notify(T data) {
            for (int i = 0; i < observers.size(); i++) {
                observers[i]->update(data);
            }
        }
    private:
        std::vector <Observer<T> *> observers;
};

#endif // SUBJECT_H
