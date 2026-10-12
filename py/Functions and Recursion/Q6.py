def ic(inch): # type: ignore
    return inch * 2.5 # type: ignore
    
n = int(input("enter a inches : "))
print(f"the inches in cms = {ic(n)}")
    