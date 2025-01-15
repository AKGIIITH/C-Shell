# Mini Project 1: Building Your Own Shell

## Overview
In this project, you'll create a custom shell using C, implementing modular code to ensure scalability and maintainability. The shell will mimic various functionalities found in Bash, including executing commands, piping, redirection, process management, and more.

---

## Instructions and Guidelines
- **Modular Code**: Separate your code into multiple files based on tasks. Use header files for declarations. Monolithic code will incur heavy penalties.
- **C Language Only**: You must write this project in C. Third-party libraries are not allowed.
- **Error Handling**: Handle all errors gracefully. Shell crashes or segmentation faults will lead to penalties.
- **Standards Compliance**: Use only C standard libraries. For references, visit: [C Standard Libraries](https://en.cppreference.com/w/c/header).
- **Signal Handling**: Implement custom signal handlers for process management and user interactions.

---

## Submission Guidelines
1. **Repository Branches**:
   - `mid`: For the first submission.
   - `final`: For the final submission.
2. **Deadlines**:
   - **1st Deadline**: 11:59 PM, 30th Aug 2024 (Mid branch). Only Part A is required.
   - **2nd Deadline**: 11:59 PM, 13th Sep 2024 (Final branch). Both Parts A and B must be submitted.

### Scoring Rules:
- Points from Part A submitted during the 1st deadline will count as 50% of the total Part A score.
- If Part A is incomplete during the 1st deadline, the remaining points will not be recoverable in the 2nd submission.

---

## Project Specifications

### Part A: Basic System Calls
1. **Display Requirements**:
   - Implement a custom shell prompt showing `<Username@SystemName:~>`.
   - Reflect current working directory and handle relative/absolute paths.

2. **Input Requirements**:
   - Support `;` and `&` for command separation and background execution.
   - Handle extra spaces and tabs in input.

3. **Commands**:
   - **`hop`**: Change directory with support for `~`, `..`, `-`.
   - **`reveal`**: List files/directories with flags `-a` and `-l`.
   - **`log`**: Implement command history (15 commands max).
   - **System Commands**: Execute external system commands with foreground and background support.
   - **`proclore`**: Display process details.
   - **`seek`**: Search for files/directories with advanced flag support.

4. **Miscellaneous**:
   - Custom error messages for invalid commands.
   - Color-coded outputs for better visualization.

---

### Part B: Processes, Files, and I/O
5. **Configuration**:
   - Implement `.myshrc` with aliases and optional bonus functions (`mk_hop`, `hop_seek`).

6. **I/O Redirection**:
   - Support `>`, `<`, and `>>`.

7. **Pipes**:
   - Implement pipes for inter-process communication.

8. **Process Management**:
   - List running processes (`activities`).
   - Manage foreground/background processes (`fg`, `bg`).
   - Send signals to processes (`ping`).

---

### Part C: Networking
9. **Networking**:
   - Implement `iMan` to fetch man pages from the internet using sockets.

---

## Evaluation Policy
- **Plagiarism**: Strictly prohibited. MOSS will be used to evaluate submissions against current and past submissions.
- **Viva**: Be prepared to explain and defend your code during evaluations.
- **Penalty**: Non-compilation, segmentation faults, and crashes will result in heavy penalties.

---

## Additional Notes
- Refer to man pages for all commands and structures used.
- Use `uname`, `getcwd`, `chdir`, `waitpid`, `fork`, `execvp`, and related system calls for functionality.
- Avoid using `popen`, `pclose`, or `system()`.

Happy coding!


