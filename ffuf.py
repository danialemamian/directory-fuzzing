import subprocess
import time
import re

# Define the ffuf command
ffuf_command = [
    "ffuf",
    "-w", "/media/danial/f9e7e1d7-a04f-4bac-b16b-a85a5b2a5fed/WordList-main/common.txt",
    "-u", "https://lm.bki.ir/FUZZ",
    "-rate", "3",
    "-ac",
    "-c",
    "-r"
]

# Function to run ffuf and handle 500 errors
def run_ffuf():
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
                time.sleep(90)  # Pause for 90 seconds
                print("[+] Resuming ffuf scan...")

    # Wait for the process to complete
    process.wait()

if __name__ == "__main__":
    print("Starting ffuf scan...")
    run_ffuf()
    print("ffuf scan completed.")
