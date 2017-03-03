#ifndef APC_H
#define APC_h

/* Maximum length of buffers */
#define MAX_BUF 500
#define LOC_BUF 500

/* Slice length of variable for
   * addition
   * subtraction 
*/
#define SLICE_SIZE_ADD_SUB 9

/* Slice lenght of variable for
   * multiplication 
*/
#define SLICE_SIZE_MUL 4

/* Slice lenght of variable for
   * division 
*/
#define SLICE_SIZE_DIV 8

/* Maximum value of signed int */
#define MAX_INT 2147483647

/* Typedef arithmetic operation */
typedef enum 
{
    ADDITON,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    MODULO,
} OPERATION_T;

/* Typedef result slice list */
typedef struct slice
{
    char slice[10];
    int slice_val;
    struct slice *link;
} slice_t;

/* Typedef variable slice list */
typedef struct result
{
    char result[10];
    int result_val;
    struct result *link;
} result_t;

/* ************************* UTILITY FUNCTIONS *****************************/

/* Function to read expression from user */
int read_variable(char *temp_buf1, char *temp_buf2, int *var1_size, int *var2_size, int temp_buf_size, OPERATION_T *operation, char *sign_var1, char* sign_var2, char *sign_result);

/* Function to check given character is operator */
int is_operator(char ch);

/* Function to pad zeros to variables at the beginning */
void pad_zeros(char *temp_buf1, int *var1_size, char *var1, char *temp_buf2, int *var2_size, char *var2, int *var_size);

/* Function to insert variable slice at first */
slice_t *insert_slice_first(slice_t **var_slice_list, char *slice);

/* Function to insert variable slice at last */
slice_t *insert_slice_last(slice_t **var_slice_list, char *slice);

/* Function to insert result at first */
result_t *insert_result_slice_last(result_t **result_slice_list);

/* Function to insert result at last */
result_t *insert_result_slice_first(result_t **result_slice_list);

/* Function to print variable slice list */
void print_result(result_t *result_slice_list);

/* Function to print result slice list */
void print_slice_list(slice_t *var_slice_list);

/* Function to copy result to buffer from result slice list */
int copy_result_to_buf(result_t *result_slice_list, char *buf);

/* Function to delete entire variable slice list */
void delete_slice_list(slice_t **head);

/* Function to delete entire result slice list */
void delete_result_list(result_t **head);

/* ************************* FUNCTIONS USED FOR ADDITION *****************************/

/* Function to perform addition */
int addition(char *var1, char *var2, int var_size, char sign_var1, char sign_var2, char sign_result, char *result, OPERATION_T operation);

/* Function to slice variables for addition */
int slice_variables_for_addition(char *var1, char *var2, int var_size, slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list);

/* Function to perform addition of sliced variables */
int addition_operation_on_slice_list(slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list);

/* Function to convert integer to character*/
char *itoa(int n, char s[]);

/* Function to get no of digits and carry in result */
int get_no_of_digits_and_carry_addition(int result, int *no_of_digits, int *carry);

/* Function to print result of addition operation */
int print_result_for_addition(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list, char sign_var1, char sign_var2, char sign_result);


/* Function to perform subtraction */
int subtraction(char *var1, char *var2, int var_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation);

/* Function to decide sign of result of subtraction */
int subtraction_sign_handler(char *var1, char *var2, int var_size, char sign_var1, char sign_var2, char *sign_result, int *reverse_var_flag, char *result, OPERATION_T operation);

/* Function to perform slicing of variables for subtraction */
int slice_variables_for_subtraction(char *var1, char *var2, int var_size, slice_t **var1_slice_list, slice_t ** var2_slice_list, result_t **result_slice_list);

/* Function to perform subtraction on sliced list */
int subtraction_opeartion_on_slice_list(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list);

/* Function to find largest for two variables */
int max(char *var1, char *var2, int var_size);

/* Function to reverse sign of a variable */
int change_sign(char sign);

/* Function to get no of digits in an integer */
int get_no_of_digits(int result, int *no_of_digits);


/* ************************* FUNCTIONS USED FOR SUBTRACTION *****************************/


/* Function to print result of subtraction operation */
int print_result_for_subtraction(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list, char sign_var1, char sign_var2, char sign_result, int reverse_var_flag);


/* ************************* FUNCTIONS USED MULTIPLICATION *****************************/


/* Function to perform multiplication operation */
int multiplication(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation);

/* Function to slice variables for multiplication */
int slice_variables_for_multiplication(char *var1, char *var2, int var1_size, int var2_size, slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list);

/* Function to perform multiplication on variable slice list */
int multiplication_operation_on_slice_list(slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list);

/* Function to get no of digits  nd carry in result */
int get_no_of_digits_and_carry_multiplication(int result, int *no_of_digits, int *carry);

/* Function to perform multiplication on each slice variable */
int multiplication_operation(slice_t **var1_slice_list, slice_t **var2_slice_list, result_t **result_slice_list, int var2_count);

/* Function to add intermediate results of multiplication */
int addition_operation_on_intermediate_result(result_t **res1_slice_list, result_t **res2_slice_list, result_t **result_slice_list);

/* Function to print result of multiplication operation */
int print_result_for_multiplication(slice_t *var1_slice_list, slice_t *var2_slice_list, result_t *result_slice_list, char sign_var1, char sign_var2, char sign_result, char *result);

/* Function to reverse a string */
void string_reverse(char *buf);


/* ************************* FUNCTIONS USED DIVISION *****************************/


/* Function to perform division */
int division(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation);

/* Function to perform division for single digit divisor */
int single_digit_division(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result);

/* Function to copy data from source to target buffers */
int copy_result(char *target_var, char *sign_target, int *target_size, char *result, char sign_result);

/* ************************* FUNCTIONS USED MODULO *****************************/


/* Function to perform modulo operation */
int modulo(char *var1, char *var2, int var1_size, int var2_size, char sign_var1, char sign_var2, char *sign_result, char *result, OPERATION_T operation);
#endif
