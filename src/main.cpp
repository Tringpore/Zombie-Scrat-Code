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
	Motor FL(FLport, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
	Motor BL(BLport, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	Motor FR(FRport, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	Motor BR(BRport, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
	Motor cataL(cataLport, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	Motor cataR(cataRport, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
	Motor intake(intakeport, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);

	pros::Imu IMU(IMUPort);
  	IMU.reset();

	FL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	BL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	FR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	BR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	Task catControlTask(catControl, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
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

void base(double left, double right) {
	Motor FL(FLport);
	Motor BL(BLport);
	Motor FR(FRport);
	Motor BR(BRport);

	FL.move(left);
	BL.move(left);
	FR.move(right);
	BR.move(right);
}

void baseMove(double dis, double speed, double cutoff) {
	Motor FL(FLport);
	Motor FR(FRport);

	double startPos = fabs((FL.get_position() + FR.get_position())/2);
	base(speed, speed);
	while(fabs(fabs(FL.get_position() + FR.get_position())/2 - startPos) < dis) delay(5);
	base(0, 0);
}

void baseTurn (double angle, double speed, double cutoff) {
	Imu IMU(IMUPort);
	int dir = angle - IMU.get_heading() > 0 ? 1 : -1;
	base(dir * speed, dir * (-speed));
	while (fabs(angle - IMU.get_heading()) < 2) delay(5);
	base(0, 0);
}

void autonomous() {
	baseMove(4000, 85, 100000000000);
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
				double power =  master.get_analog(ANALOG_LEFT_Y);
				double turn = master.get_analog(ANALOG_RIGHT_X);
				left = power + turn;
				right = power - turn;
			}
		FL.move(left);
		BL.move(left);
		FR.move(right);
		BR.move(right);	

		if(master.get_digital_new_press(DIGITAL_R1)) shootCata();

		intake.move(120 * (master.get_digital(DIGITAL_L1) - master.get_digital(DIGITAL_L2)));
		
		delay(5);
	}
}
