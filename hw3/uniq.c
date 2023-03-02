#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define P_POINTER 20
#define CHAR_POINTER 800

int main(int argc, char *argv[])
{
    int fd_input;
    int fd_output;
    printf("#of args %d\n", argc);

    switch (argc)
    {
    case 1:
        fd_input = 0;
        fd_output = 1;
        break;
    case 2:
        if (strcmp(argv[1], "-") != 0)
        {
            printf("Argument 2: %s\n", argv[1]);
            fd_input = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0664);
            if (fd_input < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            printf("argument 2 is no \"-\" fd_input: %d\n", fd_input);
            fd_output = 1;
        }
        else
        {
            printf("Argument 2 is \"-\"\n");
            fd_input = 0;
            fd_output = 1;
        }
        break;
    case 3:
        printf("Inside case 3\n");
        for (int i = 1; i < 3; i++)
        {
            switch (i)
            {
            case 1:
                if (strcmp(argv[1], "-") != 0)
                {
                    fd_input = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0664);
                    if (fd_input < 0)
                    {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    printf("Argument 2 is not \"-\" fd_input: %d\n", fd_input);
                }
                else
                {
                    fd_input = 0;
                    printf("Argument 2 is \"-\" fd_output: %d\n", fd_output);
                }
                break;
            case 2:
                if (strcmp(argv[2], "-") != 0)
                {
        
                    fd_output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
                    if (fd_output < 0)
                    {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    printf("Argument 3 is not \"-\" fd_output: %d\n", fd_output);
                }
                else
                {
                    fd_output = 1;
                    printf("Argument 3 is\"-\"fd_output: %d\n", fd_output);
                }
                break;
            default:
                break;
            }
        }
        break;
    default:
        perror("Uniq cannot accept more than 3 arguments");
        break;
    }

    return EXIT_SUCCESS;
}