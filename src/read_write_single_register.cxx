#include <cstdlib>    // for rand() function
#include <iostream>
#include <string>

#include "uhal/uhal.hpp"

// How to run it:
// bin/read_write_single_register config/uhal/connections.xml dummy.udp A


int main(int argc, char* argv[])
{
  // PART 1: Argument parsing
  if (argc != 4) {
    std::cout << "Incorrect usage!" << std::endl;
    std::cout << "usage: read_write_single_register <path_to_connection_file> <device_id> <register_name>" << std::endl;
    return 1;
  }

  const std::string lConnectionFilePath = argv[1];
  const std::string lDeviceId = argv[2];
  const std::string lRegisterName = argv[3];


  // PART 2: Creating the HwInterface
  uhal::ConnectionManager lConnectionMgr("file://" + lConnectionFilePath);
  uhal::HwInterface lHW = lConnectionMgr.getDevice(lDeviceId);
  const uhal::Node& lNode = lHW.getNode(lRegisterName);


  // PART 3: Reading from the register
  std::cout << "Reading from register '" << lRegisterName << "' ..." << std::endl;
  uhal::ValWord<uint32_t> lReg = lNode.read();
  // dispatch method sends read request to hardware, and waits for result to return
  // N.B. Before dispatch, lReg.valid() == false, and lReg.value() will throw
  lHW.dispatch();

  std::cout << "... success!" << std::endl << "Value = 0x" << std::hex << lReg.value() << std::endl;


  // PART 4: Writing (random value) to the register
  std::cout << "Writing random value to register '" << lRegisterName << "' ..." << std::endl;
  lNode.write(rand());
  // N.B. Depending on how many transactions are already queued, this write request may either be sent to the board during the write method, or when the dispatch method is called
  lHW.dispatch();
  // In case there are any problems with the transaction, an exception will be thrown from the dispatch method
  // Alternatively, if you want to check whether an individual write succeeded or failed, you can call the 'valid()' method of the uhal::ValHeader object that is returned by the write method
  std::cout << "... success!" << std::endl;

  return 0;
}