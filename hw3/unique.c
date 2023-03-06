#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define P_POINTER_SIZE 1000
#define CHAR_POINTER_SIZE 1000

int _numberOfDelim(char *arr, char delim)
{
    int count = 0;
    while (*arr != '\0')
    {
        if (*arr == delim)
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
            // printf("Line 58=>The string to copy: new-line \n");
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
            //printf("Line 70=>The string to copy: %s\n", string);
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
            // printf("Line 85=>The string to copy: %s\n", string);
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
    // file descriptors for input and output
    int in_fd, out_fd;
    // lines has a pointer to some block of memory that that stores [10| ANY NUMBER] of pointers to characters
    char **lines = malloc(P_POINTER_SIZE * (sizeof(char *)));
    // switch statement to check the number of arguments in the command line
    switch (argc)
    {
        // case is 1 argument which is the executatble
    case 1:
        in_fd = STDIN_FILENO;
        out_fd = STDOUT_FILENO;
        break;
        // case 2: there is more than one argument other than the executable
    case 2:
        // if the second argument is "-", then set the input descriptor vairable to the standard input FD
        if (strcmp(argv[1], "-") == 0)
        {
            in_fd = STDIN_FILENO;
        }
        // else set in_fd to the file descriptor returned from the open() command on the first argument
        else
        {
            in_fd = open(argv[1], O_RDONLY, 0644);
            // check for error opening file
            if (in_fd == -1)
            {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
        }
        // since this case only checks for the input, the output fd will always be the standard output fd
        out_fd = STDOUT_FILENO;
        break;
    case 3:
        // if the second argument is "-", then set the input descriptor vairable to the standard input FD
        if (strcmp(argv[1], "-") == 0)
        {
            in_fd = STDIN_FILENO;
        }
        else
        {
            // else set in_fd to the file descriptor returned from the open() command on the first argument
            in_fd = open(argv[1], O_RDONLY, 0644);
            // check for error
            if (in_fd == -1)
            {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
        }
        // check the third argument to see if it is "-", if true set the fd to the standard output fd
        if (strcmp(argv[2], "-") == 0)
        {
            out_fd = STDOUT_FILENO;
        }
        else
        {
            // else set out_fd to the fd returned from open() on the third argument
            out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            // check for error
            if (out_fd == -1)
            {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
        }
        break;
    default:
        // if there are more than 3 arguments
        perror("Too many arguments");
        exit(EXIT_FAILURE);
    }

    // allocate memory buffer on heap to read the file from in_fd
    char *buffer1 = (char *)calloc(CHAR_POINTER_SIZE, sizeof(char)); // read the input file
    // char *buffer2 = (char *)calloc(CHAR_POINTER_SIZE, sizeof(char));

    // check if we successfully allocated memory to buffer1
    if (buffer1 == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // read entire file
    int count = read(in_fd, buffer1, CHAR_POINTER_SIZE * sizeof(char));
    // check if we successfully read the file
    if (count == -1)
    {
        perror("Error reading input");
        exit(EXIT_FAILURE);
    }
    // set the last index of the buffer to null character
    buffer1[count] = '\0';
    // using the _numberOfDelim() function, find the number of strings in buffer1 + 1 because the last string does no have '\n' but '\0'
    int numberOfStrings = _numberOfDelim(buffer1, '\n') + 1;

    //printf("Number of Strings: %d\n", numberOfStrings);

    int num_lines = 0;
    // loop through the numberofString times and extract the string from the buffer
    for (int i = 0; i < numberOfStrings; i++)
    {
        // retrieves the string from the buffer
        char *subStr = _getString(&buffer1);
        // find the length of the retrieved string
        int lenOfSubStr = strlen(subStr);
        // printf("Inside for loop, iteration: %d; String = %s; Length of substring is %d\n", i, subStr, strlen(subStr));
        // for the i-th char pointer in lines, allocate memory to store the substring based on lenOfsubStr
        lines[i] = malloc(lenOfSubStr * (sizeof(char))); // allocate size for the char pointer
        // printf("Allocated memory to pointer\n");
        // copy the substring into the the i-th lines char pointer
        strcpy(lines[i], subStr);
        // increment number of lines
        num_lines++;
        // printf("Value in lines[%d]: %s\n", i, lines[i]);
        // printf("Bottom of for loop\n");
    }
    for (int i = 0; i < numberOfStrings; i++)
    {
        // printf("Calling getString(%d): %s\n", i, lines[i]);
        //  if i is the last string, there is no next string to compare to
        if (i != numberOfStrings - 1)
        {
            // printf("THE SAME: Lines[%d]: %s; Lines[%d]: %s\n", i, lines[i], i+1, lines[i+1]);
            // if the string values are the same and if i and i+1 are '\n' characters
            if (strcmp(lines[i], lines[i + 1]) == 0 && (strcmp(lines[i], "\n") != 0 && strcmp(lines[i + 1], "\n") != 0))
            {
                //printf("THE SAME: Lines[%d]: %s; Lines[%d]: %s\n", i, lines[i], i + 1, lines[i + 1]);
                // since they are the same continue the loop until it find an index i+1 that isn't
                // the same as i
                continue;
            }
            // lines[i] and lines[i+1] are not equal so we can write to the out_fd/output file
            else
            {
                //printf("The last iteration of duplication: lines[%d]: %s\n", i, lines[i]);
                // this i-th iteration is the last index in which the duplicate occurs
                // if lines[i] is and '\n' character
                if (strcmp(lines[i], "\n") != 0)
                {
                    //printf("Lines[%d]: %s is not equal to new-line\n", i, lines[i]);
                    // concat '\n' to it to maintain the empty line in read file
                    char *string = strcat(lines[i], "\n");
                    //printf("String with new-line char conacted into it: %s", string);
                    int writeRes = write(out_fd, string, strlen(string));
                    if (writeRes < 0)
                    {
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // if not a new line character write as is
                    if (write(out_fd, lines[i], strlen(lines[i])) < 0)
                    {
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        else
        {
            // lines[i] is the last string to write
            if (write(out_fd, lines[i], strlen(lines[i])) < 0)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
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