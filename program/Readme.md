# Auto Numbers Collector and Clicker

A Program which automatically recognizes the button number and clicks for you.  
It writes everything into out/stats.csv

## How to run

All options can be defined in the [options.yml](.options.yml).

First, how many runs you want and your region of interest (x,y,width,height).
The region of interest are screen coordinates which contain only the coordinates.  
For example:   
![example ROI](example.png)

Run the program (it is a CLI program, so you should run it in a cmd.exe).

Make sure that the working directory, where you run the program, has access to the "cmp" directory, as those images are needed.  
Also make sure, the options.yml is also in the working directory.

If you have any problems with msvc.dll or something similar, you need the C++ runtime [from the MS website. Select the X64 one.](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#visual-studio-2015-2017-2019-and-2022)

## How to compile

OpenCV 4 needs to be installed, just run cmake.  
You need, of course, a C++ compiler like MSVC.  

There are tests you can execute to test if everything works fine.
It is required for the tests that the "cmp" directory is on your system on you don't move it.

Currently, only Windows is supported due to the click simulation.  
