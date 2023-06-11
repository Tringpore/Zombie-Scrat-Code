#include "main.h"
#include "mech_lib.hpp"
#include "pros/rotation.hpp"

bool shoot = false;

void catControl(void*ignore) {
    Motor cataL(cataLport);
	Motor cataR(cataRport);

    Rotation rot(rotPort);
    // rot.set_reversed(true);
    // rot.reset_position();

    while(true) {
        if(shoot) {
            cataL.move(120);
            cataR.move(120);
            delay(200);
            
            cataL.move(0);
            cataR.move(0);
            delay(100);
            shoot = false;
        }else if(rot.get_angle() > 26400) {
            cataL.move(120);
            cataR.move(120);
        }else {
            cataL.move(0);
            cataR.move(0);
        }
        printf("%d\n", rot.get_angle());
        delay(5);
    }
}

void shootCata() {
    shoot = true;
}

