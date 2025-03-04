#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h> // For sleep and usleep

#define MAX_WORD_LENGTH 256
#define MAX_URL_LENGTH 1024

// ANSI color codes
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m" 
#define RESET   "\033[0m"

// Function to replace "FUZZ" in the URL with a given word
void replace_fuzz(const char *url, const char *word, char *output_url) {
    char *fuzz_pos = strstr(url, "FUZZ");
    if (fuzz_pos) {
        size_t prefix_len = fuzz_pos - url;
        size_t suffix_len = strlen(fuzz_pos + 4);

        snprintf(output_url, MAX_URL_LENGTH, "%.*s%s%s", (int)prefix_len, url, word, fuzz_pos + 4);
    } else {
        strncpy(output_url, url, MAX_URL_LENGTH);
    }
}

// Function to perform an HTTP request and retrieve the response code
long perform_request(const char *url) {
    CURL *curl;
    CURLcode res;
    long response_code = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        } else {
            fprintf(stderr, "Request error: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    return response_code;
}

// Function to print colored output in the terminal
void print_colored_output(const char *url, long response_code) {
    const char *color;

    if (response_code >= 200 && response_code < 300) {
        color = GREEN;  // ✅ 2xx -> Green
    } else if (response_code >= 300 && response_code < 400) {
        color = BLUE;   // 🔵 3xx -> Blue
    } else if (response_code >= 400 && response_code < 500) {
        color = YELLOW; // 🟡 4xx -> Yellow
    } else if (response_code >= 500 && response_code < 600) {
        color = RED;    // 🔴 5xx -> Red
    } else {
        color = RESET;  // Unknown status
    }

    printf("%s%s [Status: %ld]%s\n", color, url, response_code, RESET);
}

// Function to save output to a file
void save_to_file(FILE *output_file, const char *url, long response_code) {
    if (output_file) {
        fprintf(output_file, "%s [Status: %ld]\n", url, response_code);
        fflush(output_file); // Ensure immediate write to file
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s -w <wordlist> -u <url> [-r <rate>] [-o <output.txt>]\n", argv[0]);
        return 1;
    }

    const char *wordlist = NULL;
    const char *url_template = NULL;
    const char *output_filename = NULL;
    int rate_limit = 0; // Request rate per second (0 means unlimited)

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            wordlist = argv[++i];
        } else if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            url_template = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            rate_limit = atoi(argv[++i]); // Convert rate to integer
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_filename = argv[++i]; // Get output file name
        }
    }

    if (!wordlist || !url_template) {
        fprintf(stderr, "Error: Missing required arguments!\n");
        fprintf(stderr, "Usage: %s -w <wordlist> -u <url> [-r <rate>] [-o <output.txt>]\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(wordlist, "r");
    if (!file) {
        perror("Error opening wordlist file");
        return 1;
    }

    FILE *output_file = NULL;
    if (output_filename) {
        output_file = fopen(output_filename, "w");
        if (!output_file) {
            perror("Error creating output file");
            fclose(file);
            return 1;
        }
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);

    char word[MAX_WORD_LENGTH];
    char final_url[MAX_URL_LENGTH];

    while (fgets(word, MAX_WORD_LENGTH, file)) {
        word[strcspn(word, "\n")] = 0;

        replace_fuzz(url_template, word, final_url);
        long response_code = perform_request(final_url);

        print_colored_output(final_url, response_code);
        save_to_file(output_file, final_url, response_code);

        // If the server returns a 500 error, wait for 120 seconds
        if (response_code >= 500 && response_code < 600) {
            fprintf(stderr, MAGENTA "⚠️ Server returned a 500 error! Pausing for 120 seconds...\n" RESET);
            sleep(120);

            // Retry after 120 seconds
            response_code = perform_request(final_url);
            print_colored_output(final_url, response_code);
            save_to_file(output_file, final_url, response_code);
        }

        // If response code is 0 (connection issues), wait for 300 seconds
        if (response_code == 0) {
            fprintf(stderr, MAGENTA "❌ Connection error! Pausing for 300 seconds...\n" RESET);
            sleep(300);
        }

        // Apply rate limiting if specified
        if (rate_limit > 0) {
            usleep(1000000 / rate_limit); // Convert rate to microseconds
        }
    }

    fclose(file);
    if (output_file) fclose(output_file);
    curl_global_cleanup();

    return 0;
}

