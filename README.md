Made with the basics of C++ and python.\
This program would be much better and simpler if wrote completely in Python or C++, but I wanted to see if I could make a parser with C++ and requests with Python is so much easier. Anyway I'm happy that I created a unique functional program without any guides or tutorials.

If you want to help me make this into an actual good app worthy of it's potential hmu on discord: https://discord.gg/CNCSrYeH

Before running this program sign into the official playstation store and login.\
Then go to https://ca.account.sony.com/api/v1/ssocookie \
This is your npsso which is required for the API to verify who you are, the program will prompt when to enter this (copy it without the quotes).

![image](https://github.com/user-attachments/assets/536a0a00-6867-4208-8d31-92b0cf049118)

Use case for current build is best for adding the txt file that is updated from the app to something like OBS to track trophy count for selected game.

Right now it only updates a txt file with the trophy count of a selected game, but the json files that are returned with the python requests contain much more fun trophy info to use for an app. 

Only tested on Windows 11, but to the best of my knowledge it adheres to cross-platform functionalities (i'm noob though).

Utilized https://andshrew.github.io/PlayStation-Trophies/#/APIv2 for the Playstation API because Sony requires that you have a buisiness to get access to the official API.

To compile:

install msys2 and refer to docs to use g++ https://www.msys2.org/

In cmd go to the source files folder directory (or set path in command) and enter:\
(optionally add -O2 after g++ for optimizations if you are not testing)

g++ -std=c++17 -pthread main.cpp -o main -I"(set your python folder path here)Python313\include" -L"(set your python folder path here)Python313\libs" -lpython313 -static-libgcc -static-libstdc++
