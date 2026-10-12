with open("file.txt","r") as f:
    content1 = f.read()
with open("log.txt","r") as f:
    content2 = f.read()
    
if(content1 == content2):
    print("both files ae same")
else:
    print("both files are different")
        