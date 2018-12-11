cubecurse
=========

Overview
--------

Cubecurse is a simple ncurses based timer developed for timing Rubik's Cube
solves. It is licensed under the GPL v2.

Features
--------

* Basic 3x3x3 scramble under the title bar
* Accurate timer which times to three decimal places and outputs the times to
  the history window with two decimal place precision
* Saves all times in the session and keeps a record of the fastest time
* Keyboard commands at the bottom of the screen

Planned Features
----------------

* WCA Legal 3x3x3 scramble under the title bar
* Well formatted keyboard commands at the bottom of the screen
* Time history on the left of the screen, which can move up or down using arrow
  keys or vim keybindings and allows application of options to the time such
  as DNF, +2, delete or leave comment
* Save all times as a .json file with time, scramble and comments
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
