import random

def getPhrase():
    PhraseBank = open("phrasebank.txt").read().splitlines()
    index = random.randint(0,99)
    phrase = PhraseBank[index] #randomly selected puzzle phrase
    category = "Before and After" #categories based on index
    blanks = ""
    if index >= 20 and index < 40:
        category = "Song Lyrics"
    elif index >= 40 and index < 60:
        category = "Around the House"
    elif index >= 60 and index < 80:
        category = "Food and Drink"
    elif index >= 80:
        category = "Same Name"
    for i in range(0,len(phrase)):
        if (phrase[i] != " "):
            blanks += "_" #blanks that correspond to letters in the phrase
        else:
            blanks += " "
    return phrase, category, blanks

def spinTheWheel(ans, bal): #arguments contain phrase and user balance before turn
    spin = [50]
    spin += [100]*6
    spin += [200]*4
    spin += [250]*3
    spin += [500]*2
    spin += [750]*2
    spin += [1000, 2000, 5000, 10000, 'Bankrupt', 'Bankrupt'] #spin is a list of wheel values
    index = random.randint(0,23)
    counter = 0 #the number of occurences for the user's guess
    correct = False #whether the user makes a correct guess
    consonant = '' #the letter the user guesses
    done = False #turns true when the user inputs a correct consonant and ends the loop
    isBankrupt = False #checks to see if user is bankrupt, so an empty string won't be added to the list of consonants
    if spin[index] != 'Bankrupt':
        consonant = input("You've spun $" + str(spin[index]) + "! Guess a letter: ").upper().strip(" ")
        while not done:
            tempCounter = 0 #temporary counter to see if user mistakenly entered a vowel
            for i in 'AEIOU':
                if consonant == i:
                    tempCounter += 1
            if tempCounter == 0 and len(consonant) == 1 and consonant.isalpha(): #checks that user has a valid input
                for i in range(0, len(ans)):
                    if ans[i] == consonant:
                        counter += 1
                if counter > 0:
                    correct = True
                    win = counter*spin[index] #local variable that calculates winnings for the round
                    bal += win #updated balance
                    print("Congratulations, " + consonant + " appears in the phrase " + str(counter) + " time(s). You've won $" + str(win) + ".")
                elif counter == 0:
                    bal -= spin[index] #updated balance
                    print("No " + consonant + ". You lose $" + str(spin[index]))
                done = True
            else:
                consonant = input("Whoops, that's not a consonant! Guess again: ").upper().strip(" ") #input for if user enters an invalid entry
    else:
        print("Sorry, you're bankrupt.")
        if bal > 0:
            bal = 0 #sets balance equal to 0 if user's balance is positive
        isBankrupt = True
    return bal, consonant, correct, isBankrupt

def buyAVowel(ans, bal): #arguments contain phrase and user balance before turn
    counter = 0 #the number of occurences for the user's guess
    correct = False #whether the user makes a correct guess
    vowel = '' #the letter the user guesses
    done = False #turns true when the user inputs a correct vowel and ends the loop
    if bal >= 250: #checks to see if balance is at least $250
        bal -= 250 #updated balance
        vowel = input("Ok! $250 will be deducted from your winnings. Which vowel would you like to buy (A, E, I, O, U)? ").upper().strip(" ")
        while not done:
            tempCounter = 0 #temporary counter to see if user entered a vowel
            for i in 'AEIOU':
                if i == vowel:
                    tempCounter += 1
            if tempCounter == 1 and len(vowel) == 1 and vowel.isalpha(): #checks that user has a valid input
                for i in range(0, len(ans)):
                    if ans[i] == vowel:
                        counter += 1
                if counter > 0:
                    correct = True
                    print("Congratulations! " + vowel + " appears in the phrase " + str(counter) + " time(s)!")
                elif counter == 0:
                    print ("No " + vowel + ".")
                done = True
            else:
                vowel = input("Whoops, that's not a vowel! Guess again: ").upper().strip(" ") #input for if user enters an invalid entry
    elif bal < 250: #doesn't allow user to buy vowel if balance is less than $250
        print("Sorry your balance is less than $250, you cannot buy a vowel.")
    return bal, vowel, correct

def solveThePuzzle(ans, bal): #arguments contain phrase and user balance before turn
    answer = input("What's your best guess (be sure to enter your guess with single spaces!)? ").upper().strip(" ") #the user's guess
    correct = False #whether the user makes a correct guess
    if answer == ans:
        print("That's correct - you solved the puzzle!")
        correct = True
    elif answer != ans:
        bal = 0 #balance set to 0 if incorrect guess is made
        print("Sorry, that guess is incorrect! Your winnings will start over at $0 :(")
    return bal, correct

def main():
    info = getPhrase() #a randomly selected phrase, corresponding category and initial blanks stored
    balance = 0 #user balance
    done = False #turns true when user wins or loses and ends loop
    newList = [] #list of blanks that are replaced by letters each time a correct guess is made
    conList = [] #list of consonants guessed
    vowList = [] #list of vowels guessed
    for i in range(0,len(info[2])):
        newList += info[2][i] #initially all blanks
    print("Welcome to the Wheel of Fortune!")
    print("The phrase is:")
    print(info[2]) #inital blanks
    print("The category is: " + info[1])
    print("Your current winnings are: $" + str(balance))
    while not done:
        if len(conList) == 21 and len(vowList) == 5: #checks to see if all letters are guessed
            print("Sorry, you lose because you have guessed all the letters. You're winnings are $0.")
            done = True
        else:
            choice = input("Would you like to Spin the Wheel (type 'spin'), Buy a Vowel (type 'vowel'), or Solve the Puzzle (type 'solve')? ").lower().strip(" ")
            if choice == 'spin':
                spin = spinTheWheel(info[0],balance) #stores updated user balance, consonant guessed, whether correct and if bankrupt
                balance = spin[0]
                if not spin[3]: #if not banktrupt, adds consonant to list of consonants
                    conList += [spin[1]]
                newBlanks = '' #updated blanks after guess
                newCons = '' #updated consonants
                newVows = '' #updated vowels
                print("The phrase is:")
                for i in range(0,len(info[0])):
                    if spin[1] == info[0][i]: #checks to see if matches phrase
                        newList[i] = spin[1] #updates newList
                for i in range(0,len(newList)):
                    newBlanks += newList[i] #string of updated blanks
                for i in conList:
                    newCons += i + ' ' #string of updated consonants
                for i in vowList:
                    newVows += i + ' ' #string of updated vowels
                print(newBlanks)
                print("Vowels Guessed: " + newVows)
                print("Consonants Guessed: " + newCons)
                print("Your current winnings are: $" + str(balance))
            elif choice == 'vowel':
                data = buyAVowel(info[0],balance) #stores updated user balance, vowel guessed and whether correct
                balance = data[0]
                vowList += [data[1]] #updated list of vowels
                newBlanks = '' #updated blanks after guess
                newCons = '' #updated consonants
                newVows = '' #updated vowels
                print("The phrase is:")
                for i in range(0,len(info[0])):
                    if data[1] == info[0][i]: #checks to see if matches phrase
                        newList[i] = data[1] #updates newList
                for i in range(0,len(newList)):
                    newBlanks += newList[i] #string of updated blanks
                for i in conList:
                    newCons += i + ' ' #string of updated consonants
                for i in vowList:
                    newVows += i + ' ' #string of updated vowels
                print(newBlanks)
                print("Vowels Guessed: " + newVows)
                print("Consonants Guessed: " + newCons)
                print("Your current winnings are: $" + str(balance))
            elif choice == 'solve':
                stuff = solveThePuzzle(info[0],balance) #stores updated user balance and whether correct
                balance = stuff[0]
                newBlanks = '' #updated blanks after guess (should not change if user makes a wrong guess)
                newCons = '' #updated consonants (should not change if user makes a wrong guess)
                newVows = '' #updated vowels (should not change if user makes a wrong guess)
                if stuff[1] == False:
                    print("The phrase is:")
                    for i in range(0,len(newList)):
                        newBlanks += newList[i] #string of updated blanks
                    for i in conList:
                        newCons += i + ' ' #string of updated consonants
                    for i in vowList:
                        newVows += i + ' ' #string of updated vowels
                    print(newBlanks)
                    print("Vowels Guessed: " + newVows)
                    print("Consonants Guessed: " + newCons)
                    print("Your current winnings are: $" + str(balance))
                elif stuff[1] == True:
                    if balance < 0:
                        balance = 0 #sets balance to 0 if winnings are negative
                    print("Congratulations, you've won the game! You're winnings are $" + str(balance) + ". Thank you for playing the Wheel of Fortune!")
                    done = True
            else:
                print("Whoops, I don't recognize that input! Try again.") #if user enters anything other than spin, vowel or solve

if __name__ == '__main__':
    main()
