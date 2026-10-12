f = open("poem.txt")
content = f.read()
if("twinkle" in content):
    print("the word is in the poem")
else:
    print("the word is not in the poem")
    
f.close()
    