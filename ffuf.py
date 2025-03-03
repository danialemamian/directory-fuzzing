import subprocess
import time
import re
import argparse

# Function to run ffuf and handle 500 errors
def run_ffuf(wordlist, url, rate):
    ffuf_command = [
        "ffuf",
        "-w", wordlist,
        "-u", url,
        "-rate", rate,
        "-ac",
        "-c",
        "-r"
    ]

    process = subprocess.Popen(ffuf_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    while True:
        output = process.stdout.readline()
        if output == '' and process.poll() is not None:
            break

        if output:
            print(output.strip())  # Print ffuf output

            # Check for 500 error
            if re.search(r"\[Status: 500", output):
                print("[!] Detected 500 error. Pausing for 60 seconds...")
                time.sleep(60)  # Pause for 60 seconds
                print("[+] Resuming ffuf scan...")

    # Wait for the process to complete
    process.wait()

if __name__ == "__main__":
    # Set up argument parser
    parser = argparse.ArgumentParser(description="Run ffuf with 500 error handling.")
    parser.add_argument("-w", "--wordlist", required=True, help="Path to the wordlist file.")
    parser.add_argument("-u", "--url", required=True, help="Target URL with FUZZ placeholder.")
    parser.add_argument("-r", "--rate", default="3", help="Rate of requests per second (default: 3).")

    # Parse arguments
    args = parser.parse_args()

    print("Starting ffuf scan...")
    run_ffuf(args.wordlist, args.url, args.rate)
    print("ffuf scan completed.")
