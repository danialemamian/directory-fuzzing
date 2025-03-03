# FFUF Monitor with 500 Error Handling

This Python script automates the process of running the `ffuf` tool, monitors its output for HTTP 500 errors, and pauses for 60 seconds if such an error is encountered. After the pause, it resumes the task.

## Features
- Runs `ffuf` with a specified wordlist and target URL.
- Monitors `ffuf` output in real-time.
- Detects HTTP 500 errors and pauses execution for 60 seconds.
- Resumes scanning automatically after the pause.

## Prerequisites
- Python 3.x
- `ffuf` installed and accessible in your system's PATH.

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/ffuf-monitor.git
   cd ffuf-monitor
