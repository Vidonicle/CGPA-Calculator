# CGPA Calculator

A simple command-line CGPA calculator written in C.

Calculates CGPA based on course letter grades and course weights using the
Carleton University grading scale.

## Features
- Add courses with code, weight, and letter grade
- Automatically sorts courses by course code
- Displays total credits earned, credits completed, and CGPA
- Input validation and error handling
- Linked-list based storage

## Build and Usage
Requires a C compiler (GCC or Clang).

```bash
gcc -Wall -Wextra -std=c11 main.c cgpa.c ui_errors.c -o cgpa
```

Run the program from the terminal:
```bash
./cgpa
```

## Notes
- Intended for personal and educational use
- Grading scale is hard-coded for Carleton University
- This project is not affiliated with Carleton University in any way
- Built as a learning project to practice C, memory management, and CLI design


## Lessons Learned

What did you learn while building this project? What challenges did you face and how did you overcome them?

- Robust input handling in C requires explicit flushing and careful use of `fgets`.
- The `!fgets(...)` pattern is useful for detecting EOF and input errors cleanly.
- `printf` formatting is more powerful (and more subtle) than expected when aligning CLI output.
- Building a non-trivial program reinforced existing knowledge of structs, pointers, and dynamic memory management.

This project is complete as-is. Future improvements may be explored later.