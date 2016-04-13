## Structure Description

### `3dShapes` folder:
This is the project that generates the 3D shapes from the song. All shapes are generated with FFT from the same project. 

Requirements:

* OF 0.9.4 (0.9.0+ probably works as well)
* Have the [ofxFft](https://github.com/kylemcdonald/ofxFft) addon installed. (**Note:** The precompiled binary for FFTW provided by the ofxFft repo might not work and you might need to compile one yourself)

---

The data folder will be all the input and output for the program:

* Avenir fonts just aesthetics
* `MovinOnUp.mp3`, the song in question
* `result` folder, where the results are copied to manually after it's generated and saved into the `data` folder.
*  `song.txt`, the FFT data saved into a text file. It will be read by the program on startup, more below.
* `settings.xml`, settings for creating the shape, including the type, height, size etc. The program includes a GUI to control it, simply press `b` to rebuild the shape after changing anything.
  * `settings_*.xml` are provided tuned presets to generate 4 different types of shapes. Copy and paste the text in these files to `settings.xml` and press `b` to rebuild shape.

---

Currently the program will read prerecorded FFT data from `song.txt`. If you need to record a new set of FFT data, comment and uncomment the relevant lines in `ofApp.cpp` and `fftShapes.cpp` as instructed by the code comments, run the program once and let the song play through its entirety and you will have a new `song.txt`!