import requests
import keyring
import time
import json
import os.path
from getauth import get_auth
from cryptography.fernet import Fernet

def set_token():
    fernet = Fernet(keyring.get_password("PSNToken", "key").encode())
    with open("token.bin", "rb") as f:
        binToken = f.read()
    token = fernet.decrypt(binToken).decode()
    return token

class SetGame:
    if not os.path.exists("selectedgame.bin"):
        with open("selectedgame.bin", "w") as initfile:
            pass
    with open("selectedgame.bin", "rb") as f:
        binGame = f.read()
    selectedGame = binGame.decode()
    total = f"https://m.np.playstation.com/api/trophy/v1/npCommunicationIds/{selectedGame}/trophyGroups/all/trophies"
    earned = f"https://m.np.playstation.com/api/trophy/v1/users/me/npCommunicationIds/{selectedGame}/trophyGroups/all/trophies"
    
def total_request(aurl, afile):
    while True:
        token = set_token()
        headers = { "Authorization": f"Bearer {token}" }
        r = requests.get(aurl, headers=headers)
        result = r.content.decode()
        try:
            if json.loads(result)['error']['message'] == "Expired token":
                exit = False
                while True:
                    answer = input("Expired token would you like to update the token? y/n: ")
                    if answer == "n":
                        exit = True
                        break
                    elif answer != "y":
                        print("Invalid input")
                        time.sleep(0.2)
                    elif answer == "y":
                        get_auth()
                        break
                if exit == True:
                    print("Exiting program..")
                    time.sleep(0.5)
                    break
            elif json.loads(result)['error']['message'] == "Resource not found":
                print("Make sure you have launched the game at least once\n")
                break
        except:
            if r.status_code == 429:
                nextAvail = r.headers['X-RateLimit-Next-Available']
                print(f"Oops too many requests, try again after {nextAvail}")
            else:
                try:
                    with open(afile, "w") as f:
                        f.write(json.dumps(json.loads(result), indent=4))
                    break
                except:
                    print("No games associated with user account.\n")
                    break
        else:
            break

def main():
    totalJson = "totalTrophies.json"
    earnedJson = "earnedTrophies.json"
    gameUrl = SetGame()
    total_request(gameUrl.total, totalJson)
    total_request(gameUrl.earned, earnedJson)
    
if __name__ == "__main__":
    main()