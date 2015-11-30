This is a library for PTC08 TTL JPEG CAMERA based on VC0706 chipset.

Installing
----------

This project uses cmake build environment.

To build this poroject run:

   mkdir build
   cd build
   cmake ..
   make

Samples
-------

To run sample go into:
build/bin

and then run:
   ./example_cam_PTC08

Configuration
-------------

If you want to use an QtSerialPort library, you need to enable USE_QtSerialPort option in CMakeLists.txt.

Useful links
------------

https://learn.adafruit.com/assets/196
http://www.adafruit.com/datasheets/VC0706protocol.pdf
https://www.sparkfun.com/products/retired/10061
http://www1.coocox.org/driver_repo/305488dd-734b-4cce-a8a4-39dcfef8cc66/html/group___p_t_c08.html
http://seeedstudio.com/wiki/Grove_-_Serial_Camera
