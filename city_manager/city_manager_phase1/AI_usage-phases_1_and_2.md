## How I Used AI
I used AI for a specific part of the project. I only used it for the **Filter Logic** section.

### Parts developed with AI:
- `filter_logic.h`: AI helped me define the function signatures.
- `filter_logic.c`: I used AI to write the string parsing logic (`parse_condition`) and the comparison logic (`is_match`).

### Why I used AI:
The project document allowed us to use AI for the filtering part so, I asked the AI for a clean and fast solution.

## What I Learned
- I learned how to split a string with a colon (`:`) delimiter.
- I learned how to compare different data types (int and string) for filtering reports.

## Verification
I checked all the code from the AI. I tested it with different filter commands, and it works correctly with my binary file system.

## Phase 2: Processes and Signals

I wrote the code for Phase 2 myself. I only used AI to understand the theoretical concepts of system calls because `signal()` was not allowed.

* **How I used it:** I asked the AI to explain how `sigaction()` works with a simple example. I also asked how to safely delete a directory using `fork()` without creating zombie processes.
* **What I learned:** I learned that `sigaction()` is safer than `signal()` because we can block other signals during execution using `sigemptyset()`. I also learned that the parent process must use `wait()` while the child process runs `execlp("rm", ...)` to prevent zombie processes.

## Verification

I tested all the code manually. I used the terminal to send the `SIGUSR1` signal and verified the `reports.dat` binary file shifts correctly when a report is removed.