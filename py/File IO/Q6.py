with open("log.txt","r") as f:
    content = f.read()
    
if("python" in content):
    print("python is present")
else:
    print("python is not present")
    
        