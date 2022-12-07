# Image and Sound Processing

## Description

Programming Concepts in Scientific Computing.

Authors: Yihan Wang, Fedor Sergeev

## Requirements

- C++ compiler supporting c++17
- CMake >=3.16
- Git
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
- [AudioFile](https://github.com/adamstark/AudioFile)
- [OpenCV](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)


## How to run

### Compilation

1. Clone the repository: `git clone https://github.com/TheodorSergeev/img_sound_proc.git` (alternatively download as _.zip_ and extract)
2. Clone the AudioFile library `git clone https://github.com/adamstark/AudioFile.git` (alternatively download as _.zip_ and extract)
3. Download the Eigen library from the [website](https://eigen.tuxfamily.org/index.php?title=Main_Page) (for example as _.zip_) and extract it alongside the _img_sound_proc_ and _AudioFile_ folders
4. Install OpenCV
    - [Linux](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
        - Download and extract files (assuming the prerequisites are installed (cmake g++ wget unzip)
        ```
           wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip`
           unzip opencv.zip`
           mkdir -p build && cd build
        ```
        - Build from the source code: `cmake ../opencv-4.x && cmake -- build .` (consider using cmake's `-j` option to speed up building)
    - [Windows](https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html)
5. (Optional) Modify paths in the `CMakeLists.txt`, if needed (e.g., different versions or installation paths):

    ```
    # ...

    include_directories(../eigen-3.4.0)
    include_directories(../AudioFile)
    include_directories(../opencv/build/include/opencv2)

    set("OpenCV_DIR" "../opencv/build/")

    # ...
    ```
6. Build the source files `cd img_sound_proc && cmake . && make`

### Usage

The command is structured the following way

`./img_sound_proc <input file name> <output file name> <transform> <parameters>`

Examples:

- Apply a threshold [30, 200]: `./img_sound_proc /data/images/cameraman.tif /out.png 30 200`
- ...

## Contents

- `/data/`: folder with test samples
- `main.cpp`: ...
- `utils.hpp`, `utils.cpp`: ...
- `CMakeLists.txt`: ...
- `.gitignore`: ...