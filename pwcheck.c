/*

IZP - projekt 1 

The autor's Ekaterina Krupenko, xkrupe00

*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PASSWORDS 4000000 // variable for all_passwords string (used for passwords statistic)
#define MAX_LEN 2000000       // variable for password operations 

int verif_args(int argc, char *argv[])                                   //argument validation function
{
    char stats[] = "--stats";
    if(argc != 4 && argc != 3)                                           //condition for arguments parameter and level and for parameter level and statistic
    {
        fprintf(stderr, "Incorrect number of arguments\n");
        return EXIT_FAILURE;
    }
    if(argc == 4)                                                        //condition for checking the correct statistic command
    {
        for(int j = 0; argv[3][j] != '\0'; j++)
        {
            if(argv[3][j] != stats[j])
            {
                fprintf(stderr, "Incorrect argument of statistic\n");
                return EXIT_FAILURE; 
            } 
        }
    }
    if(argv[1][1] != '\0')                                               //level condition (level can be a single char)
    {
        fprintf(stderr, "Incorrect type or number of level\n");
        return EXIT_FAILURE; 
    }
    for(int i = 0; argv[2][i] != '\0'; i++)                              //param condition (parameter can only be a number)
    {
        if(!(argv[2][i] >= '0' && argv[2][i] <= '9'))
        {
            fprintf(stderr, "Incorrect type of parametr\n");
            return EXIT_FAILURE;  
        }
    }
    if(!(argv[1][0] >= '1' && argv[1][0] <= '4'))                        //level condition (level can only be a number from 1 to 4)
    {
        fprintf(stderr, "Incorrect number of level\n");
        return EXIT_FAILURE; 
    }
    
    return EXIT_SUCCESS;
}

void lvl_one(char str[MAX_LEN])
{
    int flag = 0; 
    for(int i = 0; str[i] != '\0'; i++)                                  //string must contain at least one character from the lowercase 
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            flag = 1;
        }
    }
    for(int j = 0; str[j] != '\0'; j++)                                  //string must contain at least one character from the uppercase
    {  
        if(str[j] >= 'A' && str[j] <= 'Z' && flag == 1)
        {
            flag = 2;
        }
    }
    if(flag != 2) str[0] = '\0';
}

void lvl_two(char str[MAX_LEN], int param)
{
    int flag1 = 0, flag2 = 0;
    int ver = param;
    if(ver > 4) ver = 4;
    lvl_one(str);                                                        //before checking the password for the second level we check the conditions first level
    ver -= 2;                                                            //subtract two values of the parameter, the first level already has lower and upper cases
    for(int i = 0; str[i] != '\n'; i++)
    {
        if((str[i] >= '0' && str[i] <= '9'))                             //check to find numbers in a string
        {                           
            flag1 = 1;
        }
        if(!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9'))) //checking for special chars in a string
        {
            flag2 = 1;
        }
    }
    ver = ver - flag1 - flag2;                                           //parameter condition check

    if(ver > 0)
    {
        str[0] = '\0';
    }
}
void lvl_three(char str[MAX_LEN], int param)
{
    lvl_two(str, param);                                                 //before checking the password for the third level we check the conditions from the first and second levels
    int flag = 0;
    for(int i = 0; str[i] != '\0'; i++)
    {
        for(int j = 0; str[j] != '\0'; j++)
        {
            if(str[i] == str[j])                                         //matching to string
            {
                flag++;
            }
            else 
            {
                flag = 0;
            }
            if(flag == param)                                                
            {
                str[0] = '\0';
                break;
            }
        }
    }
}

void lvl_four(char str[MAX_LEN], int param)
{
    int flags = 0;

    for(int j = param, g = 0; str[j] != '\0'; j++)                       //checking for the same substrings
    {
        if(g == j) j++;
        if(str[j + 1] == '\0')
        {
            g++;
            j = g + 1;
        }
        if(str[g] == str[j])
        {
            flags++;
            if(flags == param)
            {
                str[0] = '\0';
                break;
            }
            g++;
        }
        else
        {
            flags = 0;
        }
    }
}

void statistic(char str[MAX_PASSWORDS], int min_pass)
{
    int count = 0;
    bool appears;
    int rows = 0;
    double average_len;
    int i;

    for(i = 0; str[i] != '\0'; i++)                                       //checking for different chars in a string
    {
        appears = true;
        for(int j = 0; j < i; j++)
        {
            if(str[j] == str[i]) 
            {
                appears = false;
                break;
            }
        }
        if(appears) count++;

        if(str[i] == '\n') 
        {
            rows ++;
        }    
    }

    average_len = (i - rows * 1.0) / (rows); 
    
    printf("Statistika:\nRuznych znaku: %d\nMininmalni delka: %d\nPrumerna delka: %.1lf\n", count - 1, min_pass, average_len);   // count - 1 = count - '\n'
}

void level(char str[MAX_LEN], int param, int lvl)
{
    switch(lvl)
    {
        case 1 :
            lvl_one(str);
            break;
        case 2 :
            lvl_two(str, param);
            break;
        case 3 :
            lvl_three(str, param);
            break;
        case 4 :
            lvl_three(str, param);
            lvl_four(str, param);
            break;
        default :
            break;
    }

    printf("%s", str);
}

int main(int argc, char *argv[])
{
    char password[MAX_LEN];
    char symbol = '\0';
    int len = 0;

    char all_passwords[MAX_PASSWORDS];
    int all_pass_len = 0;

    int min_pass = 0;

    int lvl = atoi(argv[1]);
    int param = atoi(argv[2]);

    if(verif_args(argc, argv) == 1)                                          //program termination if arguments validation returns 1 (EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    
    while(symbol != EOF)
    {
        symbol = getchar();                                                  //writing the chars from stdin

        if(symbol == EOF) break; 
        else 
        {
            password[len] = symbol;                                          //writing the password
            all_passwords[all_pass_len] = symbol;                            //writing the all passwords

            if(symbol == '\n') 
            {
                password[len + 1] = '\0';                                    //string with password

                level(password, param, lvl);                                 //program execution with level and parameter variables 
 
                if(min_pass == 0) min_pass = len;                            //finding the shortest password for statistic
                else
                {
                    if(min_pass >= len) min_pass = len;
                }

                len = 0;
            }
            else len++;
            all_pass_len++;
        }
    }
    if(argc == 4)                                                            
    {
        statistic(all_passwords, min_pass);
    }

    return EXIT_SUCCESS;
}