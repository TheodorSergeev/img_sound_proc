# Image and Sound Processing

## Description

Course project for the course Programming Concepts in Scientific Computing.

Authors: Yihan Wang, Fedor Sergeev

## Requirements

- C++ compiler supporting c++17
- CMake >=3.10
- Git
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
- [AudioFile](https://github.com/adamstark/AudioFile)
- [OpenCV](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
- [GoogleTest](http://google.github.io/googletest/)
- (Optional) [Doxygen](https://www.doxygen.nl/index.html)

## How to run

### Compilation

#### A. General

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

4. (Optional) Build the documentation `doxygen`

#### B. Optional 

If you would like to use newer versions of the libraries or install them to a different path:

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

#### C. Experimental 

If you use Ubuntu-like Linux distribution you might be able to use opencv from this repository without rebuilding. However, this is not guaranteed to work.
    
1. Change paths in the opencv's text build files
    - Uncomment the block after `# (experimental) change opencv paths in text build files` in `CMakeLists.txt`.
    - Change the `PATH_NEW` variable to the new path of the opencv directory with respect to `/home/`.
    
2. Change paths in opencv's binary files
    - Use patchelf: [stackoverflow thread](https://stackoverflow.com/questions/13769141/can-i-change-rpath-in-an-already-compiled-binary)
    - Or create a new configuration file: [github issue thread](https://github.com/cggos/ccv/issues/1)

### Usage

- The software is used through the command line. The user specifies a transform, an input and output file names, and possibly additional transform parameters. As a result, the transform with the specified parameters will be applied to the input data and the output will be saved to the specified location. Currently only grayscale images are supported as input, and output can be either and image or a text file depending on the transform.

- The command is structured the following way
    `./img_sound_proc <input file name> <output file name> <transform> <parameters>`

- **Examples** (here all available transforms are presented):
    - Apply a threshold [30, 200]: `./img_sound_proc /data/images/cameraman.tif /out.png 30 200`
    - Compute a histogram of an image: `./img_sound_proc /data/images/cameraman.tif /out.txt`
    - FFT2D transform for frequency domain: `./img_sound_proc fft2Dfreq /data/images/cameraman.tif /out.txt`
    - FFT2D transform for magnitude: `./img_sound_proc fft2Dmag /data/images/cameraman.tif /out.txt`
    - Lowpass filter: `./img_sound_proc lowpass /data/images/cameraman.tif /out.png 250`
    - Highpass filter: `./img_sound_proc highpass /data/images/cameraman.tif /out.png 250`
    - (parser not implemented) Inverse FFT2D transform: `./img_sound_proc ifft2D /in.txt /out.txt`

- **Tests** (to run simply execute `ctest` in the `img_sound_proc` folder):
    - `OPENCV2EIGEN`: correctness of opencv -> eigen matrix conversion (check size and coefficients in a constant matrix)
    - `EIGEN2OPENCV`: correctness of eigen -> opencv matrix conversion (check size and coefficients in a constant matrix)
    - `THRESHOLDING`: correctness of Thresholding transform (check output on a sample matrix)
    - `HISTOGRAM`: correctness of Thresholding transform (check output on a sample matrix)
    - `FFT1DTEST`: correctness of FFT1D transform (check output on a sample matrix)
    - `FFT2DTEST`: correctness of FFT2D transform (check output on a sample matrix)
    - `FFT1DANDINVERSE`: correctness of FFT1D and  iFFT1D transforms (check iFFT(FFT) = identity)
    - `FFT2DANDINVERSE`: correctness of FFT2D and  iFFT2D transforms (check iFFT(FFT) = identity)
    - `LOWPASSFILTER`: correctness of LowpassFilter transform (check output on a sample matrix)
    - `HIGHPASSFILTER`: correctness of HighpassFilter transform (check output on a sample matrix)

## Implementation details

The code follows the MVC (model-view-controller) pattern. 
- **Model.** Transformations are implemented as subclasses of abstract interface `Transform` (see `transforms.cpp` and `transforms.hpp`). The transform specifies as template parameters types of its input and output: particular types of Eigen matrices. It also implements the virtual method `apply` that actually performs the transformation. The transform can store its parameters as private members.
- **View.** The user interacts with the software through the command line and input/output files. We use OpenCV and AudiFile libraries to read and write the supported formats (currently grayscale images as input and output, and text as output). The IO handling and conversion to and from Eigen matrices, with which transform work, is done simply with function (see `utils.hpp` and `utils.cpp`).
- **Controller.** Each transform class has a dedicated parser class. These classes store the name of the transform, implement methods for reading its parameters from the command line, and invoke the transform with the specified input/output. Given a user's input, we iterate through all available transform, checking if their name matches the command. If it does, the parser is applied with the rest of the command line inputs (see `parsers.hpp` and `parsers.cpp`).

## Future work

#### Todo
- Implement parser for the inverse 2D Fourier transform (ifft2D, read complex matrix)
- Implement new IO options (audio processing using AudioFile)
- Create tests for parsers in particular and increase test coverage in general
- Add more transforms (contour extraction and noise removal)
- Simplify OpenCV building (select options to turn off non-IO components)


#### Limitations
- Only 1D or 2D input is supported (to enable color image processing could use vector of Eigen matrices for channels)
- Not possible to chain 2 transforms without saving an intermediate file, which is not ideal if the transforms are applied to multiple images
- Doesn't work on EPFL' VDI (not possible to build OpenCV and install Google Test)
