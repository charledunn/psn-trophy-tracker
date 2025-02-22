Made with the basics of C++ and python.

Use case for current build is best for adding the txt file that is updated from the app to something like OBS to track trophy count for selected game.

Right now it only updates a txt file with the trophy count of a selected game, but the json files that are returned with the python requests contain much more fun trophy info to use for an app. 

Only tested on Windows 11, but to the best of my knowledge it adheres to cross-platform functionalities (i'm noob though).

Utilized https://andshrew.github.io/PlayStation-Trophies/#/APIv2 for the Playstation API because Sony requires that you have a buisiness to get access to the official API.

To compile:

install msys2 and refer to docs to use g++ https://www.msys2.org/

In cmd go to the source files folder directory (or set path in command) and enter:

g++ -std=c++17 -pthread main.cpp -o main -I"(set your python folder path here)Python313\include" -L"(set your python folder path here)Python313\libs" -lpython313 -static-libgcc -static-libstdc++
