from gettrophies import set_token, total_request

def main():
    games = "usersGames.json"
    tUrl = "https://m.np.playstation.com/api/trophy/v1/users/me/trophyTitles"
    total_request(tUrl, games)
    
if __name__ == "__main__":
    main()