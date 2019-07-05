#include <iostream>
#include "world/World.h"

using namespace std;
using namespace z2;
int main(){
    World w(5,5);
    w.getTile(1,1).removeEntity();
    
}