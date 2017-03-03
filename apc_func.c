#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "apc.h"

/* Function to read expression from user */
int read_variable(char *temp_buf1, char *temp_buf2, int *var1_size, int *var2_size, int temp_buf_size, OPERATION_T *operation, char *sign_var1, char* sign_var2, char *sign_result)
{
    int idx = 0;
    char ch;
    char operator = '\0';
    char arthmetic_operations[4];

    /* Prompt user for expression input */
    printf("Enter the expression : ");

    /* Ignore white spaces */
    while ((ch = getchar()) == ' ');

    /* Get sign of first variable */
    if (is_operator(ch))
    {
        switch (ch)
        {
            case '+':
            case '-':
                break;
            case '/':
            case '*':
                printf("ERROR : Invalid variable\n");
                exit(1);

        }
        *sign_var1 = ch;
    }
    else if (isdigit(ch))
    {
        temp_buf1[idx++] = ch;
        *sign_var1 = '+';
    }
    else
    {
        printf("ERROR : Invalid variable\n");
        exit(1);
    }
    
    /* Fetch first variable */
    while (idx < temp_buf_size - 1)
    {
        temp_buf1[idx] = getchar();
        
        if (isdigit(temp_buf1[idx]))
        {
            idx++;
        }
        else
        {
            if (temp_buf1[idx] != ' ')
            {
                if (is_operator(temp_buf1[idx]))
                {
                    operator = temp_buf1[idx];
                }
                else
                {
                    printf("ERROR : Ivalid operation '%c'\n", temp_buf1[idx]);
                    exit(1);
                }
            }
            temp_buf1[idx] = '\0';
            *var1_size = idx;
            break;
        }
    }

    /* Fetch operator */
    if (operator == '\0')
    {
        /* Ignore white spaces */
        while ((ch = getchar()) == ' ');

        if (is_operator(ch))
        {
            operator = ch;
        }
        else
        {
            if (isdigit(ch))
                printf("ERROR : arthmetic operation ??\n");
            else
                printf("ERROR : Ivalid operation '%c'\n", ch);
            exit(1);
        }
    }
    
    idx = 0;
    /* Ignore white spaces */
    while ((ch = getchar()) == ' ');

    if (is_operator(ch))
    {
        switch (ch)
        {
            case '+':
            case '-':
                break;
            case '/':
            case '*':
                printf("ERROR : Invalid variable\n");
                exit(1);
        }

        *sign_var2 = ch;
    }
    else if (isdigit(ch))
    {
        temp_buf2[idx++] = ch;
        *sign_var2 = '+';
    }
    else
    {
        printf("ERROR : Invalid variable\n");
        exit(1);
    }

    /* Fetch second variable */
    while (idx < temp_buf_size - 1)
    {
        temp_buf2[idx] = getchar();
        
        if (isdigit(temp_buf2[idx]))
        {
            idx++;
        }
        else
        {
            temp_buf2[idx] = '\0';
            *var2_size = idx;
            break;
        }
    }
 

    /* Decide operation */
    arthmetic_operations[0] = *sign_var1;
    arthmetic_operations[1] = operator;
    arthmetic_operations[2] = *sign_var2;
    arthmetic_operations[3] = '\0';

    switch(operator)
    {
        case '+':
        case '-':
        if (strcmp(arthmetic_operations, "+++") == 0)
        {
            *operation = ADDITON;
            *sign_result = '+';
        }
        else if (strcmp(arthmetic_operations, "-+-") == 0)
        {
            *operation = ADDITON;
            *sign_result = '-';
        }
        else if (strcmp(arthmetic_operations, "+--") == 0)
        {
            *operation = ADDITON;
            *sign_result = '+';
        }
        else if (strcmp(arthmetic_operations, "--+") == 0)
        {
            *operation = ADDITON;
            *sign_result = '-';
        }
        else
        {
            *operation = SUBTRACTION;
            *sign_result = '\0';
        }
        break;
        case '*':
            *operation = MULTIPLICATION;
            break;
        case '/':
            *operation = DIVISION;
            break;
        case '%':
            *operation = MODULO;
            break;
    }

    /* Error case for empty variables */
    if (!strlen(temp_buf1))
    {
        printf("ERROR : Invalid variable1\n");
        exit(1);
    }
    if (!strlen(temp_buf2))
    {
        printf("ERROR : Invalid variable2\n");
        exit(1);
    }

    return idx;
}

/* Function to pad zeros to variables at the beginning */
void pad_zeros(char *temp_buf1, int *var1_size, char *var1, char *temp_buf2, int *var2_size, char *var2, int *var_size)
{
    int idx;

    /* If length of first variable > second variable */
    if (*var1_size > *var2_size)
    {
        strcpy(var1, temp_buf1);

        /* Pad zeros for second variable */
        for( idx = 0; idx < *var1_size - *var2_size; idx++)
        {
            var2[idx] = '0';
        }
        strcpy(var2 + idx, temp_buf2);
        *var_size = *var2_size = *var1_size;
    }
    /* If length of second variable > first variable */
    else if (*var2_size > *var1_size)
    {
        strcpy(var2, temp_buf2);

        /* Pad zeros for first variable */
        for( idx = 0; idx < *var2_size - *var1_size; idx++)
        {
            var1[idx] = '0';
        }
        strcpy(var1 + idx, temp_buf1);
        *var_size = *var1_size = *var2_size;
    }
    else
    {
        /* If both variables are of same length */
        strcpy(var1, temp_buf1);
        strcpy(var2, temp_buf2);
        *var_size = *var1_size = *var2_size;
    }

    return;
}

/* Function to perform addition */
int addition(char *var1, char *var2, int var_size, char sign_var1, char sign_var2, char sign_result, char *result, OPERATION_T operation)
{
    slice_t *var1_slice_list = NULL;
    slice_t *var2_slice_list = NULL;
    result_t *result_slice_list = NULL;

    slice_variables_for_addition(var1, var2, var_size, &var1_slice_list, &var2_slice_list, &result_slice_list); /************/

    addition_operation_on_slice_list(&var1_slice_list, &var2_slice_list, &result_slice_list);

    copy_result_to_buf(result_slice_list, result);

    if (operation == ADDITON)
    print_result_for_addition(var1_slice_list, var2_slice_list, result_slice_list, sign_var1, sign_var2, sign_result);

    /* Free allocated memory for slice slice lists */
    delete_slice_list(&var1_slice_list);
    delete_slice_list(&var2_slice_list);
    delete_result_list(&result_slice_list);

    return 1;
}

/* Function to slice variables for addition */
int slice_variables_for_addition(char *var1, char *var2, int var_size, slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list)
{
    int idx = var_size - SLICE_SIZE_ADD_SUB;
    char local_var1[MAX_BUF];
    char local_var2[MAX_BUF];

    /* Copy variables to local buffer */
    strcpy(local_var1, var1);
    strcpy(local_var2, var2);

    /* Slice variables in length of 9 digits */
    while (idx >= 0)
    {
            insert_slice_last(var1_slice_list, local_var1 + idx);
            insert_slice_last(var2_slice_list, local_var2 + idx);
            insert_result_slice_last(result_slice_list);
            local_var1[idx] = '\0';
            local_var2[idx] = '\0';
            idx = idx - SLICE_SIZE_ADD_SUB;
    }

    if (local_var1[0] != '\0')
    {
        insert_slice_last(var1_slice_list, local_var1);
        insert_slice_last(var2_slice_list, local_var2);
        insert_result_slice_last(result_slice_list);
    }

    return 0;
}

/* Function to perform addition of sliced variables */
int addition_operation_on_slice_list(slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list)
{
    int idx = 0, swap_idx = 0;
    int carry = 0;
    int no_of_digits, no_of_zeros;
    char ch, buf[10];
    slice_t *var1_slice_list_ptr = *var1_slice_list;
    slice_t *var2_slice_list_ptr = *var2_slice_list;
    result_t *result_slice_list_ptr = *result_slice_list;
    result_t *new_result_slice;

    /* Till end of slice list */
    while (var1_slice_list_ptr != NULL)
    {
        idx = 0, swap_idx = 0;
        /* Add slices of two variables */
        result_slice_list_ptr->result_val = var1_slice_list_ptr->slice_val + var2_slice_list_ptr->slice_val + carry;

        /* Get carry for above operation */
        get_no_of_digits_and_carry_addition(result_slice_list_ptr->result_val, &no_of_digits, &carry);

        /* Get no of zeros if exists */
        no_of_zeros = SLICE_SIZE_ADD_SUB - no_of_digits;

        /* convert result character to integer excluding carry */
        do
        {
            result_slice_list_ptr->result[idx++] = result_slice_list_ptr->result_val % 10 + '0';
            result_slice_list_ptr->result_val /= 10;
        } while (result_slice_list_ptr->result_val > 0);

   
        if (carry)
        {
            idx--;
            result_slice_list_ptr->result[idx] = '\0';
        }
        else
        {
            if (var1_slice_list_ptr->link != NULL)
            {
                while (no_of_zeros > 0)
                {
                    result_slice_list_ptr->result[idx++] = '0';
                    no_of_zeros--;
                }
            }

            result_slice_list_ptr->result[idx] = '\0';
        }

        /* Reverse the result string */
        string_reverse(result_slice_list_ptr->result);

        /* Store integer value of result in slice */
        result_slice_list_ptr->result_val = atoi(result_slice_list_ptr->result);

        /* Move to next slice */
        var1_slice_list_ptr = var1_slice_list_ptr->link;
        var2_slice_list_ptr = var2_slice_list_ptr->link;
        result_slice_list_ptr = result_slice_list_ptr->link;
    }

    /* If carry exists, create a new result slice to store it */
    if (carry)
    {
        new_result_slice = insert_result_slice_last(result_slice_list);
        strcpy(new_result_slice->result, itoa(carry, buf));
        new_result_slice->result_val = carry;
    }

    return 0;
}

/* Function to print result of addition operation */
int print_result_for_addition(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list, char sign_var1, char sign_var2, char sign_result)
{
    /* Print first variable */
    printf("\n\t %c", sign_var1);
    print_slice_list(var1_slice_list);
    puts("");

    /* Print operation */
    if (sign_result == '-')
        printf("'-' ");
    else
    {
        if (sign_var2 == '-')
        printf("'-' ");
        else
        printf("'+' ");
    }

    /* Print second variable */
    printf("\t %c",sign_var2);
    print_slice_list(var2_slice_list);
    puts("");

    /* Print result variable */
    printf("-----------------------------------------------------------------------\n");
    printf("Result : %c",sign_result);
    print_result(result_slice_list);
    puts("\n");

    return 0;
}

/* Function to perform subtraction */
int subtraction(char *var1, char *var2, int var_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation)
{
    slice_t *var1_slice_list = NULL;
    slice_t *var2_slice_list = NULL;
    result_t *result_slice_list = NULL;
    int reverse_var_flag = 0;

    /* Decide sign of result */
    if (subtraction_sign_handler(var1, var2, var_size, sign_var1, sign_var2, sign_result, &reverse_var_flag, result, operation) != 0)
    {
        *sign_result = '+';
        return 0;
    }

    /* Flag is set if variables are reversed */
    if (reverse_var_flag)
    {
        slice_variables_for_subtraction(var2, var1, var_size, &var1_slice_list, &var2_slice_list, &result_slice_list);
    }
    else
    {
        slice_variables_for_subtraction(var1, var2, var_size, &var1_slice_list, &var2_slice_list, &result_slice_list);
    }

    subtraction_opeartion_on_slice_list(var1_slice_list, var2_slice_list, result_slice_list);

    copy_result_to_buf(result_slice_list, result);

    /* Print result for subtraction */
    if (operation == SUBTRACTION)
    print_result_for_subtraction(var1_slice_list, var2_slice_list, result_slice_list, sign_var1, sign_var2, *sign_result, reverse_var_flag);

    /* Delete variable and result list */
    delete_slice_list(&var1_slice_list);
    delete_slice_list(&var2_slice_list);
    delete_result_list(&result_slice_list);

    return 0;
}

/* Function to decide sign of result of subtraction */
int subtraction_sign_handler(char *var1, char *var2, int var_size, char sign_var1, char sign_var2, char *sign_result, int *reverse_var_flag, char *result, OPERATION_T operation)
{
    char op_sign;

    /* If variables are same */
    if (strcmp(var1, var2) == 0)
    {
        strcpy(result, "0");
        *sign_result = '+';
        if (sign_var1 == sign_var2)
            op_sign = '-';
        else
            op_sign = '+';
        if (operation == SUBTRACTION)
        {
            printf("\n\t %c%s\n", sign_var1, var1);
            printf("'%c'", op_sign);
            printf("\t %c%s\n", sign_var2, var2);
            printf("-----------------------------------------------------------------------\n");
            printf("Result = %c%s\n", *sign_result, result);
        }

        return 1;
    }

    switch (sign_var1)
    {
        case '+':
        switch (sign_var2)
        {
            case '+':
                if (max(var1, var2, var_size))
                {
                    *sign_result = sign_var1;
                    *reverse_var_flag = 0;
                }
                else
                {
                    *sign_result = change_sign(sign_var1);
                    *reverse_var_flag = 1;
                }
                break;
            case '-':
                if (max(var1, var2, var_size))
                {
                    *reverse_var_flag = 0;
                    *sign_result = sign_var1;
                }
                else
                {
                    *sign_result = change_sign(sign_var1);
                    *reverse_var_flag = 1;
                }
                break;
            }
            break;

        case '-':
            if (max(var1, var2, var_size))
            {
                *reverse_var_flag = 0;
                *sign_result = sign_var1;
            }
            else
            {
                *sign_result = change_sign(sign_var1);
                *reverse_var_flag = 1;
            }
            break;
    }
    return 0;
}

/* Function to perform slicing of variables for subtraction */
int slice_variables_for_subtraction(char *var1, char *var2, int var_size, slice_t **var1_slice_list, slice_t ** var2_slice_list, result_t **result_slice_list)
{
    int idx = var_size - SLICE_SIZE_ADD_SUB;
    char local_var1[MAX_BUF];
    char local_var2[MAX_BUF];

    strcpy(local_var1, var1);
    strcpy(local_var2, var2);

    /* Till end of list */
    while (idx >= 0)
    {
            /* slice variable in length of 9 digits */
            insert_slice_last(var1_slice_list, local_var1 + idx);
            insert_slice_last(var2_slice_list, local_var2 + idx);
            insert_result_slice_last(result_slice_list);
            local_var1[idx] = '\0';
            local_var2[idx] = '\0';
            idx = idx - SLICE_SIZE_ADD_SUB;
    }

    if (local_var1[0] != '\0')
    {
        insert_slice_last(var1_slice_list, local_var1);
        insert_slice_last(var2_slice_list, local_var2);
        insert_result_slice_last(result_slice_list);
    }

    return 0;
}

/* Function to perform subtraction on sliced list */
int subtraction_opeartion_on_slice_list(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list)
{
    int idx, swap_idx;
    int no_of_digits, no_of_zeros;
    char ch;
    int borrow = 0;
    slice_t *var1_slice_list_ptr = var1_slice_list;
    slice_t *var2_slice_list_ptr = var2_slice_list;
    result_t *result_slice_list_ptr = result_slice_list;

    /* Till end of list */
    while (var1_slice_list_ptr != NULL)
    {
        idx = 0, swap_idx = 0;

        /* Perform subtraction of each slice */
        result_slice_list_ptr->result_val = var1_slice_list_ptr->slice_val - borrow - var2_slice_list_ptr->slice_val;

        /* If borrow is generated */
        if (result_slice_list_ptr->result_val < 0)
        {
            result_slice_list_ptr->result_val += 1000000000;
            borrow = 1;
        }

        /* Get no of digits in result */
        get_no_of_digits(result_slice_list_ptr->result_val, &no_of_digits);

        /* Get no of zeros in result */
        no_of_zeros = SLICE_SIZE_ADD_SUB - no_of_digits;

        /* Convert result value from integer to character */ 
        do
        {
            result_slice_list_ptr->result[idx++] = result_slice_list_ptr->result_val % 10 + '0';
            result_slice_list_ptr->result_val /= 10;
        } while (result_slice_list_ptr->result_val > 0);

        /* Pad zeros */
        if (var1_slice_list_ptr->link != NULL)
        {
            while (no_of_zeros > 0)
            {
                result_slice_list_ptr->result[idx++] = '0';
                no_of_zeros--;
            }
        }

        result_slice_list_ptr->result[idx] = '\0';

        /* Reverse result buffer */
        string_reverse(result_slice_list_ptr->result);
        /* Convert reverse buf to integer and store it in result slice */
        result_slice_list_ptr->result_val = atoi(result_slice_list_ptr->result);

        /* Move to next slice */
        var1_slice_list_ptr = var1_slice_list_ptr->link;
        var2_slice_list_ptr = var2_slice_list_ptr->link;
        result_slice_list_ptr = result_slice_list_ptr->link;
    }

    return 0;
}

/* Function to print result of subtraction operation */
int print_result_for_subtraction(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list, char sign_var1, char sign_var2, char sign_result, int reverse_var_flag)
{

    if (!reverse_var_flag)
    {
        /* Print first variable */
        printf("\n\t %c", sign_var1);
        print_slice_list(var1_slice_list);
        puts("");

        /* Print operation */
            printf("'-' ");

        /* Print second variable */
        printf("\t %c",sign_var2);
        print_slice_list(var2_slice_list);
        puts("");

        /* Print result variable */
        printf("-----------------------------------------------------------------------\n");
        printf("Result : %c",sign_result);
        print_result(result_slice_list);
        puts("\n");
    }
    else
    {
        /* Print first variable */
        printf("\n\t %c", sign_var1);
        print_slice_list(var2_slice_list);
        puts("");

        /* Print operation */
            printf("'-' ");

        /* Print second variable */
        printf("\t %c",sign_var2);
        print_slice_list(var1_slice_list);
        puts("");

        /* Print result variable */
        printf("-----------------------------------------------------------------------\n");
        printf("Result : %c",sign_result);
        print_result(result_slice_list);
        puts("\n");
    }

    return 0;
}

/* Function to perform multiplication operation */
int multiplication(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation)
{
    slice_t *var1_slice_list = NULL;
    slice_t *var2_slice_list = NULL;
    result_t *result_slice_list = NULL;

    slice_variables_for_multiplication(var1, var2, var1_size, var2_size, &var1_slice_list, &var2_slice_list, &result_slice_list); 

    multiplication_operation_on_slice_list(&var1_slice_list, &var2_slice_list, &result_slice_list);

    copy_result_to_buf(result_slice_list, result);

    /* Decide sign of result */
    if (sign_var1 == sign_var2)
        *sign_result = '+';
    else
        *sign_result = '-';

    if (operation == MULTIPLICATION)
    print_result_for_multiplication(var1_slice_list, var2_slice_list, result_slice_list, sign_var1, sign_var2, *sign_result, result);

    /* Free link list created to store variables and result */
    delete_slice_list(&var1_slice_list);
    delete_slice_list(&var2_slice_list);
    delete_result_list(&result_slice_list);

    return 0;
}

/* Function to slice variables for multiplication */
int slice_variables_for_multiplication(char *var1, char *var2, int var1_size, int var2_size, slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list)
{
    int idx = var1_size - SLICE_SIZE_MUL;
    result_t *new_result_slice;
    char local_var1[MAX_BUF];
    char local_var2[MAX_BUF];

    strcpy(local_var1, var1);
    strcpy(local_var2, var2);

    /* Till end of variable */
    while (idx >= 0)
    {
            /* Slice first variable in length of 4 digits */
            insert_slice_last(var1_slice_list, local_var1+ idx);
            new_result_slice = insert_result_slice_last(result_slice_list);
            strcpy(new_result_slice->result, "0000");
            new_result_slice->result_val = 0;

            local_var1[idx] = '\0';
            idx = idx - SLICE_SIZE_MUL;
    }

    if (local_var1[0] != '\0')
    {
        insert_slice_last(var1_slice_list, local_var1);
        new_result_slice = insert_result_slice_last(result_slice_list);
        strcpy(new_result_slice->result, "0000");
        new_result_slice->result_val = 0;
    }

    /* Slice second variable in length of 4 digits */
    idx = var2_size -SLICE_SIZE_MUL;
    while (idx >= 0)
    {
            insert_slice_last(var2_slice_list, local_var2+ idx);

            new_result_slice = insert_result_slice_last(result_slice_list);
            strcpy(new_result_slice->result, "0000");
            new_result_slice->result_val = 0;

            local_var2[idx] = '\0';
            idx = idx - SLICE_SIZE_MUL;
    }

    if (local_var2[0] != '\0')
    {
        insert_slice_last(var2_slice_list, local_var2);

        new_result_slice = insert_result_slice_last(result_slice_list);
        strcpy(new_result_slice->result, "0000");
        new_result_slice->result_val = 0;

    }
    return 0;
}

/* Function to perform multiplication on variable slice list */
int multiplication_operation_on_slice_list(slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list)
{
    int idx;
    int result_slice_count = 0, var2_count = 0;
    result_t *new_slice;
    result_t *inter_result = NULL;
    slice_t *var1_slice_list_ptr = *var1_slice_list;
    slice_t *var2_slice_list_ptr = *var2_slice_list;
    result_t *result_slice_list_ptr = *result_slice_list;
    result_t *inter_result_ptr = inter_result;

    /* Create result slices */
    while (result_slice_list_ptr != NULL)
    {
        result_slice_count++;
        result_slice_list_ptr = result_slice_list_ptr->link;
    }

    /* Initialize all result slices to zero */
    for (idx = 0; idx < result_slice_count; idx++)
    {
        new_slice = insert_result_slice_last(&inter_result);
        strcpy(new_slice->result, "0000");
        new_slice->result_val = 0;
    }

    var2_count = 0;
    var2_slice_list_ptr = *var2_slice_list;
    while (var2_slice_list_ptr != NULL)
    {

        multiplication_operation(var1_slice_list, var2_slice_list, &inter_result, var2_count);

        addition_operation_on_intermediate_result(&inter_result, result_slice_list, result_slice_list);

        /* clear the result slice list */
        inter_result_ptr = inter_result;
        for (idx = 0; idx < result_slice_count; idx++)
        {
            strcpy(inter_result_ptr->result, "0000");
            inter_result_ptr->result_val = 0;
            inter_result_ptr = inter_result_ptr->link;
        }

        var2_count++;
        var2_slice_list_ptr = var2_slice_list_ptr->link;
    }

    return 0;
}

/* Function to perform multiplication on each slice variable */
int multiplication_operation(slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list, int var2_count)
{
    int idx = 0, swap_idx = 0;
    int carry = 0;
    int no_of_digits, no_of_zeros;
    char ch, buf[10];
    slice_t *var1_slice_list_ptr = *var1_slice_list;
    slice_t *var2_slice_list_ptr = *var2_slice_list;
    result_t *result_slice_list_ptr = *result_slice_list;
    result_t *new_result_slice;

    /* Pad slice with zeros */
    for (idx = 0; idx < var2_count; idx++)
    {
        strcpy(result_slice_list_ptr->result, "0000");
        result_slice_list_ptr->result_val = 0;
        result_slice_list_ptr = result_slice_list_ptr->link;
        var2_slice_list_ptr = var2_slice_list_ptr->link;
    }

    /* Till end of list */
    while (var1_slice_list_ptr != NULL)
    {
        idx = 0, swap_idx = 0;
        /* Multiply each each slice */
        result_slice_list_ptr->result_val = (var1_slice_list_ptr->slice_val * var2_slice_list_ptr->slice_val) + carry;

        /* Get no of digits and carry from result */
        get_no_of_digits_and_carry_multiplication(result_slice_list_ptr->result_val, &no_of_digits, &carry);

        /* Get no of zeros in result */
        no_of_zeros = SLICE_SIZE_MUL - no_of_digits;

        /* Convert result from integer to character excluding carry */
        do
        {
            result_slice_list_ptr->result[idx++] = result_slice_list_ptr->result_val % 10 + '0';
            result_slice_list_ptr->result_val /= 10;
        } while (result_slice_list_ptr->result_val > 0);

   
        if (carry)
        {
            while (idx >= 5)
            idx--;
            result_slice_list_ptr->result[idx] = '\0';
        }
        else
        {
            /* pad zeros if required */
            if (var1_slice_list_ptr->link != NULL)
            {
                while (no_of_zeros > 0)
                {
                    result_slice_list_ptr->result[idx++] = '0';
                    no_of_zeros--;
                }
            }

            result_slice_list_ptr->result[idx] = '\0';
        }

        /* reverse result buffer */
        string_reverse(result_slice_list_ptr->result);
        /* convert result from character to intger and store it in result slice */
        result_slice_list_ptr->result_val = atoi(result_slice_list_ptr->result);

        /* Move to next slice */
        var1_slice_list_ptr = var1_slice_list_ptr->link;
        result_slice_list_ptr = result_slice_list_ptr->link;
    }

    /* If carry exists, create a new result slice and insert the carry */
    if (carry)
    {
        result_slice_list_ptr->result_val = carry;
        strcpy(result_slice_list_ptr->result, itoa(result_slice_list_ptr->result_val, buf));
    }

    return 0;
}

/* Function to add intermediate results of multiplication */
int addition_operation_on_intermediate_result(result_t **res1_slice_list, result_t **res2_slice_list, result_t **result_slice_list)
{
    int idx = 0, swap_idx = 0;
    int carry = 0;
    int no_of_digits, no_of_zeros;
    char ch, buf[10];
    result_t *res1_slice_list_ptr = *res1_slice_list;
    result_t* res2_slice_list_ptr = *res2_slice_list;
    result_t *result_slice_list_ptr = *result_slice_list;

    /* Till end of list */
    while (res1_slice_list_ptr != NULL)
    {
        idx = 0, swap_idx = 0;

        /* Add each slices of intermediate results */
        result_slice_list_ptr->result_val = res1_slice_list_ptr->result_val + res2_slice_list_ptr->result_val + carry;

        /* Get no of digits and carry from result */
        get_no_of_digits_and_carry_multiplication(result_slice_list_ptr->result_val, &no_of_digits, &carry);

        /* Get no of zeros in result */
        no_of_zeros = SLICE_SIZE_MUL - no_of_digits;

        /* Convert result from integer to character excluding carry */
        do
        {
            result_slice_list_ptr->result[idx++] = result_slice_list_ptr->result_val % 10 + '0';
            result_slice_list_ptr->result_val /= 10;
        } while (result_slice_list_ptr->result_val > 0);

   
        if (carry)
        {
            while (idx == 5)
            idx--;
            result_slice_list_ptr->result[idx] = '\0';
        }
        else
        {
            if (res1_slice_list_ptr->link != NULL)
            {
                /* Pad zeros if required */
                while (no_of_zeros > 0)
                {
                    result_slice_list_ptr->result[idx++] = '0';
                    no_of_zeros--;
                }
            }

            result_slice_list_ptr->result[idx] = '\0';
        }

        /* reverse result buffer */
        string_reverse(result_slice_list_ptr->result);
        /* convert result from character to intger and store it in result slice */
        result_slice_list_ptr->result_val = atoi(result_slice_list_ptr->result);

        /* Move to next slice */
        res1_slice_list_ptr = res1_slice_list_ptr->link;
        res2_slice_list_ptr = res2_slice_list_ptr->link;
        result_slice_list_ptr = result_slice_list_ptr->link;
    }

    /* If carry exists, create a new result slice and insert the carry */
    if (carry)
    {
        result_slice_list_ptr->result_val += carry;
        strcpy(result_slice_list_ptr->result, itoa(result_slice_list_ptr->result_val, buf));
    }

    return 0;
}

/* Function to print result of multiplication operation */
int print_result_for_multiplication(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list, char sign_var1, char sign_var2, char sign_result, char *result)
{
    /* Print first variable */
    printf("\n\t %c", sign_var1);
    print_slice_list(var1_slice_list);
    puts("");

    /* Print operation */
    printf("'*' ");

    /* Print second variable */
    printf("\t %c",sign_var2);
    print_slice_list(var2_slice_list);
    puts("");

    /* Print result variable */
    printf("-----------------------------------------------------------------------\n");
    printf("Result : %c",sign_result);
    //print_result(result_slice_list);
    printf("%s\n", result);
    puts("\n");

    return 0;
}

/* Function to perform division */
int division(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation)
{
    /* Variable to store local data */
    char local_var1[MAX_BUF];
    char sign_local_var1;
    int local_var1_size;

    /* Variable to store local data */
    char local_var2[MAX_BUF];
    char sign_local_var2;
    int local_var2_size;
    int local_var_size;

    /* Variable to store local result */
    char local_result[LOC_BUF];
    char sign_local_result;

    /* Variable to store quotient */
    char quotient[LOC_BUF];
    char sign_quotient;
    int quotient_size;

    /* Variable to store new quotient */
    char quotient_new[LOC_BUF];
    char sign_quotient_new;
    int quotient_new_size;

    /* Variable to store divisor */
    char divisor[LOC_BUF];
    char sign_divisor;
    int divisor_size;

    /* Variable to numerator */
    char numerator[LOC_BUF];
    char sign_numerator;
    int numerator_size;

    /* Variable to divisor */
    char remainder[LOC_BUF];
    char sign_remainder;
    int remainder_size;

    /* Copy first variable as dividend */
    strcpy(numerator, var1);
    sign_numerator = sign_var1;
    numerator_size = var1_size;

    /* copy second variable as divisor */
    strcpy(divisor, var2);
    sign_divisor = sign_var2;
    divisor_size = var2_size;

    /* Check for zero divison */
    if (var2[0] - '0' == 0)
    {
        printf ("MATH ERROR : ZERO DIVISON\n");
        exit (1);
    }

    /* Decide sign of result */
    if (sign_numerator == sign_divisor)
    {
        sign_divisor = sign_numerator = '+';
        *sign_result = '+';
    }
    else
    {
        sign_divisor = sign_numerator = '+';
        *sign_result = '-';
    }

    /* Get a initial quotient */
    local_result[0] = '\0';
    single_digit_division(numerator, divisor, numerator_size, divisor_size, sign_numerator, sign_divisor, &sign_local_result, local_result);
    copy_result(quotient, &sign_quotient, &quotient_size, local_result, sign_local_result);

    /* set conditions to enter below loop */
    strcpy(remainder, divisor);
    remainder_size = divisor_size;

    /* compare remainder with divisor */
    local_var1_size = remainder_size;
    local_var2_size = divisor_size;
    pad_zeros(remainder, &local_var1_size, local_var1, divisor, &local_var2_size, local_var2, &local_var_size);

    
    /* Check if remainder is greater than the divisor */
     while (max(local_var1, local_var2, local_var_size))
     {
        /* Multiply Quotient and divisor to get a product */
        local_result[0] = '\0';
        multiplication(quotient, divisor, quotient_size, divisor_size, sign_quotient, sign_divisor, &sign_local_result, local_result, operation);

        /* Subratact numerator and product to get remainder*/
        local_var1_size = numerator_size;
        sign_local_var1 = sign_numerator;
        local_var2_size = strlen(local_result);
        sign_local_var2 = sign_local_result;
        pad_zeros(numerator, &local_var1_size, local_var1, local_result, &local_var2_size, local_var2, &local_var_size);
        /* Clear local result buffer */
        local_result[0] = '\0';
        subtraction(local_var1, local_var2, local_var_size, sign_local_var1, sign_local_var2, &sign_local_result, local_result, operation);
        copy_result(remainder, &sign_remainder, &remainder_size, local_result, sign_local_result);

        /* Get a new quotient */
        local_result[0] = '\0';
        single_digit_division(remainder, divisor, remainder_size, divisor_size, sign_remainder, sign_divisor, &sign_local_result, local_result);

        local_var1_size = quotient_size;
        sign_local_var1 = sign_quotient;
        local_var2_size = strlen(local_result);
        sign_local_var2 = sign_local_result;
        pad_zeros(quotient, &local_var1_size, local_var1, local_result, &local_var2_size, local_var2, &local_var_size);
        /* Clear local result buffer */
        local_result[0] = '\0';
        if (sign_local_var1 == sign_local_var2)
        {
            if (sign_local_var1 == '-')
            {
                sign_local_result = '-';
            }
            else
            {
                sign_local_result = '+';
            }
            addition(local_var1, local_var2, local_var_size, sign_local_var1, sign_local_var2, sign_local_result, local_result, operation);
        }
        else
        {
            subtraction(local_var1, local_var2, local_var_size, sign_local_var1, sign_local_var2, &sign_local_result, local_result, operation);
        }
        copy_result(quotient_new, &sign_quotient_new, &quotient_new_size, local_result, sign_local_result);

        /* Take average of old and new quotient */
        local_var1_size = quotient_size;
        sign_local_var1 = sign_quotient;
        local_var2_size = quotient_new_size;
        sign_local_var2 = sign_quotient_new;
        pad_zeros(quotient, &local_var1_size, local_var1, quotient_new, &local_var2_size, local_var2, &local_var_size);
        /* Clear local result buffer */
        local_result[0] = '\0';
        addition(local_var1, local_var2, local_var_size, sign_local_var1, sign_local_var2, sign_local_result, local_result, operation);
        copy_result(local_var1, &sign_local_var1, &local_var1_size, local_result, sign_local_result);

        /* Clear local result buffer */
        local_result[0] = '\0';
        single_digit_division(local_var1, "2", local_var1_size, 1, sign_local_var1, '+', &sign_local_result, local_result);

        copy_result(quotient, &sign_quotient, &quotient_size, local_result, sign_local_result);

        /* compare remainder with divisor */
        local_var1_size = remainder_size;
        local_var2_size = divisor_size;
        pad_zeros(remainder, &local_var1_size, local_var1, divisor, &local_var2_size, local_var2, &local_var_size);
    }

    /* Clear local result buffer */
    local_result[0] = '\0';
    multiplication(quotient, divisor, quotient_size, divisor_size, sign_quotient, sign_divisor, &sign_local_result, local_result, operation);

    /* Get remainder */
    local_var1_size = numerator_size;
    sign_local_var1 = sign_numerator;
    local_var2_size = strlen(local_result);
    sign_local_var2 = sign_local_result;
    pad_zeros(numerator, &local_var1_size, local_var1, local_result, &local_var2_size, local_var2, &local_var_size);

    /* Clear local result buffer */
    local_result[0] = '\0';
    subtraction(local_var1, local_var2, local_var_size, sign_local_var1, sign_local_var2, &sign_local_result, local_result, operation);
    copy_result(remainder, &sign_remainder, &remainder_size, local_result, sign_local_result);

    /* If remainder is negative, increment quotient and add divisor to remainder */
    if (sign_remainder == '-')
    {
        /* Decrement quotient by one */
        local_var1_size = quotient_size;
        sign_local_var1 = sign_quotient;
        local_var2_size = 1;
        sign_local_var2 = '+';
        pad_zeros(quotient, &local_var1_size, local_var1, "1", &local_var2_size, local_var2, &local_var_size);
        /* Clear local result buffer */
        local_result[0] = '\0';
        subtraction(local_var1, local_var2, local_var_size, sign_local_var1, sign_local_var2, &sign_local_result, local_result, operation);
        copy_result(quotient, &sign_quotient, &quotient_size, local_result, sign_local_result);

        /* Add divisor to remainder */
        local_var1_size = divisor_size;
        sign_local_var1 = sign_divisor;
        local_var2_size = remainder_size;
        sign_local_var2 = sign_remainder;
        pad_zeros(divisor, &local_var1_size, local_var1, remainder, &local_var2_size, local_var2, &local_var_size);
        /* Clear local result buffer */
        local_result[0] = '\0';
        subtraction(local_var1, local_var2, local_var_size, sign_local_var1, sign_local_var2, &sign_local_result, local_result, operation);
        copy_result(remainder, &sign_remainder, &remainder_size, local_result, sign_local_result);
    }

    /* Print result */
    sign_quotient = *sign_result;
    sign_remainder = *sign_result;

    if (operation == DIVISION)
    printf("Result = %c%s\n", sign_quotient, quotient);

    if (operation == MODULO)
    printf("Result = %c%s\n", sign_remainder, remainder);

    strcpy(result, quotient);
    return 0;
}

/* Function to perform division for single digit divisor */
int single_digit_division(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result)
{
    int carry = 0;
    int no_of_digits, no_of_zeros;
    int idx = var1_size - SLICE_SIZE_DIV;
    int divisor = var2[0] - '0';
    int iidx, jidx, len;
    char local_var1[MAX_BUF];
    result_t *result_slice_list = NULL;
    result_t *new_result_slice;
    slice_t *var1_slice_list = NULL;

    slice_t *var1_slice_list_ptr = var1_slice_list;
    result_t *result_slice_list_ptr = result_slice_list;

    /* Copy first variable to local buffer */
    strcpy(local_var1, var1);

    /* Till end of variable */
    while (idx >= 0)
    {
            /* Slice variable in length of 8 digits */
            insert_slice_first(&var1_slice_list, local_var1 + idx);
            new_result_slice = insert_result_slice_first(&result_slice_list);
            strcpy(new_result_slice->result, "00000000");
            new_result_slice->result_val = 0;

            local_var1[idx] = '\0';
            idx = idx - SLICE_SIZE_DIV;
    }

    if (local_var1[0] != '\0')
    {
        insert_slice_first(&var1_slice_list, local_var1);
        new_result_slice = insert_result_slice_first(&result_slice_list);
        strcpy(new_result_slice->result, "00000000");
        new_result_slice->result_val = 0;
    }

    var1_slice_list_ptr = var1_slice_list;
    result_slice_list_ptr = result_slice_list;

    /* Till end of variable slice list */
    while (var1_slice_list_ptr != NULL)
    {
        idx = 0;

        /* Divide each slice */
        result_slice_list_ptr->result_val = (var1_slice_list_ptr->slice_val + (carry * 100000000)) / divisor;

        /* Get forward carry */
        carry = (var1_slice_list_ptr->slice_val % divisor);

        /* Get no of digits in result */
        get_no_of_digits(result_slice_list_ptr->result_val, &no_of_digits);

        /* Get no of zeros in result */
        no_of_zeros = SLICE_SIZE_DIV - no_of_digits;

        /* Covert result from integer to character excluding carry */
        do
        {
            result_slice_list_ptr->result[idx++] = result_slice_list_ptr->result_val % 10 + '0';
            result_slice_list_ptr->result_val /= 10;
        } while (result_slice_list_ptr->result_val > 0);

        if (var1_slice_list_ptr->link != NULL)
        {
            /* Pad zeros if required */
            while (no_of_zeros > 0)
            {
                result_slice_list_ptr->result[idx++] = '0';
                no_of_zeros--;
            }
        }
        result_slice_list_ptr->result[idx] = '\0';
        
        /* reverse result buffer */
        string_reverse(result_slice_list_ptr->result);

        /* Covert result from charater to integer and store it in result slice */
        result_slice_list_ptr->result_val = atoi(result_slice_list_ptr->result);

        /* Move to next slice */
        var1_slice_list_ptr = var1_slice_list_ptr->link;
        result_slice_list_ptr = result_slice_list_ptr->link;
    }

    /* Copy result to result buffer from slice list */
    result_slice_list_ptr = result_slice_list;
    result[0] = '\0';
    while (result_slice_list_ptr != NULL)
    {
        strcat(result, result_slice_list_ptr->result);
        result_slice_list_ptr = result_slice_list_ptr->link;
    }

    /* Remove zeros in the begining of result */
    iidx = 0, jidx = 0;
    while(result[iidx] != '\0' && result[iidx] == '0')
    {
        iidx++;
    }

    if (result[iidx] == '\0')
    {
        strcpy(result, "0");
        return 0;
    }

    while ((result[jidx++] = result[iidx++]) != '\0');

    len = strlen(result);

    for (idx = 0; idx < len - (var2_size - 1); idx++);
    result[idx] = '\0';

    if (sign_var1 == sign_var2)
        *sign_result = '+';
    else
        *sign_result = '-';

    return 0;
}

/* Function to perform modulo operation */
int modulo(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation)
{
    /* Call divison function to get the remainder */
    division(var1, var2, var1_size, var2_size, sign_var1, sign_var2, sign_result, result, operation);

    return 0;
}

/* Function to insert variable slice at first */
slice_t *insert_slice_first(slice_t **var_slice_list, char *slice)
{
    slice_t *new;

    if (*var_slice_list == NULL)
    {
        *var_slice_list = (slice_t *) malloc(sizeof(slice_t));
        strcpy((*var_slice_list)->slice, slice);
        (*var_slice_list)->slice_val = atoi((*var_slice_list)->slice);
        (*var_slice_list)->link = NULL;
        return *var_slice_list;
    }
    else
    {
        new = (slice_t *) malloc(sizeof(slice_t));
        strcpy(new->slice, slice);
        new->slice_val = atoi(new->slice);
        new->link = *var_slice_list;
        *var_slice_list = new;
        return new;
    }
}

/* Function to insert variable slice at last */
slice_t *insert_slice_last(slice_t **var_slice_list, char *slice)
{
    slice_t *ptr = *var_slice_list, *new;

    if (*var_slice_list == NULL)
    {
        *var_slice_list = (slice_t *) malloc(sizeof(slice_t));
        strcpy((*var_slice_list)->slice, slice);
        (*var_slice_list)->slice_val = atoi((*var_slice_list)->slice);
        (*var_slice_list)->link = NULL;
        return *var_slice_list;
    }
    else
    {
        while (ptr->link != NULL)
            ptr = ptr->link;

        new = (slice_t *) malloc(sizeof(slice_t));
        strcpy(new->slice, slice);
        new->slice_val = atoi(new->slice);
        new->link = NULL;
        ptr->link = new;
        return new;
    }
}

/* Function to insert result at first */
result_t *insert_result_slice_first(result_t **result_slice_list)
{
    result_t *new;

    if (*result_slice_list == NULL)
    {
        *result_slice_list = (result_t *) malloc(sizeof(result_t));
        (*result_slice_list)->link = NULL;
        return *result_slice_list;
    }
    else
    {
        new = (result_t *) malloc(sizeof(result_t));
        new->link = *result_slice_list;
        *result_slice_list = new;
        return new;
    }
}

/* Function to insert result at last */
result_t *insert_result_slice_last(result_t **result_slice_list)
{
    result_t *ptr = *result_slice_list, *new;

    if (*result_slice_list == NULL)
    {
        *result_slice_list = (result_t *) malloc(sizeof(result_t));
        (*result_slice_list)->link = NULL;
        return *result_slice_list;
    }
    else
    {
        while (ptr->link != NULL)
            ptr = ptr->link;

        new = (result_t *) malloc(sizeof(result_t));
        new->link = NULL;
        ptr->link = new;
        return new;
    }
}

/* Function to print result slice list */
void print_result(result_t *result_slice_list)
{
    if (result_slice_list == NULL)
    {
        return;
    }
    else
    {
        print_result(result_slice_list->link);
#ifdef DEBUG
        printf("%s\n", result_slice_list->result);
#else
        printf("%s", result_slice_list->result);
#endif
    }
    return;
}

/* Function to print variable slice list */
void print_slice_list(slice_t *var_slice_list)
{
    if (var_slice_list == NULL)
    {
        return;
    }
    else
    {
        print_slice_list(var_slice_list->link);
#ifdef DEBUG
        printf("%s\n", var_slice_list->slice);
#else
        printf("%s", var_slice_list->slice);
#endif
    }
    return;
}

/* Function to delete entire variable slice list */
void delete_result_list(result_t **head)
{
	result_t *ptr = *head;
	result_t *next;

	/* Check if list is empty */
	if (*head == NULL)
	{
		return;
	}
	else
	{
		/* Till end of list */
		while (ptr != NULL)
		{
			next = ptr->link;
			free(ptr);
			ptr = next;
		}
		*head = NULL;
	}
}

/* Function to delete entire result slice list */
void delete_slice_list(slice_t **head)
{
	slice_t *ptr = *head;
	slice_t *next;

	/* Check if list is empty */
	if (*head == NULL)
	{
		return;
	}
	else
	{
		/* Till end of list */
		while (ptr != NULL)
		{
			next = ptr->link;
			free(ptr);
			ptr = next;
		}
		*head = NULL;
	}
}

/* Function to get no of digits and carry in result */
int get_no_of_digits_and_carry_addition(int result, int *no_of_digits, int *carry)
{
    char buf[10];
    int idx = 0;
    
    do
    {
        buf[idx++] = result % 10 + '0';
        result /= 10;
    } while (result > 0);

    if (idx == 10)
    {
        *carry = buf[--idx] - '0';
        buf[idx] = '\0';
    }
    else
    {
        *carry = 0;
        buf[idx] = '\0';
    }

    *no_of_digits = strlen(buf);
}

/* Function to get no of digits  nd carry in result */
int get_no_of_digits_and_carry_multiplication(int result, int *no_of_digits, int *carry)
{
    char buf[10];
    int idx = 0, j;
    
    do
    {
        buf[idx++] = result % 10 + '0';
        result /= 10;
    } while (result > 0);

    if (idx > 4)
    {
        *carry = 0;
        for (j = idx; j > 4; j--)
        *carry = (*carry * 10) + (buf[--idx] - '0');
        buf[idx] = '\0';
    }
    else
    {
        *carry = 0;
        buf[idx] = '\0';
    }

    *no_of_digits = strlen(buf);

    return 0;
}

/* Function to get no of digits in an integer */
int get_no_of_digits(int result, int *no_of_digits)
{
    char buf[10];
    int idx = 0;
    
    do
    {
        buf[idx++] = result % 10 + '0';
        result /= 10;
    } while (result > 0);

    buf[idx] = '\0';

    *no_of_digits = strlen(buf);
}

/* Function to check given character is operator */
int is_operator(char ch)
{
    char operators[] = {'+', '-', '*', '/', '%', '\0'};
    int idx;

    for (idx = 0; operators[idx] != '\0'; idx++)
    {
        if (operators[idx] == ch)
            return 1;
    }

    return 0;
}

/* Function to find largest for two variables */
int max(char *var1, char *var2, int var_size)
{
    int idx;
    
    for (idx = 0; idx < var_size; idx++)
    {
        if (var1[idx] > var2[idx])
        {
            return 1;
        }
        if (var1[idx] < var2[idx])
        {
            return 0;
        }
    }
    return 1;
}

/* Function to reverse sign of a variable */
int change_sign(char sign)
{
    switch (sign)
    {
        case '+':
            return '-';
        case '-':
            return '+';
    }
}

/* Function to convert integer to character*/
char *itoa(int n, char s[])
{
    int i, j;
    char sign, temp;

    /* Get sign */
    if (n > 0)
    {
        sign = '\0';
    }
    else
    {
        sign = '-';
        n = -n;
    }

    /* Get each bit from num in reverse order and store it in s[] */
    for (i = 0; n != 0; i++)
    {
        s[i] = n % 10 + '0';
        n = n / 10;
    }
    s[i++] = sign;
    s[i] = '\0';

    /* String reversal */
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }

    return s;
}

/* Function to copy result to buffer from result slice list */
int copy_result_to_buf(result_t *result_slice_list, char *buf)
{
    char temp[10];
    int iidx = 0, jidx = 0;
    result_t *result_slice_list_ptr = result_slice_list;

    while (result_slice_list_ptr != NULL)
    {
        strcpy(temp, result_slice_list_ptr->result);
        string_reverse(temp);
        strcat(buf, temp);
        result_slice_list_ptr = result_slice_list_ptr->link;
    }
    string_reverse(buf);

    while(buf[iidx] != '\0' && buf[iidx] == '0')
    {
        iidx++;
    }
    if (buf[iidx] == '\0')
        return 0;

    while ((buf[jidx++] = buf[iidx++]) != '\0');

    return 0;
}

/* Function to reverse a string */
void string_reverse(char *buf)
{
    int idx, swap_idx;
    char ch;

    for (idx = 0; buf[idx] != '\0'; idx++);
    for (--idx, swap_idx = 0; swap_idx < idx; swap_idx++, idx--)
    {
        ch = buf[idx];
        buf[idx] = buf[swap_idx];
        buf[swap_idx] = ch;
    }
    return;
}

/* Function to copy data from source to target buffers */
int copy_result(char *target_var, char *sign_target, int *target_size, char *result, char sign_result)
{
    strcpy(target_var, result);
    *sign_target = sign_result;
    *target_size = strlen(result);
    return 0;
}
