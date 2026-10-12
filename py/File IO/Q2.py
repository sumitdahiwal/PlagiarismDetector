import random

def game():
    print("you are playinf a game")
    score = random.randint(1,100)
    # fetch the score
    with open("hiscore.txt") as f:
        hiscore = f.read()
        if(hiscore!=""):
            hiscore = int(hiscore)
        else:
            hiscore = 0
    print(f"score : {score}")
    if(score > hiscore):
        with open("hiscore.txt","w") as f:
            f.write(str(score))
            
    return score       
                    

game()
