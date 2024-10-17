#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeSparseFile(FILE *file, long long fileSize, const char *fileName) {
    const int chunkSize = 100 * 1024;  // 100 KB chunk size
    char buffer[chunkSize];

    for (long long offset = 0; offset < fileSize; offset += chunkSize) {
        // Fill the buffer with the filename and offset information
        snprintf(buffer, sizeof(buffer), "%s%09lld", fileName, offset);

        // Seek to the offset and write the buffer
        fseek(file, offset, SEEK_SET);
        fwrite(buffer, 1, strlen(buffer), file);
    }
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

    char filePath[256];

    for (int fileNumber = 1; fileNumber <= maxFiles; ++fileNumber) {
        snprintf(filePath, sizeof(filePath), "%s/%s%04d.bin", baseDirectory, baseName, fileNumber);

        FILE *file = fopen(filePath, "wb");
        if (file == NULL) {
            perror("Error creating file");
            return EXIT_FAILURE;
        }

        writeSparseFile(file, fileSize, filePath);  // Passing the file name for demonstration

        fclose(file);

        printf("File %s created successfully.\n", filePath);
    }

    return EXIT_SUCCESS;
}

