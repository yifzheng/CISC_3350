
// C program to illustrate
// read system Call
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define P_POINTER_SIZE 10
#define CHAR_POINTER_SIZE 100

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
    // if there is something pointing to the memory oter than '\0';
    if (strlen(*arr) != 1)
    {
        if (_isEmptyLine(*arr) == 1)
        {
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
            // increment argument pointer by index + 1 so the starting position of the next _getString() call isn't '\n'
            *arr += (index + 1);
            // return new cstring
            return string;
            // free memory
            free(string);
        }
        else
        {
            char *string = malloc(strlen(*arr) * sizeof(char));
            strncpy(string, *arr, strlen(*arr));
            *arr += (strlen(*arr) - 1);
            return string;
            free(string);
        }
    }
    return NULL;
}

int main()
{
    int fd, sz; // file directory and read file value
    char *buffer1 = (char *)calloc(CHAR_POINTER_SIZE, sizeof(char));
    char **ptr_to_strings = malloc(P_POINTER_SIZE * (sizeof(char *))); // ptr_to_strings has a pointer to some block of memory that that stores [10| ANY NUMBER] of pointers to characters

    fd = open("foo.txt", O_RDWR);
    if (fd < 0)
    {
        perror("r1");
        exit(1);
    }
    // read from fd returned from open() into buffer1 with size of CHAR_POINTER_SIZE
    sz = read(fd, buffer1, CHAR_POINTER_SIZE);

    buffer1[sz] = '\0'; // make the last index of C-string null or '\0'
    close(fd);          // close file directory or free the value on file directory table
    int numberOfStrings = _numberOfDelim(buffer1, '\n') + 1;
    for (int i = 0; i < numberOfStrings; i++)
    {
        // retrieves the string from the buffer
        char *subStr = _getString(&buffer1);
        // find the length of the retrieved string
        int lenOfSubStr = strlen(subStr);
        ptr_to_strings[i] = malloc(lenOfSubStr * (sizeof(char))); // allocate size for the char pointer
        strcpy(ptr_to_strings[i], subStr);
    }

    // printf("\nThose bytes are as follows: \n% s", c);
    int fd2 = open("output.txt", O_RDWR | O_CREAT);
    if (fd2 < 0)
    {
        perror("r1");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < numberOfStrings; i++)
    {
        // printf("Number of strings: %d -> string[%d] = %s; length of string: %d\n", numberOfStrings, i, ptr_to_strings[i], strlen(ptr_to_strings[i]));
        //  if i is the last string, there is no next string to compare to
        if (i != numberOfStrings - 1)
        {
            if (strcmp(ptr_to_strings[i], ptr_to_strings[i + 1]) == 0) // if the string values are the same
            {
                // printf("string[%d] = %s | string[%d] = %s\n", i, ptr_to_strings[i], i + 1, ptr_to_strings[i + 1]);
                continue; // continue the for loop for i+1
            }
            else
            {
                // this ith iteration is the last index in which this string occurs so there is no duplicate next iteration
                if (strcmp(ptr_to_strings[i], "\n")) // if this is a new line char '\n'
                {
                    // concat '\n' to it to maintain the empty line in read file
                    write(fd2, strcat(ptr_to_strings[i], "\n"), strlen(ptr_to_strings[i]) + 1);
                }
                else
                {
                    // else write as is
                    write(fd2, ptr_to_strings[i], strlen(ptr_to_strings[i]));
                }
            }
        }
        else
        {
            write(fd2, ptr_to_strings[i], strlen(ptr_to_strings[i]));
        }
    }
    close(fd2);
    return EXIT_SUCCESS;
}