#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define P_POINTER_SIZE 1000
#define CHAR_POINTER_SIZE 1000

// function to find the number of delimiters in the char pointer
int _numberOfDelim(char *arr, char delim)
{
    // set the intial count
    int count = 0;
    // while the current value of the pointer does not equal null
    while (*arr != '\0')
    {
        // if the current pointer equals the delimiter
        if (*arr == delim)
        {
            // increment count
            count++;
        }
        // increment pointer position
        arr++;
    }
    // return count of delimiters
    return count;
}

// finds the index of the delimiter which is '\n'
int _indexOf(char *arr, char delim)
{
    // stores the index where the delimiter appears
    int instance = 0;
    // while the value of the current position in pointer != new line and length of cstring isn't 1 or '\0'
    while (*arr != delim && (strlen(arr) > 0))
    {
        // increment pointer position and instance value
        arr++;
        instance++;
    }
    // check if there is an instance of the delimiter and return the instance value if value > 0 and length of pointer is > 0
    if (instance > 0 && (strlen(arr) > 0))
    {
        return instance;
    }
    // else return -1
    return -1;
}

// function to check if current pointer of char pointer is a new-line character
int _isEmptyLine(char *arr)
{
    return *arr == '\n' ? 1 : -1;
}

// retriees the cstring from the character pointer until the index of the delimiter
// need to pass character pointer as _getString(&char* POINTER)
char *_getString(char **arr)
{
    // if there is something pointing to the memory oter than '\0';
    if (strlen(*arr) != 1)
    {
        // if current pointer is an empty line
        if (_isEmptyLine(*arr) == 1)
        {
            // increment position of pointer and return a new line character
            *arr += 1; // increment position
            return "\n";
        }
        // find index of the first delimiter
        int index = _indexOf(*arr, '\n');
        // if the delimiter exists in the char pointer
        if (index != -1)
        {
            // allocate the size of the char pointer to the size of index of the delimiter
            char *string = malloc(index * (sizeof(char)));
            // copy string from pointer to new pointer
            strncpy(string, *arr, index);
            //  increment argument pointer by index + 1 so the starting position of the next _getString() call isn't '\n'
            *arr += (index + 1);
            // return new cstring
            return string;
            // free memory
            free(string);
        }
        // there are no more delimiters in the char pointer so you're left with the last string to return
        else
        {
            // allocate memory to the char pointer on heap
            char *string = malloc(strlen(*arr) * sizeof(char));
            // copy string to string pointer
            strncpy(string, *arr, strlen(*arr));
            // increment the char pointer by length of remaining string pointer - 1 because the last value if null-char
            *arr += (strlen(*arr) - 1);
            // return C-string
            return string;
            // free memory
            free(string);
        }
    }
    // char pointer length is 1, only null-char is left so return NULL
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
            // open or create a read only file
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
            // open or create a read only file
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
            // open or create a write only file
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

    int num_lines = 0;
    // loop through the numberofString times and extract the string from the buffer
    for (int i = 0; i < numberOfStrings; i++)
    {
        // retrieves the string from the buffer
        char *subStr = _getString(&buffer1);
        // find the length of the retrieved string
        int lenOfSubStr = strlen(subStr);
        // for the i-th char pointer in lines, allocate memory to store the substring based on lenOfsubStr
        lines[i] = malloc(lenOfSubStr * (sizeof(char))); // allocate size for the char pointer
        // copy the substring into the the i-th lines char pointer
        strcpy(lines[i], subStr);
        // increment number of lines
        num_lines++;
    }
    for (int i = 0; i < numberOfStrings; i++)
    {
        //  if i is the last string, there is no next string to compare to
        if (i != numberOfStrings - 1)
        {
            // if the string values are the same and if i and i+1 are '\n' characters
            // we want to preserve the empty line characters or structure from original file
            if (strcmp(lines[i], lines[i + 1]) == 0 && (strcmp(lines[i], "\n") != 0 && strcmp(lines[i + 1], "\n") != 0))
            {
                //  since they are the same continue the loop until it find an index i+1 that isn't the same as i
                continue;
            }
            // lines[i] and lines[i+1] are not equal so we can write to the out_fd/output file
            else
            {
                //  this i-th iteration is the last index in which the duplicate occurs if lines[i] is and '\n' character
                if (strcmp(lines[i], "\n") != 0)
                {
                    //  concat '\n' to it to maintain the empty line in read file
                    char *string = strcat(lines[i], "\n");
                    // write into output file
                    int writeRes = write(out_fd, string, strlen(string));
                    // check if there is error writing to a file
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