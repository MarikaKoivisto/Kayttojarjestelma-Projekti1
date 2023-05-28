// kerrattu argc ja argv: http://crasseux.com/books/ctutorial/argc-and-argv.html
// kerrattu strcmp: https://www.geeksforgeeks.org/strcmp-in-c/
// kerrattu strpcy: https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
//stdin, stdout and stderr: https://stackoverflow.com/questions/3385201/confused-about-stdin-stdout-and-stderr


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Error handling part
    // Check the number of command-line arguments
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: reverse <input> [<output>]\n");
        exit(1);
    }

    // Open the input file for reading
    FILE *iFile = fopen(argv[1], "r");
    if (iFile == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        exit(1);
    }

    // Error handling if the input and output are the same
    if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "Error: Input and output file must differ\n");
        fclose(iFile);
        exit(1);
    }

    // Open the output file for writing 
    FILE *oFile = NULL;
    if (argc == 3) {
        oFile = fopen(argv[2], "w");
        // error handling in case there is no such file
        if (oFile == NULL) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", argv[2]);
            fclose(iFile);
            exit(1);
        }
    } else {
        // Use stdout as the output if no output file is provided
        oFile = stdout;
    }
    //For reverse
    // set pointer
    char **lines = NULL;
    int l_Count = 0;
    char currentLine[1024];
    while (fgets(currentLine, sizeof(currentLine), iFile) != NULL) {

        /*// Test malloc by allocating more memory than available
        size_t largeSize = 10000000000000; // Attempt to allocate maximum possible size
        char *testMalloc = malloc(largeSize);
        if (testMalloc == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            fclose(iFile);
            fclose(oFile);
            return 1;
        }
        free(testMalloc);*/

        // Get last character of from the line
        currentLine[strcspn(currentLine, "\n")] = '\0';

        // Allocate memory for the line dynamically
        char *line = malloc((strlen(currentLine) + 1) * sizeof(char));
        // Error handling for dynamic memory allocation
        if (line == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            fclose(iFile);
            fclose(oFile);
            exit(1);
        }
        strcpy(line, currentLine);

        // Increase the size of the lines array
        char **temp = realloc(lines, (l_Count + 1) * sizeof(char *));
        // Error handling for dynamic memory allocation
        if (temp == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            fclose(iFile);
            fclose(oFile);
            exit(1);
        }
        lines = temp;

        // Store the line in the lines array
        lines[l_Count] = line;
        l_Count++;
    }

    // Print the lines in reverse order to the output file or stdout
    for (int i = l_Count - 1; i >= 0; i--) {
        fprintf(oFile, "%s\n", lines[i]);
    }

    // Close the files
    fclose(iFile);
    if (oFile != stdout) {
        fclose(oFile);
    }

    // Free the dynamically allocated memory
    for (int i = 0; i < l_Count; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}