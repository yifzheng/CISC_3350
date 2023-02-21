#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printf("No file name was provided\n");
        return EXIT_FAILURE;
    }
    else
    {
        for (int i = 1; i < argc; i++) // name of files start at index 1 in argv
        {
            FILE *my_file = fopen(argv[i], "r"); // opening file on ith index in argv
            if (my_file == NULL) // if file does not exist
            {
                printf("File: \"%s\" does not exist\n", argv[i]);
                return EXIT_FAILURE;
            }
            else
            {
                char *my_string = malloc(100 * sizeof(char)); // allocate memory to cstring on heap
                while (fgets(my_string, 100, my_file))
                {
                    printf("%s", my_string);
                }
                free(my_string); // free resources
            }
            fclose(my_file); // close file
        }
    }

    return EXIT_SUCCESS;
}