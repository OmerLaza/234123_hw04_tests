Tests for part 2 of ```234123 - Operating Systems``` HW04 course spring 2023 semester.

# Requirements
* cmake version 3.14 (or higher) or any version that supports ```FetchContent```.This is needed to download the googletest library.
* GoogleTest library (will be downloaded automatically by cmake).
* C++ compiler (tested with gcc) that supports C++14 standard, since it is required by GoogleTest.

# How to run
* You need to add a function called ```void _reset_all();``` to ```test_part2.cpp``` file, it should reset all the statistics, I use it before every test so each test can start from the base case.
* Project structure (you can change it however you want, but you will need to change the CMakeLists.txt file accordingly):
```
.
├── malloc_2.cpp
├── CMakeLists.txt
├── README.md
├── test
│   ├── part2
│   │   ├── test_part2.cpp
```
* I used CLion IDE, so I just opened the project and ran the tests, with the GUI for gtest. If you use other IDEs or the CLI IDK just google it.

# Notes
* I hope you find this useful, and if found any bugs please let me know. So I can fix them for everyone :)
* I'm not responsible for any wild pokemon that may appear while using this code.
* Don't forget to drink water.