#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "apc.h"

int main(void)
{
    char *temp_buf1;        /* Temporary buffer            */
    char *temp_buf2;        /* Temporary buffer            */
    char var1[MAX_BUF];     /* First variable              */
    char var2[MAX_BUF];     /* Second variable             */
    char result[MAX_BUF];   /* Result buffer               */
    int var1_size;          /* Length of first variable    */
    int var2_size;          /* Length of second variable   */
    int var_size;           /* Length of padded variables  */
    char sign_var1;         /* sign of first variable      */
    char sign_var2;         /* sign of second variable     */
    char sign_result;       /* sign of result              */
    OPERATION_T operation;  /* Type of arithmetic opeation */


    /* Temporary buffer to take user input */
    temp_buf1 = (char *) malloc(MAX_BUF);
    temp_buf2 = (char *) malloc(MAX_BUF);
    
    /* Read expression from user */
    read_variable(temp_buf1, temp_buf2, &var1_size, &var2_size, MAX_BUF, &operation, &sign_var1, &sign_var2, &sign_result);

    /* Clear result buffer */
    result[0] = '\0';

    /* Switch opeartion based on user input */
    switch (operation)
    {
        case ADDITON:

            /* copy data from temp buffer to variables with zero padding */
            pad_zeros(temp_buf1, &var1_size, var1, temp_buf2, &var2_size, var2, &var_size);
            
            /* Perform addition by invoking function */
            addition(var1, var2, var_size, sign_var1, sign_var2, sign_result, result, operation);
            break;

        case SUBTRACTION:

            /* copy data from temp buffer to variables with zero padding */
            pad_zeros(temp_buf1, &var1_size, var1, temp_buf2, &var2_size, var2, &var_size);

            /* Perform subtraction by invoking function */
            subtraction(var1, var2, var_size, sign_var1, sign_var2, &sign_result, result, operation);
            break;

        case MULTIPLICATION:

            /* copy data from temp buffer to variables */
            strcpy (var1, temp_buf1);
            strcpy (var2, temp_buf2);

            /* Perform multiplication by invoking function */
            multiplication(var1, var2, var1_size, var2_size, sign_var1, sign_var2, &sign_result, result, operation);
            break;

        case DIVISION:

            /* copy data from temp buffer to variables */
            strcpy (var1, temp_buf1);
            strcpy (var2, temp_buf2);

            /* Perform division by invoking function */
            division(var1, var2, var1_size, var2_size, sign_var1, sign_var2, &sign_result ,result, operation);
            break;

        case MODULO:

            /* copy data from temp buffer to variables */
            strcpy (var1, temp_buf1);
            strcpy (var2, temp_buf2);

            /* Perform modulo by invoking function */
            modulo(var1, var2, var1_size, var2_size, sign_var1, sign_var2, &sign_result ,result, operation);
            break;
    }

    /* Free temorary buffers */
    free(temp_buf1);
    free(temp_buf2);

    return 0;
}
