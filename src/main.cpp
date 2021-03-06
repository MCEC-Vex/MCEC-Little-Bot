#include "main.h"
#include <cmath>
#include "wheels.h"
#include "constants.h"
#include "arm.h"
#include "line_tracker.h"

// parts of the robot / controller
pros::Controller master(pros::E_CONTROLLER_MASTER);
Wheels hex_drive;
Arm arm;
Line_Tracker line_tracker;


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous()
{
	pros::delay(1000);
	hex_drive.drive(127, 127);
	pros::delay(1000);
	hex_drive.stop();
	pros::delay(500);
	hex_drive.rotate(-100);
	pros::delay(1000);
	hex_drive.stop();
	pros::delay(500);
	hex_drive.drive(-50, 0, 127);
	pros::delay(1000);
	hex_drive.stop();
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
void opcontrol()
{
	int test = 0;

	while (true)
	{
		// move the hex bot using controller input
		hex_drive.drive(master);

		// show wheel motor temperatures
		hex_drive.output_temperatures();

		// keep track of the bot's orientation
		hex_drive.run();

		// move arm using controller input
		arm.run(master);

		line_tracker.run();

		pros::lcd::set_text(5, std::to_string(line_tracker.get_cross_count()));

		// wait for a specific amount of time before looping
		pros::delay(TICK_DELAY);
	}
}
