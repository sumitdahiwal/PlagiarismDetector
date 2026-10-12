import random

# List of words for the game
words = ["python", "developer", "keyboard", "computer", "programming", "algorithm", "machine", "science"]

def scramble_word(word):
    scrambled = list(word)
    random.shuffle(scrambled)
    return ''.join(scrambled)

def play_game():
    word = random.choice(words)  # Pick a random word
    scrambled = scramble_word(word)  # Scramble the word
    attempts = 3

    print("\n🧩 Welcome to the Word Scramble Game!")
    print(f"Scrambled Word: {scrambled}")

    for attempt in range(1, attempts + 1):
        guess = input(f"Attempt {attempt}/{attempts}: Guess the word: ").lower()

        if guess == word:
            print(f"🎉 Correct! The word was '{word}'. You win!")
            return
        else:
            print("❌ Wrong guess. Try again.")

    print(f"😢 You lost! The correct word was '{word}'.")

# Start the game
play_game()
