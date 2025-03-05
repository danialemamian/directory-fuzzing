import requests
import argparse

# Function to send a request and get the response status and length
def get_response(url):
    try:
        response = requests.get(url, timeout=5)  # Set timeout to 5 seconds
        status_code = response.status_code
        response_length = len(response.text)

        return status_code, response_length
    except requests.RequestException:
        return None, None  # Ignore errors

# Function to process URLs
def process_urls(input_file, temp_output):
    url_responses = {}

    # Read URLs from input file
    with open(input_file, 'r') as file:
        urls = [line.strip() for line in file.readlines()]

    valid_urls = []  # List to store valid URLs

    # Filter out URLs with status code 500 or 0
    for url in urls:
        if "[Status: 500]" in url or "[Status: 0]" in url:
            continue  # Skip these URLs

        # Send request only for valid URLs
        status_code, response_length = get_response(url)

        if status_code and response_length and 200 <= status_code < 500:
            print(f"{url} [New Status: {status_code}, Length: {response_length}]")
            valid_urls.append(f"{url} [Length: {response_length}]")

            # Save in dictionary based on response length
            if response_length not in url_responses:
                url_responses[response_length] = [url]
            else:
                url_responses[response_length].append(url)

    # Save valid URLs to temporary file
    with open(temp_output, 'w') as file:
        for url in valid_urls:
            file.write(url + '\n')

    print(f"\n✅ Valid URLs are saved in {temp_output}.")

    return temp_output

# Function to filter out URLs with the same response length
def filter_final_urls(temp_output, final_output):
    url_responses = {}

    # Read data from temporary file
    with open(temp_output, 'r') as file:
        urls = [line.strip() for line in file.readlines()]

    # Group URLs by response length
    for url in urls:
        parts = url.split("[Length: ")
        if len(parts) < 2:
            continue  # Skip invalid lines

        url_part = parts[0].strip()
        length = int(parts[1].replace("]", "").strip())

        if length not in url_responses:
            url_responses[length] = [url_part]
        else:
            url_responses[length].append(url_part)

    # Remove URLs with duplicate response lengths
    final_urls = []
    for length, urls in url_responses.items():
        if len(urls) == 1:  # Only keep unique URLs
            final_urls.append(urls[0])

    # Save the final URLs to the output file
    with open(final_output, 'w') as file:
        for url in final_urls:
            file.write(url + '\n')

    print(f"\n✅ Final URLs are saved in {final_output}.")

# Main function
def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser(description='Process URLs from an input file.')
    parser.add_argument('-i', '--input_file', required=True, help='The input file containing the list of URLs to request')

    args = parser.parse_args()

    input_file = args.input_file  # Get the input file from the command line argument
    temp_output = 'valid_urls.txt'  # Temporary file to store valid URLs
    final_output = 'good_urls.txt'  # Final output file after filtering

    temp_file = process_urls(input_file, temp_output)
    filter_final_urls(temp_file, final_output)

if __name__ == "__main__":
    main()
