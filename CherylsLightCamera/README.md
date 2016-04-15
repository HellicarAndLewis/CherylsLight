## Structure Description

### `CherylsLightCamera` folder:
This is the project runs on a Raspberry Pi receiving data from the Raspberry Pi Camera, process the data with OpenCV and output the result to the GPIO pins, controlling the LED dimmers. 

Requirements:

* OF 0.9.3 (0.9.0+ probably works as well) [for Raspberry Pi](http://openframeworks.cc/setup/raspberrypi/raspberry-pi-getting-started/).
* Raspberry Pi with 40 GPIO pins (Raspberry Pi B+, Raspberry Pi 2, *etc*) with Raspbian Jessie installed and GPIO pins enabled.
* Raspberry Pi Camera module attached and enabled.
* WiringPi installed on Raspberry Pi according to instruction [here](http://wiringpi.com/download-and-install/).
* On openframeworks folder on Raspberry Pi install the following addons:
  * [ofxRPiCameraVideoGrabber](https://github.com/jvcleave/ofxRPiCameraVideoGrabber) (**Note:** Clone the master branch and not download the release for OF 0.9.0 as it doesn't have the latest features used in this project)
  * [ofxCv](https://github.com/kylemcdonald/ofxCv)
  * ofxWiringPi is required but already included in the project.

---

To build and run the project:
1. Either `ssh` into the Raspberry Pi or use the command line on the Raspberry Pi directly.
2. `cd` into the project directory.
3. Run `make`.
4. When compilation is done, run `sudo make RunRelease` (`sudo` is required).

---

A few constants can be edited at the beginning of `setup()`. Because of the nature of WiringPi's pin numbering system, it is better to use the `registeredPins` vector to define which pins to use, for accurate documentation of the pin numbers see [here](http://pinout.xyz/pinout/wiringpi_gpio_pinout).

Currently the program only looks for the difference in the video feed and map that number to the pin outputs uniformly.