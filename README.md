# FFUF Monitor with 500 Error Handling

This C program automates the process of performing directory fuzzing on a given target URL. It monitors the HTTP response codes, specifically handling **500 errors** by pausing the execution for 120 seconds if encountered. After the pause, the program will resume the task, making it useful for bypassing Web Application Firewalls (WAFs) that temporarily block requests after detecting suspicious activity.

## Features
- Runs a fuzzing tool with a specified wordlist and target URL.
- Monitors the output in real-time.
- Detects HTTP **500** errors and pauses execution for 120 seconds.
- Resumes scanning automatically after the pause.
- Detects **0 status code** errors (indicating connection issues) and pauses for 300 seconds before resuming.
- Option to limit the rate of requests per second.

This tool is particularly useful when bypassing WAFs: Many websites with WAFs temporarily block your IP after detecting suspicious behavior. By pausing on HTTP **500 errors**, this program helps avoid IP bans and allows you to continue scanning after the cooldown period.

## Prerequisites
- **C Compiler** (e.g., GCC) to compile the program.
- **libcurl** library for HTTP requests.
- **Wordlist** file for fuzzing.

## Installation
1. Clone this repository:
    ```bash
    git clone https://github.com/danialemamian/directory-fuzzing
    ```
2. Navigate into the project directory:
    ```bash
    cd directory-fuzzing
    ```
3. Compile the program:
    ```bash
    gcc -o fuff fuff.c -lcurl
    ```

## Example Command

```bash
./fuff -w /path/to/wordlist.txt -u https://example.com/FUZZ -r 50 -o output.txt
