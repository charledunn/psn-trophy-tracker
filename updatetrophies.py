from gettrophies import *

def main():
    earnedJson = "earnedTrophies.json"
    gameurl = SetGame()
    total_request(gameurl.earned, earnedJson)

if __name__ == "__main__":
    main()