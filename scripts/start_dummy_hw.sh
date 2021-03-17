 #!/usr/bin/env
 echo "Starting Dummy Hardware @ localhost:50001 ..."
 
/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe -p 50001 -v 2 & 

 echo "Starting Dummy Hardware @ localhost:50002 ..."
/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe -p 50002 -v 2 &

 echo "Starting Dummy Hardware @ localhost:50003 ..."
/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe -p 50003 -v 2 &

