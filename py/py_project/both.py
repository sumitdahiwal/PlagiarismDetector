import random

def user_guess_game():
    number = random.randint(1, 1000)
    attempts = 100
    print("\nSumit Guess the number (1-1000). You have 100 attempts.")

    for guess in range(1, attempts + 1):
        user_guess = int(input(f"Attempt {guess}/{attempts}: Your guess? "))

        if user_guess > number:
            print("Too high! Try again.")
        elif user_guess < number:
            print("Too low! Try again.")
        else:
            print(f"🎉 Congrats! You guessed it in {guess} attempts.")
            return True  # User won

    print(f"❌ You lost! The correct number was {number}.")
    return False  # User lost


def computer_guess_game():
    low, high = 1, 100
    attempts = 6
    print("\n[COMPUTER GAME] Think of a number (1-100). The computer will guess it!")

    for guess in range(1, attempts + 1):
        comp_guess = random.randint(low, high)
        print(f"Attempt {guess}/{attempts}: Is it {comp_guess}?")
        feedback = input("Enter 'h' if too high, 'l' if too low, 'c' if correct: ").lower()

        if feedback == 'h':
            high = comp_guess - 1
        elif feedback == 'l':
            low = comp_guess + 1
        elif feedback == 'c':
            print(f"🤖 Computer guessed your number in {guess} attempts!")
            return True  # Computer won

    print("💻 Computer failed to guess your number!")
    return False  # Computer lost


# Run both games
user_won = user_guess_game()
computer_won = computer_guess_game()

# Determine the winner
print("\n--- GAME OVER ---")
if user_won and computer_won:
    print("🏆 It's a tie! Both guessed correctly.")
elif user_won:
    print("🎉 You win! The computer couldn't guess your number.")
elif computer_won:
    print("🤖 Computer wins! It guessed your number.")
else:
    print("😅 Nobody guessed correctly. Try again!")
