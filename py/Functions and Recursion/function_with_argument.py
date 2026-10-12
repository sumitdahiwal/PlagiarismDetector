
def gday(name, ending):  # type: ignore
    print("Hello, " + name)  # type: ignore
    print(ending)  # type: ignore
    return "done"
    
nick = input("Enter your name:")    
a = gday(nick, "thank you")
print(a)   