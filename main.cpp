#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <thread>
#include <future>
using namespace std;

struct subStrResults {
	size_t start;
	size_t begin;
	size_t end;
	subStrResults(const string&, const char&, const char&);
	subStrLength() {
		return end - begin;
	}
};

subStrResults::subStrResults(const string& lstring, const char& delim1, const char&delim2) {
	start = lstring.find(delim1);
	begin = lstring.find(delim2, start) + 1;
	end = lstring.find(delim2, begin + 1);
}

unordered_map<string, string> gameMap () {
	const char colon = ':';
	const char quote = '"';
	string line;
	string id;
	string name;
	string titleId = "npCommunicationId";
	string titleName = "trophyTitleName";
	unordered_map<string, string> gameIdmap;
	ifstream requestFile("usersGames.json");
	if (!requestFile.is_open()) {
		cout << "Can't open usersGames.json\n";
	} else {
		while (getline (requestFile, line)) {
			if (line.find(titleId) != string::npos) {
				subStrResults tId(line, colon, quote);
				id = line.substr(tId.begin, tId.subStrLength());
				transform(id.begin(), id.end(), id.begin(), ::toupper);
			}
			if (line.find(titleName) != string::npos) {
				subStrResults tName(line, colon, quote);
				name = line.substr(tName.begin, tName.subStrLength());
				transform(name.begin(), name.end(), name.begin(), ::toupper);
				gameIdmap[name] = id;
			}
		}
	}
	requestFile.close();
	return gameIdmap;
}

void unordMapToFile (const unordered_map<string, string>& umap, const string& mfile) {
	if (!filesystem::exists(mfile)) {
		ofstream initmfile(mfile.c_str());
		initmfile.close();
	}
	ofstream mfile1(mfile.c_str());
	if (!mfile1.is_open()) {
		cout << "Can't open " << mfile << " \n";
	} else {
		for (auto a : umap) {
			mfile1 << a.first << ">" << a.second << '\n';
		}
	}
	mfile1.close();
}

unordered_map<string, string> unordMapFromFile (const string& mfile) {
	string key;
	string value;
	string line;
	unordered_map<string, string> unordMap;
	if (!filesystem::exists(mfile)) {
		ofstream initmfile(mfile.c_str());
		initmfile.close();
	}
	ifstream mfile1(mfile.c_str());
	if (!mfile1.is_open()) {
		cout << "Can't open " << mfile << " \n";
	} else {
		while (getline (mfile1, line)) {
			if (line.find('>') != string::npos) {
				size_t pos = line.find('>');
				key = line.substr(0, pos);
				value = line.substr(pos + 1);
				unordMap[key] = value;
			}
		}
	}
	mfile1.close();
	return unordMap;
}

bool searchMap(unordered_map<string, string> umap) {
	string userSearch;
	string userOption;
	string prompt1 = "If you would like to see a list of your games enter the keyword: list \n\n"
	"If you are unsure of the exact name of your game you can enter a portion of of the title to see a list of titles that match "
	"(i.e \"god\" or \"war\" or \"go\"(without quotes) for God of War)\n"
	"Or if you want to cancel enter cancel: ";
	string prompt2 = "Enter full game name or part of name, list, or cancel: ";
	bool searchLoop = true;
	cout << prompt1;
	while (searchLoop) {
		for(int i = 0; i < 2; i++) {
			getline(cin, userSearch);
			transform(userSearch.begin(), userSearch.end(), userSearch.begin(), ::toupper);
			if(userSearch == "CANCEL") {
				cout << "Cancelled search..\n";
				i = 2;
				searchLoop = false;
			} else if (userSearch == "UPDATE") {
				cout << "Updating..\n";
				return true;
			} else if(userSearch == "LIST") {
				if (umap.empty()) {
					cout << "List empty!\n" << prompt2;
				} else {
					for (auto a : umap) {
						cout << a.first << '\n';
					}
					cout << prompt2;
				}
			} else {
				if(umap.find(userSearch) != umap.end()) {
					string foundGame = umap.find(userSearch)->second;
					size_t length = foundGame.length();
					ofstream writeGame("selectedgame.bin", ios::binary | ios::trunc);
					if(writeGame.is_open()) {
						writeGame.write(foundGame.c_str(), length);
						cout << "GAME SET" << '\n';
						searchLoop = false;
					} else cout << "Could not open selectedgame.bin\n";
					writeGame.close();
					return false;
				} else {
					bool notFound = true;
					cout << "'" << userSearch << "' " << "not found, here are some similar results: " << '\n';
					for (auto a : umap) {
						if(a.first.find(userSearch) != string::npos) {
							cout << a.first << '\n';
							notFound = false;
						}
					}
					if (notFound) {
						unordered_map<string, string> updateMap = gameMap();
						unordMapToFile(updateMap, "gameids.txt");
						umap = updateMap;
					}
					if (i = 1) {
						cout << "You may have to update the game list, if you would like to do so enter the keyword: update\n";
						cout << prompt2;
						
					}
				}
			}
		}
	}
	return false;
}

bool itemCount (const string& jsonFile) {
	const string totalCount = "totalItemCount";
	const char colon = ':';
	string countFound;
	string trophyCount;
	string cline;
	string tline;
	bool found = false;
	ifstream itemCountFile(jsonFile.c_str());
	if (!itemCountFile.is_open()) {
		cout << "Couldn't open " << jsonFile << '\n';
		return false;
	} else {
		while (getline (itemCountFile, cline)) {
			if (cline.find(totalCount) != string::npos) {
				countFound = cline;
				found = true;
			}
		}
	}
	itemCountFile.close();
	if (found) {
		if (countFound.find(colon) != string::npos) {
			size_t posStart = countFound.find(colon) + 2;
			trophyCount = countFound.substr(posStart);
		}
		if (!filesystem::exists("updatetrophy.txt")) {
			ofstream initFile("updatetrophy.txt");
			initFile.close();
		}
		fstream updateFile("updatetrophy.txt");
		if (!updateFile.is_open()) {
			cout << "Couldn't open updatetrophy.txt\n";
		} else {
			getline(updateFile, tline);
			if (!tline.empty()) {
				if (tline.find("/") != string::npos) {
					if (jsonFile != "totaltrophies.json") {
						updateFile.seekg(0);
						updateFile << trophyCount.append(tline, tline.find("/"));
					}
				} else {
					int tlineNum = stoi(tline);
					int trophyCountNum = stoi(trophyCount);
					if (tlineNum >= trophyCountNum) {
						updateFile.seekg(0);
						updateFile << trophyCount << '/' << tline;
					} else {
						updateFile.seekg(0);
						updateFile << tline << '/' << trophyCount;
					}
				}
			} else {
				updateFile.close();
				updateFile.open("updateTrophy.txt");
				if (updateFile.is_open()) {
					updateFile << trophyCount;
				}
			}
		}
		updateFile.close();
	}
	return true;
}

void checkStop() {
	bool inputget = true;
	while (inputget) {
		char checkInput = cin.get();
		if(checkInput == '\n') {
			inputget = false;
		}
	}
}

int main () {
	Py_Initialize();
	FILE* pauth = fopen("getauth.py", "r");
	PyRun_SimpleFile(pauth, "getauth.py");
	FILE* pgames = fopen("usersgames.py", "r");
	PyRun_SimpleFile(pgames, "usersgames.py");
	FILE* ptotal = fopen("gettrophies.py", "r");
	FILE* pearned = fopen("updatetrophies.py", "r");
	const string ttlGames = "usersGames.json";
	const string ttlTrophies = "totalTrophies.json";
	const string ernTrophies = "earnedTrophies.json";
	string mapFile = "gameids.txt";
	string updateToken = "UPDATE TOKEN";
	string selectGame = "SELECT GAME";
	string exit = "EXIT";
	string options = "UPDATE TOKEN/SELECT GAME/EXIT\n";
	string uinput;
	cout << "What would you like to do? " << options;
	bool running = true;
	while (running) {
		getline(cin, uinput);
		transform(uinput.begin(), uinput.end(), uinput.begin(), ::toupper);
		if (uinput == exit) {
			running = false;
		} else if (uinput == updateToken) {
			PyRun_SimpleFile(pauth, "getauth.py");
			cout << options;
		} else if (uinput == selectGame) {
			bool selectGame = true;
			bool getTotal = true;
			bool isItemCount = false;
			bool updGList = false;
			while (selectGame) {
				updGList = searchMap(unordMapFromFile(mapFile));
				if (updGList) {
					PyRun_SimpleFile(pgames, "usersgames.py");
					this_thread::sleep_for(3s);
				} else {
					if (getTotal) {
						PyRun_SimpleFile(ptotal, "gettrophies.py");
						isItemCount = itemCount(ttlTrophies);
						getTotal = false;
						this_thread::sleep_for(3s);
					}
					if (isItemCount) {
						bool updatingText = true;
						cout << "Auto updating trophies, press ENTER at any time to stop: \n";
						future<void> fut = async(launch::async, checkStop);
						while (updatingText) {
							this_thread::sleep_for(1s);
							auto status = fut.wait_for(0ms);
							PyRun_SimpleFile(pearned, "updatetrophies.py");
							this_thread::sleep_for(2s);
							if(itemCount(ernTrophies)) {
								if(status == future_status::ready) {
									updatingText = false;
									selectGame = false;
									cout << options;
								} 
							} else {
								updatingText = false;
								selectGame = false;
								cout << "Unable to update trophies, please check to ensure you have \nlaunched the game at least once and press ENTER: \n";
								fut.get();
								cout << options;
							}
						}
					} else {
						selectGame = false;
						cout << "Could not retrieve users games.\n";
						cout << options;
					}
				}
			}
		} else {
			cout << "Invalid input try again." << '\n';
		}
	}
	Py_FinalizeEx();
	cout << "Exiting program..";
	this_thread::sleep_for(1s);
}