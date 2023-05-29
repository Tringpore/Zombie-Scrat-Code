#include "main.h"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "globals.hpp"
#include "mech_lib.hpp"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {

}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {

}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	Controller master(pros::E_CONTROLLER_MASTER);
	Motor FL(FLport);
	Motor BL(BLport);
	Motor FR(FRport);
	Motor BR(BRport);

	Motor cataL(cataLport);
	Motor cataR(cataRport);

	Motor intake(intakeport);

	bool tankdrive = true;

	while(true){
		double left, right;
		if(master.get_digital_new_press(DIGITAL_Y)) tankdrive = !tankdrive;
			if(tankdrive) {
				left = master.get_analog(ANALOG_LEFT_Y);
				right = master.get_analog(ANALOG_RIGHT_Y);
			} 
			
			else {
				double power = master.get_analog(ANALOG_LEFT_Y);
				double turn = master.get_analog(ANALOG_RIGHT_X);
				left = power + turn;
				right = power - turn;
			}
		FL.move(left);
		BL.move(left);
		FR.move(right);
		BR.move(right);

		cataL.move(120 * master.get_digital(DIGITAL_R1));
		cataR.move(-120 * master.get_digital(DIGITAL_R1));

		intake.move(-120 * (master.get_digital(DIGITAL_L1) - master.get_digital(DIGITAL_L2)));
	}
}
