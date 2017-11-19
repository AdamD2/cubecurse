cubecurse
=========

Overview
--------

Cubecurse is a simple ncurses based timer developed for timing Rubik's Cube
speedsolves. 

Features
--------

* Title bar with version number
* Basic 3x3x3 scramble under the title bar
* Timer (not accurate)

Planned Features
----------------

* WCA Legal 3x3x3 scramble under the title bar
* Keyboard commands at the bottom of the screen
* Time history on the left of the screen, which can move up or down using arrow
  keys or vim keybindings and allows application of options to the time such
  as DNF, +2, delete or leave comment
* Save all times as a .json file with time, scramble and comments
* Timer in the middle of the screen with 2 or 3 decimal place precision
* Stats on the right side of the screen for best times, averages, number of
  times, current times and session average (feel free to suggest other stats)

Extended Features
-----------------

* Draw scramble
* Allow for the central timer to be scaled

Building
--------

git clone https://github.com/adamd2/cubecurse
cd cubecurse/
make
./cubecurse

Authors
-------

* Adam Douglas
