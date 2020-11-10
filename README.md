# OpenCV Object Detection
Sample project to understand how to work with OpenCV and image processing.

# How to compile
First of all you have to have OpenCv installed in your computer. According to the docs, there should be two folders: one with the source downloaded from opencv page and other with the build binaries.
Then, follow this steps:

1. Find the CMakeLists.txt file.
2. Replace the line 12 with your OpenCv directory (set(OpenCV_DIR /path/to/opencv))
3. Replace the line 21 with your OpenCv include directory (set(OpenCV_INCLUDE_DIRS /path/to/opencv_binaries/include)
4. Replace the line 22 with your OpenCv library directory (set(OpenCV_LIBS /path/to/opencv_binaries/lib/{core_library}))

    * {core_library} = libopencv_core.dylib for Mac
    
    * {core_library} = cv2.so for Windows
5. Configure the project and finally build it.
