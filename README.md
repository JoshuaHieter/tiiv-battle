# tiiv-battle
Battle simulator for Twilight Imperium Fourth Edition

How to use:
First compile the project using the command
#make opt
this will create a battle.out file
to run this file, use the following syntax, this works for linux only, though editing the makefile to output an exe is very easy
#./battle.out [-r <int runCount>] -p [-s <string shipName> <int shipCount>] -p [-s <string shipName> <int shipCount>]
you may repeat the [-s <string shipName> <int shipCount>] multiple times as to create different types of ships in each fleet
the -r denotes you want to repeat the simulation multiple times, -p denotes a new fleet, -s denotes a new ship
