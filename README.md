# Mastermind Game

## Authors

- [@LuisDA39](https://github.com/LuisDA39)
- [@Kurisari](https://www.github.com/kurisari)
- [@Nemesis1174](https://github.com/Nemesis1174)
- [@cidreee](https://github.com/cidreee)
- [@samir-bstb](https://github.com/samir-bstb)

This C++ program implements the classic Mastermind game, allowing users to play against the computer or vice versa. The game supports two modes: 

1. **PC Creates Code, User Deciphers**
2. **User Creates Code, PC Deciphers**

The program also provides options to view the last game, save the last game to a binary file, save the last game to a text file, or exit the program.

## How to Play

### PC Creates Code, User Deciphers
The computer generates a secret code, and the user attempts to guess it within 10 attempts. The user provides a sequence of colors as input, and after each attempt, the program provides feedback on the correct positions and colors guessed.

### User Creates Code, PC Deciphers
The user generates a secret code, and the computer attempts to guess it within 10 attempts. The user inputs a sequence of colors for the computer to guess, and after each attempt, the program provides feedback on the correct positions and colors guessed.

## Classes

### `MastermindJugada`
- Represents a move in the game.
- Contains information about the length and colors chosen in a move.

### `Jugador`
- Abstract base class for game players.
- Defines the interface for creating a move (`crearJugada`).

### `JugadorPC`
- Derived class from `Jugador`.
- Represents the computer player.
- Implements the logic for generating moves based on feedback from previous attempts.

### `JugadorUsuario`
- Derived class from `Jugador`.
- Represents the user player.
- Takes user input for creating moves.

### `MastermindJuego`
- Manages the game flow, including turns, feedback, and determining the end of the game.
- Evaluates and provides feedback on user and computer moves.
- Handles game initialization and completion.

## Main Functionality

- The program offers a menu allowing users to select the game mode or perform other actions.
- The game is played interactively, providing feedback after each move.
- The program allows users to view the last game, save it to a binary file (`archivo.bin`), save it to a text file (`output.txt`), or exit the program.

## File Saving

- The program allows users to save the last game to a binary file (`archivo.bin`) or a text file (`output.txt`).

## How to Compile and Run

- Compile the program using a C++ compiler.
- Run the executable file.
- Follow the on-screen menu instructions to play the game or perform other actions.

**Note:** Make sure to handle the user input as specified in the prompts during the game.