
def f_to_c(f): # type: ignore
    c = (f - 32) * 5/9 # type: ignore
    return c # type: ignore
f = int(input("Enter a Farenheit number : "))
c = f_to_c(f)
print(f"{round(c,2)}°C")
