# Image and Sound Processing

## Description

Programming Concepts in Scientific Computing.

Authors: Yihan Wang, Fedor Sergeev

## Requirements

- C++ compiler supporting c++17
- CMake >=3.10
- Git
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
- [AudioFile](https://github.com/adamstark/AudioFile)
- [OpenCV](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
- [GoogleTest](http://google.github.io/googletest/)


## How to run

### Compilation

A. General

1. Clone the repository: `git clone https://github.com/TheodorSergeev/img_sound_proc.git` (alternatively download as _.zip_ and extract)

2. Navigate to the img_sound_proc directory and build opencv (this might take a while)
    - [Linux](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
        - Navigate to the build drectory: `cd opencv/build/`
        - Build: `cmake ../opencv-4.x && cmake --build .` (consider using cmake's `-j` option to speed up building)
        - Return to the base directory: `cd ../..`

2. Install GoogleTest
    - Linux: `sudo apt install libgtest-dev`
    - Windows: consult [GoogleTest docs](http://google.github.io/googletest/platforms.html)

3. Build the source files `cd img_sound_proc && cmake . && make`

B. (Optional) If you would like to use newer versions of the libraries or install them to a different path:

1. Clone the AudioFile library `git clone https://github.com/adamstark/AudioFile.git` (alternatively download as _.zip_ and extract)

2. Download the Eigen library from the [website](https://eigen.tuxfamily.org/index.php?title=Main_Page) (for example as _.zip_) and extract it alongside the _img_sound_proc_ and _AudioFile_ folders

3. Install OpenCV
    - [Linux](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
        - Download and extract files (assuming the prerequisites are installed (cmake g++ wget unzip)
            ```
            wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip`
            unzip opencv.zip`
            mkdir -p build && cd build
            ```
        - Build from the source code: `cmake ../opencv-4.x && cmake --build .` (consider using cmake's `-j` option to speed up building)
    - [Windows](https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html)

4. Modify paths in the `CMakeLists.txt`, if needed (e.g., different versions or installation paths)

    ```
    ...
    include_directories(eigen-3.4.0/)
    include_directories(AudioFile/)
    include_directories(opencv/build/opencv2/)
    include_directories(opencv/build/lib/)
    set("OpenCV_DIR" "opencv/build/")
    ...
    ```

C. (Experimental) If you use Ubuntu-like Linux distribution you might be able to use opencv from this repository without rebuilding. However, this is not guaranteed to work.
    
1. Change paths in the opencv's text build files
    - Uncomment the block after `# (experimental) change opencv paths in text build files` in `CMakeLists.txt`.
    - Change the `PATH_NEW` variable to the new path of the opencv directory with respect to `/home/`.
    
2. Change paths in opencv's binary files
    - Use patchelf: [stackoverflow thread](https://stackoverflow.com/questions/13769141/can-i-change-rpath-in-an-already-compiled-binary)
    - Or create a new configuration file: [github issue thread](https://github.com/cggos/ccv/issues/1)

### Usage

- The command is structured the following way
    `./img_sound_proc <input file name> <output file name> <transform> <parameters>`

- Examples:
    - Apply a threshold [30, 200]: `./img_sound_proc /data/images/cameraman.tif /out.png 30 200`
    - ...

- Running tests:
    - After building simply run `ctest` in the `img_sound_proc` folder

## Contents

- `/data/`: folder with test samples
- `main.cpp`: ...
- `utils.hpp`, `utils.cpp`: ...
- `CMakeLists.txt`: ...
- `.gitignore`: ...

## Future work

- Implement new IO options (audio processing using AudioFile)
- Implement color image processing (possibly vector of Eigen matrices for channels)
- Add more transforms (contour extraction and noise removal)
- Simplify OpenCV building (select options to turn off non-IO components)
- Develop new processing method (currently it is not possible to chain 2 transforms without saving an intermediate file, which is not ideal if the transforms are applied to multiple images)
