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
  // Read out the hand information and print it to the screen

  while(true) {
    std::cout << driver.hand_info << std::endl;
    driver.wait(50);
  }

  /////////////////////////////////////////////////////////////////////

  return 0;
}
