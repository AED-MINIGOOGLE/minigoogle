#ifndef NODE_H
#define NODE_H

#include "Object.h"

class Node{
    private:
        string idNode;
        bool isLeaf; //Is leaf or index 
        vector<Object*> objects;
        Object * repObject;
        int C; // number max of entries
        int count;
        bool isFull = false;
    public:
        int insertObject(Object * e);
        void runSplitting();
        void incrCount();
    friend class SlimTree;
};
int Node::insertObject(Object * o){
    this->objects.push_back(o);
    return count++;
}
void Node::runSplitting(){

}
void incrCount(){
    this->count++;
    if(count == C) this->isFull = true;
}
#endif