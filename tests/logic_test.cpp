#include <cassert>
#include "logic.hpp"
int main(){
    assert(qol::release_tick(5,6));
    assert(!qol::release_tick(4,6));
    assert(!qol::release_tick(1,1));
    assert(!qol::shoulder_chord(0xff));
    assert(qol::shoulder_chord(0xfe));
}
