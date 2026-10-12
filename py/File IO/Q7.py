with open("log.txt","r") as f:
    lines = f.readlines()
    
lineno = 1
for line in lines:
    if("python" in line):
        print(f"python in present , line no is {lineno}")
        break
    lineno+=1
    
else:
    print("python is not present")
            