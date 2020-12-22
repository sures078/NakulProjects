#Nakul Suresh sures078

  # I understand this is a graded, individual examination that may not be
  # discussed with anyone.  I also understand that obtaining solutions or
  # partial solutions from outside sources, or discussing
  # any aspect of the examination with anyone will result in failing the course.
  # I further certify that this program represents my own work and that none of
  # it was obtained from any source other than material presented as part of the
  # course.

import turtle
import random

def initializeGrid():
    grid = []
    for i in range(0,8):
        tempGrid = []
        for j in range(0,8):
            tempGrid += [0]
        grid += [tempGrid]
    grid[3][3] = 2
    grid[3][4] = 1
    grid[4][3] = 1
    grid[4][4] = 2
    return grid

def convertCoordinates(x,y): #converts coordinates from 2d list coordinates to turtle coordinates
    turtleX = 0
    turtleY = 0
    listY = [7,6,5,4,3,2,1,0]
    listX = [1,2,3,4,5,6,7,8]
    for i in range(0,8):
        if x == i:
            turtleY = listY[i]
        if y == i:
            turtleX = listX[i]
    return turtleX, turtleY

def isValidMove(board, row, col, color):
    valid = [] #checklist of 4 conditions
    counter = 0 #counts whether at least one immediate neighbor is a different color
    counter2 = 0 #increases if straight line ends with same color
    if row >= 0 and row < 8 and col >= 0 and col < 8: #checks if within bounds
        valid += [True]
        if board[row][col] == 0:
            valid += [True]
        rowNums = [row-1, row, row+1] #all possible x coordinates of neighbors
        colNums = [col-1, col, col+1] #all possible y coordinates of neighbors
        player = 0 #assigned 1 or 2 depending on color
        opponent = 0 #assigned 1 or 2 depending on color
        a = -1 #used to check whether straight line ends with same color
        b = -1 #used to check whether straight line ends with same color
        if color == 'black':
            player = 1
            opponent = 2
        elif color == 'white':
            player = 2
            opponent = 1
        for i in rowNums:
            for j in colNums:
                if i >= 0 and i < 8 and j >= 0 and j < 8: #makes sure not out of bounds
                    if i != row or j != col: #does not count (row, col)
                        if board[i][j] == opponent:
                            counter += 1
                            x = i+a
                            y = j+b
                            done = False
                            while not done:
                                if x < 0 or x > 7 or y < 0 or y > 7 or board[x][y] == 0: #checks if out of bounds or empty
                                    done = True
                                elif board[x][y] == player:
                                    counter2 += 1
                                    done = True
                                x += a
                                y += b
                b += 1
            a += 1
            b = -1
    if counter > 0:
        valid += [True]
    if counter2 > 0:
        valid += [True]
    if valid == [True,True,True,True]: #satisifes 4 conditions
        return True
    else:
        return False

def getValidMoves(board, color):
    coordList = []
    for i in range(0,len(board)):
        for j in range(0,len(board)):
            if isValidMove(board,i,j,color):
                coordList += [(i,j)]
    return coordList #list of tuple valid moves

def selectNextPlay(board):
    moves = getValidMoves(board, 'white') #list of valid moves for computer
    num = random.randint(0,len(moves)-1) #coordinate randomly chosen
    return moves[num]

def updateGrid(board, coor, color): #grid, tuple, color
    newBoard = []
    for i in board:
        tempBoard = []
        for j in i:
            tempBoard += [j]
        newBoard += [tempBoard]
    coordList = list(coor) #ordered pair converted to list
    x = coordList[0]
    y = coordList[1]
    player = 0
    opponent = 0
    if color == 'black':
        newBoard[x][y] = 1
        player = 1
        opponent = 2
    elif color == 'white':
        newBoard[x][y] = 2
        player = 2
        opponent = 1
    rowNums = [x-1, x, x+1] #all possible x coordinates of neighbors
    colNums = [y-1, y, y+1] #all possible y coordinates of neighbors
    a = -1 #index to keep checking further neighbors
    b = -1 #index to keep checking further neighbors
    for i in rowNums:
        for j in colNums:
            if i >= 0 and i < 8 and j >= 0 and j < 8: #makes sure not out of bounds
                if i != x or j != y: #does not count (x, y)
                    if newBoard[i][j] == opponent:
                        c = i+a #new coordinate to be checked
                        d = j+b #new coordinate to be checked
                        change = []
                        done = False
                        while not done:
                            if c < 0 or c > 7 or d < 0 or d > 7 or newBoard[c][d] == 0: #checks if out of bounds or empty
                                for thing in change:
                                    t1 = thing[0]
                                    t2 = thing[1]
                                    newBoard[t1][t2] = opponent #undoes changes made, if any
                                done = True
                            elif newBoard[c][d] == opponent:
                                newBoard[c][d] = player
                                change += [(c,d)] #if doesn't end with player, all these will be changed back
                            elif newBoard[c][d] == player:
                                newBoard[i][j] = player #original neighbor changed
                                done = True
                            c += a
                            d += b
            b += 1
        a += 1
        b = -1
    return newBoard

def isDone(board):
    done = False
    counter1 = 0
    counter2 = 0
    for i in board:
        for j in i:
            if j == 1:
                counter1 += 1
            elif j == 2:
                counter2 += 1
    if counter1 + counter2 == 64: #if whole board is filled
        done = True
    return done, counter1, counter2 #whether done, player score, computer score

def main():
    grid = initializeGrid()

    #creation of tile board

    screen = turtle.getscreen()
    screen.tracer(0) #turns turtle animation off
    screen.setworldcoordinates(0,0,8,8)
    screen.setup(1200,1200)

    tile = turtle.Turtle()
    tile.penup()
    tile.shape("square")
    tile.shapesize(3.9,3.9,1)
    tile.color("green")
    for i in range(1,9):
        for j in range(0,8):
            tile.goto(i,j)
            tile.stamp()
    turtle.penup()
    num1 = 7
    num2 = 0
    for i in range(0,8):
        turtle.goto(0,i)
        turtle.write(str(num1), font=("Arial", 20, "normal"))
        num1 -= 1
    for i in range(1,9):
        turtle.goto(i,8)
        turtle.write(str(num2), font=("Arial", 20, "normal"))
        num2 += 1

    human = turtle.Turtle()
    computer = turtle.Turtle()

    human.penup()
    human.shape("circle")
    human.color("black")
    human.shapesize(3.9,3.9)
    human.goto(4,3)
    human.stamp()
    human.goto(5,4)
    human.stamp()

    computer.penup()
    computer.shape("circle")
    computer.color("white")
    computer.shapesize(3.9,3.9)
    computer.goto(4,4)
    computer.stamp()
    computer.goto(5,3)
    computer.stamp()

    #loop of player response

    done = False
    while not done:
        gameover = False
        results = isDone(grid)
        response = 'something'
        if results[0]: #game done if whole board is filled
            myScore = results[1]
            compScore = results[2]
            if myScore > compScore:
                screen.textinput("Results", "You win! " + str(myScore) + ' - ' + str(compScore) + " Press enter to quit game.")
            elif myScore < compScore:
                screen.textinput("Results", "You lose. " + str(myScore) + ' - ' + str(compScore) + " Press enter to quit game.")
            elif myScore == compScore:
                screen.textinput("Results", "It's a tie: 32-32. Press enter to quit game.")
            done = True
            gameover = True
            turtle.bye() #closes turtle window after game is done

        elif len(getValidMoves(grid, 'black')) == 0 and len(getValidMoves(grid, 'white')) == 0: #game done if no more valid moves for both players
            myScore = results[1]
            compScore = results[2]
            if myScore > compScore:
                screen.textinput("Results", "You win! " + str(myScore) + ' - ' + str(compScore) + " Press enter to quit game.")
            elif myScore < compScore:
                screen.textinput("Results", "You lose. " + str(myScore) + ' - ' + str(compScore) + " Press enter to quit game.")
            elif myScore == compScore:
                screen.textinput("Results", "It's a tie: 32-32. Press enter to quit game.")
            done = True
            gameover = True
            turtle.bye() #closes turtle window after game is done

        elif len(getValidMoves(grid, 'black')) != 0: #game continues if player has valid move(s)
            coor = screen.textinput("Coordinates a,b", "Enter your next move: ").strip() #user must enter coordinates like > a,b
            if coor == '': #game done if user enters null string, no winner but score so far is displayed
                response = coor
                myScore = results[1]
                compScore = results[2]
                screen.textinput("Results", "Black: " + str(myScore) + " White: " + str(compScore) + " Press enter to quit game.")
                done = True
                gameover = True
                turtle.bye() #closes turtle window after game is done
            else:
                if not coor[0:1].isdigit() or coor[1:2] != ',' or not coor[2:3].isdigit() or len(coor) != 3: #if player doesn't enter a proper coordinate entry
                    done2 = False
                    while not done2:
                        coor = screen.textinput("Coordinates a,b", "Move not recognized, enter again:").strip()
                        if coor[0:1].isdigit() and coor[1:2] == ',' and coor[2:3].isdigit() and len(coor) == 3:
                            done2 = True
                coor = eval(coor) #converts string to tuple
                listified = list(coor)
                x = listified[0]
                y = listified[1]
                if not isValidMove(grid, x, y, 'black'): #makes sure player enters valid move
                    done3 = False
                    while not done3:
                        coor = screen.textinput("Coordinates a,b", "Not a valid move, enter again:").strip()
                        if coor[0:1].isdigit() and coor[1:2] == ',' and coor[2:3].isdigit() and len(coor) == 3:
                            coor = eval(coor) #converts string to tuple
                            listified = list(coor)
                            x = listified[0]
                            y = listified[1]
                            if isValidMove(grid, x, y, 'black'):
                                done3 = True
                coordList = getValidMoves(grid, 'black') #list of valid move tuples
                pair = ()
                for i in coordList:
                    if coor == i:
                        pair = coor
                grid = updateGrid(grid, pair, 'black') #updates grid, coordinates converted, turtle stamps
                for i in range(0,len(grid)):
                    for j in range(0,len(grid)):
                        if grid[i][j] == 1:
                            conv = convertCoordinates(i,j)
                            human.goto(conv[0],conv[1])
                            human.stamp()

        elif len(getValidMoves(grid, 'black')) == 0:
            screen.textinput("Next","Sorry, you are out of valid moves.")

        if len(getValidMoves(grid, 'white')) != 0 and response != '' and gameover == False: #game continues if computer has valid move(s)
            screen.textinput("Next","Nice move. Press enter for the computer's play.")
            compMove = selectNextPlay(grid) #tuple - computer's next move
            grid = updateGrid(grid, compMove, 'white') #updates grid, coordinates converted, turtle stamps
            for i in range(0,len(grid)):
                for j in range(0,len(grid)):
                    if grid[i][j] == 2:
                        conv = convertCoordinates(i,j)
                        computer.goto(conv[0],conv[1])
                        computer.stamp()

        elif len(getValidMoves(grid, 'white')) == 0 and gameover == False:
            screen.textinput("Next","Sorry, Computer out of valid moves.")

if __name__ == '__main__':
    main()
