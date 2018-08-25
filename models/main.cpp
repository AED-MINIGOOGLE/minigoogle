#include "Includes.h"
#include "SlimTree.h"

int main(){
    Object *o1 = new Object(0,"luis",2,true,4,3);
    Object *o2 = new Object(0,"laas",2,true,4,3);
    int dist = o1->calculateEditDistanceWithOther(o2);
    cout << dist << endl;
}