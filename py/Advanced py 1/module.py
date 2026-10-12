def myFunc():
    print("Hello World!")

myFunc()
print(__name__)

if __name__ == "__main__":
    print("we are directly running this file")
    myFunc()
    print(__name__)