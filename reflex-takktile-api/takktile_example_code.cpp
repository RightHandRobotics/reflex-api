#include <boost/bind.hpp>

#include "reflex_hand.h"
#include "reflex_driver.h"

int main() {

  /////////////////////////////////////////////////////////////////////
  // Always need this code to start the connection to the hand
  
  std::string network_interface = "eth0";
  std::string finger_file_name = "/finger_calibrate.yaml";
  std::string tactile_file_name = "/tactile_calibrate.yaml";
  std::string motor_file_name = "/motor_constants.yaml";

  reflex_driver::ReflexDriver driver(network_interface, finger_file_name, tactile_file_name, motor_file_name);
  
  reflex_hand::ReflexHandState *state = &driver.rh->rx_state_;
  driver.rh->setStateCallback(boost::bind(&reflex_driver::ReflexDriver::reflex_hand_state_cb, &driver, state));
  
  /////////////////////////////////////////////////////////////////////
  // Calibrate the fingers (make sure hand is opened in zero position)

  std::cout << "Starting calibration..." << std::endl;

  driver.calibrate();
  driver.wait(100);

  std::cout << "Calibration complete" << std::endl;
  driver.wait(500);

  /////////////////////////////////////////////////////////////////////
  // Demonstration of position control

  std::cout << "Moving fingers..." << std::endl;

  const float finger_positions[][NUM_SERVOS] = {
    {0.0, 0.0, 0.0, 0.0},
    {0.5, 0.5, 0.5, 0.0},
    {1.0, 1.0, 1.0, 0.0},
    {0.5, 0.5, 0.5, 0.0},
    {0.0, 0.0, 0.0, 0.0}
  };

  for (int i = 0; i < 5; i++) {
    driver.set_angle_position(finger_positions[i]);
    driver.wait(500);
  }

  std::cout << "Finger movement complete" << std::endl;
  driver.wait(500);

  /////////////////////////////////////////////////////////////////////
  // Demonstration of preshape joint

  std::cout << "Moving preshape joint..." << std::endl;

  const float preshape_positions[][NUM_SERVOS] = {
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.5},
    {0.0, 0.0, 0.0, 1.5},
    {0.0, 0.0, 0.0, 0.5},
    {0.0, 0.0, 0.0, 0.0},
  };

  for (int i = 0; i < 5; i++) {
    driver.set_angle_position(preshape_positions[i]);
    driver.wait(500);
  }

  std::cout << "Preshape movement complete" << std::endl;
  driver.wait(500);

  /////////////////////////////////////////////////////////////////////
  // Demonstration of changing velocity

  std::cout << "Changing to faster speed..." << std::endl;
  const float faster_velocities[] = {7.0, 7.0, 7.0, 7.0};
  driver.set_motor_speed(faster_velocities);

  for (int i = 0; i < 5; i++) {
    driver.set_angle_position(finger_positions[i]);
    driver.wait(500);
  }

  std::cout << "Changing to slower speed..." << std::endl;
  const float slower_velocities[] = {0.5, 0.5, 0.5, 0.5};
  driver.set_motor_speed(slower_velocities);

  for (int i = 0; i < 5; i++) {
    driver.set_angle_position(finger_positions[i]);
    driver.wait(500);
  }

  std::cout << "Done changing speeds" << std::endl;
  driver.wait(500);

  /////////////////////////////////////////////////////////////////////
  // Demonstration of blended control

  std::cout << "Starting blended control..." << std::endl;
  const float mixed_speeds[] = {0.5, 1.0, 1.5, 0.0};
  driver.set_motor_speed(mixed_speeds);

  for (int i = 0; i < 5; i++) {
    driver.set_angle_position(finger_positions[i]);
    driver.wait(500);
  }

  driver.set_motor_speed(slower_velocities);

  std::cout << "Blended control complete" << std::endl;
  driver.wait(500);

  /////////////////////////////////////////////////////////////////////
  // Demonstration of tactile feedback and setting sensor thresholds

  std::cout << "Starting tactile feedback..." 
            << std::endl
            << "All fingers will stop moving when any one makes contact"
            << std::endl; 
  const float move_speeds[] = {1.25, 1.25, 1.25};

  driver.populate_tactile_thresholds(15);

  driver.move_until_any_contact(move_speeds);
  driver.wait(1000);

  driver.set_angle_position(finger_positions[0]);
  driver.wait(500);

  std::cout << "Now each finger will only stop moving once it makes contact" << std::endl;

  const int thresholds[NUM_FINGERS * NUM_SENSORS_PER_FINGER] = {
    10, 10, 10, 10, 10, 10, 10, 10, 1000,
    20, 20, 20, 20, 20, 20, 20, 20, 1000,
    30, 30, 30, 30, 30, 30, 30, 30, 1000
  };

  driver.set_tactile_thresholds(thresholds);

  driver.move_until_each_contact(move_speeds);
  driver.wait(1000);

  driver.set_angle_position(finger_positions[0]);
  driver.wait(500);

  std::cout << "Tactile feedback complete" << std::endl;
  driver.wait(500);

  /////////////////////////////////////////////////////////////////////

  std::cout << "Finished all commands" << std::endl;
  
  return 0;
}
