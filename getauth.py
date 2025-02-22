import requests
import keyring
import time
from cryptography.fernet import Fernet
from urllib.parse import urlparse, parse_qs

def store_key():
    if keyring.get_password("PSNToken", "key") == None:
        key = Fernet.generate_key()
        keyring.set_password("PSNToken", "key", key.decode())

def get_auth():
    newNpsso = False
    while True:
        if keyring.get_password("PSNnpsso", "npsso") == None:
            npsso = input("Enter your npsso and press enter: ")
        elif keyring.get_password("PSNnpsso", "npsso") != None and newNpsso == True:
            keyring.delete_password("PSNnpsso", "npsso")
            npsso = input("Npsso has expired, follow original steps to aquire new npsso. Enter your npsso and press enter: ")
        else:
            npsso = keyring.get_password("PSNnpsso", "npsso")
            
        codeQueryUrl = ("https://ca.account.sony.com/api/authz/v3/oauth/authorize?access_type=offline&"
        "client_id=09515159-7237-4370-9b40-3806e67c0891&"
        "response_type=code&"
        "scope=psn:mobile.v2.core psn:clientapp&"
        "redirect_uri=com.scee.psxandroid.scecompcall://redirect")
        
        codeHeaders = { 'Cookie': f'npsso = {npsso}' }
        try:
            codeRequest = requests.get(codeQueryUrl, headers=codeHeaders)
        except requests.exceptions.InvalidSchema as err:
            errstr = str(err)
            eParseUrl = urlparse(errstr)
            eQuery = parse_qs(eParseUrl.query)
        
        fernet = Fernet(keyring.get_password("PSNToken", "key").encode())
        codeparam = str(eQuery['code'])
        tokenHeaders = { 'Authorization': 'Basic MDk1MTUxNTktNzIzNy00MzcwLTliNDAtMzgwNmU2N2MwODkxOnVjUGprYTV0bnRCMktxc1A=', 'Content-Type': 'application/x-www-form-urlencoded', 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) WindowsPowerShell/7.4.0', }
        payload = { 'code': codeparam.strip("'[]"), 'redirect_uri': 'com.scee.psxandroid.scecompcall://redirect', 'grant_type': 'authorization_code', 'token_format': 'jwt' }
        tokenUrl = "https://ca.account.sony.com/api/authz/v3/oauth/token"
        try:
            tokenRequest = requests.post(tokenUrl, headers=tokenHeaders, data=payload)
            result = tokenRequest.json()
            token = result['access_token']
            encToken = fernet.encrypt(token.encode())
            with open('token.bin', 'wb') as f:
                f.write(encToken)
            print("Token granted")
            if keyring.get_password("PSNnpsso", "npsso") == None:
                keyring.set_password("PSNnpsso", "npsso", npsso)
            break
        except requests.RequestException:
            if tokenRequest.status_code == 429:
                print('Too many requests ', tokenRequest.headers)
                print("Exiting program..")
                time.sleep(0.5)
                break
        except:
            answer = input('Unable to obtain token, would you like to enter a new npsso? y/n: ')
            if answer == "n":
                print("Exiting program..")
                time.sleep(0.5)
                break
            elif answer != "y":
                break
            newNpsso = True
        
def main():
    store_key()
    get_auth()

if __name__ == "__main__":
    main()