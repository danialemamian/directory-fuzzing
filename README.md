# FFUF Monitor with 500 Error Handling

This Python script automates the process of running the `ffuf` tool, monitors its output for HTTP 500 errors, and pauses for 60 seconds if such an error is encountered. After the pause, it resumes the task. This is particularly useful for bypassing Web Application Firewalls (WAFs) that may temporarily ban your IP after detecting suspicious activity.
## Features
- Runs `ffuf` with a specified wordlist and target URL.
- Monitors `ffuf` output in real-time.
- Detects HTTP 500 errors and pauses execution for 60 seconds.
- Resumes scanning automatically after the pause.
- **Efficient for bypassing WAFs**: Many sites with WAFs will ban your IP temporarily after detecting excessive requests or suspicious activity. By pausing on HTTP 500 errors, this script helps avoid IP bans and allows you to continue scanning after the WAF cooldown period.

## Prerequisites
- Python 3.x
- `ffuf` installed and accessible in your system's PATH.

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/danialemamian/directory-fuzzing
   cd ffuf-monitor
## Example Command
The default `ffuf` command in the script is:

```bash
ffuf -w /path/to/wordlist.txt -u https://example.com/FUZZ -rate 1 -ac -c -r

## Example Output
Starting ffuf scan...
.git/HEAD               [Status: 500, Size: 38652, Words: 305, Lines: 1, Duration: 12ms]
[!] Detected 500 error. Pausing for 60 seconds...
[+] Resuming ffuf scan...
CVS/Entries             [Status: 500, Size: 38654, Words: 305, Lines: 1, Duration: 13ms]
[!] Detected 500 error. Pausing for 60 seconds...
[+] Resuming ffuf scan...
ffuf scan completed.
