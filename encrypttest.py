import keyring
import json
from cryptography.fernet import Fernet

# def encryptToken():
    # token = "testToken"
    # key = keyring.get_password("PSNToken", "key").encode()
    # fernet = Fernet(key)
    # encToken = fernet.encrypt(token.encode())
    # return encToken

# def storeKey():
    # key = Fernet.generate_key()
    # keyring.set_password("PSNToken", "key", key.decode())
    
# def saveToken(data):
    # with open('cred.bin', 'wb') as f:
        # f.write(data)

# def decryptToken():
    # key = keyring.get_password("PSNToken", "key").encode()
    # fernet = Fernet(key)
    # with open('token.bin', 'rb') as f:
        # binToken = f.read()
    # decryToken = fernet.decrypt(binToken).decode()
    # print(decryToken)
    
# def testbin():
    # with open('selectedgame.bin', 'rb') as f:
        # gamebin = f.read()
    # print(gamebin)
    
# def whatKeyring():
    # if keyring.get_password("test", "test") == None:
        # print("hehe")
    # else:
        # print("haha")
    
# def json_parse():
    # with open("totalTrophies.json", "r") as f:
        # data = f.read()
    # ddata = json.dumps(data)
    # print(ddata['totalItemCount'])

# class set_gameurl:
    # with open("selectedgame.bin", "rb") as f:
        # binGame = f.read()
    # selectedGame = binGame.decode()
    # earnedUrl = f"https://m.np.playstation.com/api/trophy/v1/users/me/npCommunicationIds/{selectedGame}/trophyGroups/all/trophies"
    # totalUrl = f"https://m.np.playstation.com/api/trophy/v1/npCommunicationIds/{selectedGame}/trophyGroups/all/trophies"

class testy:
    statusCode = 429;
    
if 
# x = set_gameurl()
# print(x.earnedUrl)
# json_parse()
# storeKey()
# encryptToken()
# saveToken(encryptToken())
# decryptToken()
# testbin()
# whatKeyring()