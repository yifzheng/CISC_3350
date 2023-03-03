#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define P_POINTER_SIZE 10
#define CHAR_POINTER_SIZE 500

int _numberOfDelim(char *arr, char delim)
{
    int count = 0;
    while (*arr != '\0')
    {
        if (*arr == '\n')
        {
            count++;
        }
        arr++;
    }
    return count;
}

// finds the index of the delimiter which is '\n'
int _indexOf(char *arr, char delim)
{

    int instance = 0;
    // while the value of the current position in pointer != new line and length of cstring isn't 1 or '\0'
    while (*arr != delim && (strlen(arr) > 0))
    {
        arr++;
        instance++;
    }
    if (instance > 0 && (strlen(arr) > 0))
    {
        return instance;
    }
    return -1;
}

int _isEmptyLine(char *arr)
{
    return *arr == '\n' ? 1 : -1;
}

// retriees the cstring from the character pointer until the index of the delimiter
// need to pass character pointer as _getString(&char* POINTER)
char *_getString(char **arr)
{
    // printf("Initial length of string: %d\n", strlen(*arr));
    // if there is something pointing to the memory oter than '\0';
    if (strlen(*arr) != 1)
    {
        if (_isEmptyLine(*arr) == 1)
        {
            *arr += 1; // increment position
            printf("Line 58=>The string to copy: new-line \n");
            return "\n";
        }
        // find index of the first delimiter
        int index = _indexOf(*arr, '\n');
        // printf("Index after delimiter search: %d\n", index);
        // if the delimiter exists in the char pointer
        if (index != -1)
        {
            // allocate the size of the char pointer to the size of index of the delimiter
            char *string = malloc(index * (sizeof(char)));
            // copy string from pointer to new pointer
            strncpy(string, *arr, index);
            printf("Line 70=>The string to copy: %s\n", string);
            // increment argument pointer by index + 1 so the starting position of the next _getString() call isn't '\n'
            *arr += (index + 1);
            // printf("String length after copy: %d\n", strlen(*arr));
            // return new cstring
            return string;
            // free memory
            free(string);
        }
        else
        {
            // printf("Final get String, index was -1, length of string: %d\n", strlen(*arr));
            char *string = malloc(strlen(*arr) * sizeof(char));
            // printf("Allocated last string\n");
            strncpy(string, *arr, strlen(*arr));
           /*  for (int i = 0; i < strlen(string); i++)
            {
                printf("%c\n", string[i]);
            } */
            printf("Line 85=>The string to copy: %s\n", string);
            // printf("After string copied: %s\n", string);
            *arr += (strlen(*arr) - 1);
            // printf("String length after copy: %d\n", strlen(*arr));
            return string;
            free(string);
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{

    int in_fd, out_fd;
    // lines has a pointer to some block of memory that that stores [10| ANY NUMBER] of pointers to characters
    char **lines = malloc(P_POINTER_SIZE * (sizeof(char *)));

    switch (argc)
    {
    case 1:
        in_fd = STDIN_FILENO;
        out_fd = STDOUT_FILENO;
        break;
    case 2:
        if (strcmp(argv[1], "-") == 0)
        {
            in_fd = STDIN_FILENO;
        }
        else
        {
            in_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (in_fd == -1)
            {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
        }
        out_fd = STDOUT_FILENO;
        break;
    case 3:
        if (strcmp(argv[1], "-") == 0)
        {
            in_fd = STDIN_FILENO;
        }
        else
        {
            in_fd = open(argv[1], O_RDONLY);
            if (in_fd == -1)
            {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
        }
        if (strcmp(argv[2], "-") == 0)
        {
            out_fd = STDOUT_FILENO;
        }
        else
        {
            out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd == -1)
            {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
        }
        break;
    default:
        perror("Too many arguments");
        exit(EXIT_FAILURE);
    }

    char *buffer1 = (char *)calloc(CHAR_POINTER_SIZE, sizeof(char)); // read the input file
    // char *buffer2 = (char *)calloc(CHAR_POINTER_SIZE, sizeof(char));

    if (buffer1 == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    // read entire file
    int count = read(in_fd, buffer1, CHAR_POINTER_SIZE * sizeof(char));
    if (count == -1)
    {
        perror("Error reading input");
        exit(EXIT_FAILURE);
    }
    buffer1[count] = '\0';
    int numberOfStrings = _numberOfDelim(buffer1, '\n') + 1;
    printf("Number of Strings: %d\n", numberOfStrings);

    /* for (int i =0; i < numberOfStrings; i++)
    {
        printf("Calling getString(%d): %s\n", i, _getString(&buffer1));
    }*/
    int num_lines = 0; 
    for (int i = 0; i < numberOfStrings; i++)
    {
        char *subStr = _getString(&buffer1);
        int lenOfSubStr = strlen(subStr);
        // printf("Inside for loop, iteration: %d; String = %s; Length of substring is %d\n", i, subStr, strlen(subStr));
        lines[i] = malloc(lenOfSubStr * (sizeof(char))); // allocate size for the char pointer
        // printf("Allocated memory to pointer\n");
        strcpy(lines[i], subStr);
        num_lines++;
        // printf("Value in lines[%d]: %s\n", i, lines[i]);
        // printf("Bottom of for loop\n");
    }
    for (int i =0; i < numberOfStrings; i++)
    {
        printf("Calling getString(%d): %s\n", i, lines[i]);
    }

    // Clean up
    free(buffer1);
    for (int i = 0; i < num_lines; i++)
    {
        free(lines[i]);
    }
    free(lines);
    close(in_fd);
    close(out_fd);

    return EXIT_SUCCESS;
}