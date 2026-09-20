# 🎮 SudokuMaster

A **console-based Sudoku game built in C++** with multiple difficulty levels, scoring, hints, undo/redo functionality, and a simple interactive menu.

## 📌 About the Project

**SudokuMaster** is a C++ implementation of the classic Sudoku puzzle game.

The game provides randomly selected puzzles based on the chosen difficulty level and allows the player to solve them interactively through the console.

It also includes features such as:

* 🎯 Multiple difficulty levels
* 💡 Hints
* ↩️ Undo
* ↪️ Redo
* 🏆 Score tracking
* ✅ Move validation
* 📖 Built-in Sudoku rules
* 🎨 Colored console output
* 🧩 Predefined Sudoku puzzles

---

## ✨ Features

### 🎚️ Difficulty Levels

The game supports three difficulty levels:

| Difficulty | Maximum Hints |
| ---------- | ------------- |
| 🟢 Easy    | 7             |
| 🟡 Medium  | 5             |
| 🔴 Hard    | 3             |

A puzzle is randomly selected from the corresponding puzzle file when a new game starts.

---

### 🏆 Scoring System

Players earn or lose points depending on the actions they perform.

| Action       | Score   |
| ------------ | ------- |
| Correct move | **+10** |
| Undo         | **-2**  |
| Hint         | **-3**  |

The score is maintained throughout the game.

---

### 💡 Hint System

The game provides hints to help the player solve difficult parts of the puzzle.

The number of hints available depends on the selected difficulty:

* Easy → 7 hints
* Medium → 5 hints
* Hard → 3 hints

Using a hint also reduces the score.

---

### ↩️ Undo & ↪️ Redo

Players can undo previous moves and redo moves when needed.

The project implements separate **undo and redo stacks** to keep track of player moves.

---

### ✅ Move Validation

Every move is checked before being placed on the board.

The game verifies Sudoku constraints across:

* Rows
* Columns
* 3 × 3 blocks

Invalid moves are rejected and the player is asked to try again.

---

### 📖 Built-in Rules

The main menu includes a **Rules** option that displays the Sudoku rules stored in:

```text
Game Rules/Rules.txt
```

---

## 🗂️ Project Structure

```text
SudokuMaster/
│
├── project.cpp
├── display.cpp
│
├── Game Rules/
│   └── Rules.txt
│
├── Puzzles/
│   ├── easy.txt
│   ├── medium.txt
│   └── hard.txt
│
└── .gitignore
```

### 📄 File Description

**`project.cpp`**

Contains the main Sudoku game implementation, including:

* Game menu
* Puzzle loading
* Difficulty selection
* Board validation
* Score management
* Hint system
* Undo/redo
* Game completion
* User interaction

**`display.cpp`**

Contains a simple standalone Sudoku board display function for displaying a Sudoku grid in the console.

**`Game Rules/Rules.txt`**

Contains the rules displayed by the game.

**`Puzzles/`**

Contains the Sudoku puzzle data separated by difficulty.

---

## 🛠️ Technologies Used

* **C++**
* Standard C++ libraries
* File handling
* Arrays
* Classes
* Stacks
* Random puzzle selection
* Console-based user interaction

The project also uses Windows-specific console functionality such as:

```cpp
#include <conio.h>
#include <windows.h>
```

Therefore, the current version is primarily intended to run on **Windows**.

---

## 💻 Requirements

To run the project, you need:

* Windows
* A C++ compiler such as **MinGW / GCC**
* A terminal or command prompt

You can also use an IDE such as:

* Visual Studio Code
* Code::Blocks
* Visual Studio

---

## 🚀 How to Run

### 1. Clone the repository

```bash
git clone https://github.com/Samruddhi335/SudokuMaster.git
```

### 2. Enter the project directory

```bash
cd SudokuMaster
```

### 3. Compile the project

Using `g++`:

```bash
g++ project.cpp -o SudokuMaster.exe
```

### 4. Run the game

```bash
SudokuMaster.exe
```

> **Important:** Run the executable from the project root directory so that the program can correctly locate:
>
> ```text
> Puzzles/
> Game Rules/
> ```

---

## 🎮 How to Play

When the game starts, the main menu is displayed:

```text
***** MAIN MENU *****

1. NEW GAME
2. RULES
3. QUIT
```

### Start a New Game

Select:

```text
1. NEW GAME
```

The game loads a puzzle and displays the Sudoku board.

You will then enter:

* Row number
* Column number
* Value

For example:

```text
Enter row number: 1
Enter column number: 3
Enter value: 7
```

The game checks whether the move is valid.

After a valid move, you can choose what to do next:

```text
1. CONTINUE
2. UNDO
3. REDO
4. HINT
5. EXIT
```

---

## 🧩 Sudoku Rules

The game follows the standard Sudoku rules:

1. The Sudoku grid consists of **9 × 9 cells**.
2. Only numbers from **1 to 9** can be used.
3. Each **3 × 3 block** must contain valid Sudoku numbers.
4. Each **column** must contain numbers without repetition.
5. Each **row** must contain numbers without repetition.

---

## 📊 Game Logic

The game represents the Sudoku board using a 2D array:

```cpp
int board[9][9];
```

Each puzzle is loaded from a text file inside the `Puzzles` directory.

The program then validates each player move using Sudoku constraints.

The project also maintains a separate cell-status array to distinguish between:

* Pre-filled cells
* Empty cells
* User-entered cells
* Backtracked cells

---

## 🔄 Undo / Redo Implementation

The project uses a custom stack structure to store player moves.

```cpp
class stk
{
public:
    int i, j, value;
};
```

Two stacks are maintained:

```text
Undo Stack
     ↓
Player moves

Redo Stack
     ↓
Undone moves
```

This allows the player to move backward and forward through their actions.

---

## 📁 Puzzle Files

The project contains three puzzle files:

```text
Puzzles/
├── easy.txt
├── medium.txt
└── hard.txt
```

Each file contains Sudoku boards represented using numbers.

A `0` represents an empty cell.

Example:

```text
0 5 0 0 0 0 0 6 0
8 6 9 0 0 0 7 1 4
0 0 0 8 0 4 0 0 0
```

---

## 🔒 Git Ignore

Compiled executables and VS Code configuration are excluded from the repository using `.gitignore`:

```gitignore
*.exe
.vscode/
```

This keeps the GitHub repository focused on the **source code and project resources** rather than generated files.

---

## 🎯 Future Improvements

Some possible improvements for future versions include:

* [ ] Add a graphical user interface
* [ ] Add more Sudoku puzzles
* [ ] Add a puzzle generator
* [ ] Add difficulty-based puzzle generation
* [ ] Add a timer
* [ ] Add a leaderboard
* [ ] Save and resume games
* [ ] Improve cross-platform compatibility
* [ ] Refactor the project into multiple C++ source/header files
* [ ] Add automated tests

---

## 👩‍💻 Author

**Your Name**

GitHub: `https://github.com/Samruddhi335`

---

## ⭐ Acknowledgement

This project was created as a C++ Sudoku game to practice:

* Object-oriented programming
* Arrays
* Stack-based data structures
* File handling
* Input validation
* Game logic
* Problem solving

If you find the project interesting, consider giving the repository a ⭐ on GitHub!
