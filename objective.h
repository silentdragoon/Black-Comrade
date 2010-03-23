#ifndef OBJECTIVE_H
#define OBJECTIVE_H

class Objective {
    private:
        int health;
    public:
        Objective();
        ~Objective();

        int getHealth();
        void damageObjective();
};

#endif
