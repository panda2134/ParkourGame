#include "world.h"

bool parkour::World::getReady() const
{
    return ready;
}

void parkour::World::setReady(bool value)
{
    ready = value;
}
