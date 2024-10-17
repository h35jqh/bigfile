#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createExpectedContent(char *expectedContent, const char *filePath, long long offset) {
    snprintf(expectedContent, 256, "%s%09lld", filePath, offset);
}

int verifyFile(const char *filePath, long long fileSize) {
    char expectedContent[256];

    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        perror("Error opening file for verification");
        return 0;  // Verification failed
    }

    // Verify the content of the file
    for (long long offset = 0; offset < fileSize; offset += 100 * 1024) {
        createExpectedContent(expectedContent, filePath, offset);

        // Seek to the offset and read the buffer
        fseek(file, offset, SEEK_SET);

        char buffer[256];
        size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file);
        buffer[bytesRead] = '\0';  // Null-terminate the buffer

        if (strcmp(buffer, expectedContent) != 0) {
            printf("Verification failed for file %s at offset %lld\n", filePath, offset);
            printf("Expected Buffer: %s\n", expectedContent);
            printf("Actual Buffer  : %s\n", buffer);

            fclose(file);
            return 0;  // Verification failed
        }
    }

    fclose(file);
    printf("Verification successful for file %s\n", filePath);
    return 1;  // Verification successful
}

int verifyFiles(const char *baseDirectory, const char *baseName, long long fileSize, int maxFiles) {
    char filePath[256];
    int overallResult = 1;  // Initialize to success

    for (int fileNumber = 1; fileNumber <= maxFiles; ++fileNumber) {
        snprintf(filePath, sizeof(filePath), "%s/%s%04d.bin", baseDirectory, baseName, fileNumber);

        if (!verifyFile(filePath, fileSize)) {
            overallResult = 0;  // Set to failure if any file fails verification
        }
    }

    return overallResult;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <base_directory> <file_size> <base_name> <max_files>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *baseDirectory = argv[1];
    const long long fileSize = atoll(argv[2]);
    const char *baseName = argv[3];
    const int maxFiles = atoi(argv[4]);

    if (verifyFiles(baseDirectory, baseName, fileSize, maxFiles)) {
        printf("All files are self-consistent.\n");
        return EXIT_SUCCESS;
    } else {
        printf("Verification failed for one or more files.\n");
        return EXIT_FAILURE;
    }
}

