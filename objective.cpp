#include "objective.h"

Objective::Objective() :
    health(10)
{}

Objective::~Objective() {}

int Objective::getHealth() { return health; }

void Objective::damageObjective() {
    if(health!=0) health--;
}
