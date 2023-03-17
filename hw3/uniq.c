#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE_LENGTH 500

/*
Function take two arguments and return integer value depending on success of function. Success returns 1 and failure returns 0.
input=> int: file descriptor, char: pointer
output=> int: 1 | int: 0
With the two values, the function will call read() on the file descriptor one character at a time until it reads
a new-line character. One we read a new line character, we have stored an entire line from the input file into
the buffer so we return 1. If there are any errors in reading, we return the error and exit with 0
*/
int readLine(int fd, char *buffer)
{
    char *buf1 = buffer;             // assign char pointer value of buffer
    ssize_t ret;                     // stores the return value from read()
    short max = MAX_LINE_LENGTH - 1; // short variable for reading 1 less than MAX_LINE_LENGTH
    ret = read(fd, buf1, 1);         // read 1 character from the fd
    // while the current character in buffer is not new-line char and ret != 0 meaning EOF
    while (*buf1 != '\n' && ret != 0)
    {
        if (ret == -1)
        {
            perror("read"); // error type
            return 0;       // return 0 for error
        }
        // no more space to read from file
        if (max == 0)
        {
            int rVal = write(STDERR_FILENO, "Error: Line length exceeded maximum.\n", strlen("Error: Line length exceeded maximum.\n"));
            // check for error writing error message
            if (rVal == -1)
            {
                perror("write");
                return 0;
            }
            return 0; // return function for error
        }
        max--;                   // decrement max
        buf1++;                  // increment buffer pointer value
        ret = read(fd, buf1, 1); // read the next character from file
    }

    buf1[0] = '\0';

    return 1; // successfully written one line into buffer
}
/*
Function writes a new line into output file
*/
void newLine(int fd)
{
    if (write(fd, "\n", 1) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
}
/*
Function to swap two pointers
*/
void swap(char **buf1, char **buf2)
{
    char *temp = *buf1;
    *buf1 = *buf2;
    *buf2 = temp;
}

// Main function
int main(int argc, char *argv[])
{
    int infile, outfile; // input and output file
    // check how many arguments there are
    switch (argc)
    {
    case 1:
        // read from standard input
        infile = STDIN_FILENO;
        // write to standard output
        outfile = STDOUT_FILENO;
        break;
    case 2:
        if (strcmp(argv[1], "-") == 0)
        {
            // set file innput to standard input
            infile = STDIN_FILENO;
        }
        else
        {
            // set file input to second argument
            infile = open(argv[1], O_RDONLY);
            // check for error
            if (infile == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
        }
        // set outfile to standard output
        outfile = STDOUT_FILENO;
        break;
    case 3:
        if (strcmp(argv[1], "-") == 0)
        {
            // set to standard input
            infile = STDIN_FILENO;
        }
        else
        {
            // set to open()
            infile = open(argv[1], O_RDONLY);
            // check for error
            if (infile == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
        }
        if (strcmp(argv[2], "-") == 0)
        {
            // set to standard output
            outfile = STDOUT_FILENO;
        }
        else
        {
            outfile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            // check for error
            if (outfile == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
        }
        break;
    default:
        // default: more than 4 arguments
        if (write(STDERR_FILENO, "Error: Too many arguments.\n", strlen("Error: Too many arguments.\n") == -1))
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        break;
    }

    // buffer1
    char *buffer1 = malloc(MAX_LINE_LENGTH * sizeof(char));
    char *buffer2 = malloc(MAX_LINE_LENGTH * sizeof(char));

    if (buffer1 == NULL || buffer2 == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int returnValue = readLine(infile, buffer1);
    // check for read error
    if (returnValue == 0)
    {
        perror("readLine() error");
        exit(EXIT_FAILURE);
    }

    // write buffer1 to output file
    int writeRet = write(outfile, buffer1, strlen(buffer1));
    if (writeRet == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    newLine(outfile); // add new line to file
    // read into buffer2
    int retVal2 = readLine(infile, buffer2);
    if (retVal2 == 0)
    {
        perror("readLine() error");
        exit(EXIT_FAILURE);
    }

    // while buffer2 is not empty
    while (strlen(buffer2) > 0)
    {
        // if the two strings are not equal
        if (strcmp(buffer1, buffer2) != 0)
        {
            // write to output
            int wrRetVal = write(outfile, buffer2, strlen(buffer2));
            if (wrRetVal < 0)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
            newLine(outfile); // add new line to file
            // swap pointer values
            swap(&buffer1, &buffer2);
        }
        // read next line into buffer2
        retVal2 = readLine(infile, buffer2);
        if (retVal2 == -1)
        {
            // if equal -1, on last line and set buffer2 to empty character
            strcpy(buffer2, "\0");
        }
        // check if there is an error reading: exceed_buffer_length error or reading
        else if (retVal2 == 0)
        {
            exit(EXIT_FAILURE);
        }
    }
    // free all resources and close file descriptors
    close(infile);
    close(outfile);
    free(buffer1);
    free(buffer2);

    return EXIT_SUCCESS;
}