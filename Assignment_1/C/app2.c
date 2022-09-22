#include <stdio.h>
#include <stdlib.h>

/**
 * The expected file name of the data file.
 * If no argument is given, the program will search for 
 * this name in the working directory.
 */
#define DATA_FILE_NAME "Data.csv"

/**
 * A structure that emulates a matrix of strings.
 * Each cell is of a fixed width and any string shorter
 * than the max width is trailed with nulls.
 * 
 * The column_count and row_count members help to 
 * utilize the inter_padded_string as matrix.
 * 
 * All the members are pointer so they can be allocated in
 * heap memory.
 * 
 * IMPORTANT: A string that uses the maximum width will not 
 * have a termination null.
*/
typedef struct _string_mat {

    char *inter_padded_strings;
    unsigned long *cell_size;
    unsigned long *column_count;
    unsigned long *row_count;
} string_mat;

/**
 * A structure that houses the information of a person.
 * 
 * All the members are pointers so they can be allocated in
 * heap memory.
*/
typedef struct _Person {

    char *id;
    char *name;
    unsigned int *age;
    unsigned int *weight;
} Person;

/**
 * A helper function to handle the allocation of the unsigned int type.
 * 
 * Can be used to allocate an unsigned int in heap or to create an array
 * of unsigned int.
 * 
 * As of now, if the memory is not sufficient, the pointer will be lost.
 * This behaviour will be changed.
 * 
 * args:
 *  - size: the the number of unsigned int's to allocate.
 * 
 * return:
 *  - unsigned int pointer to the location in heap.
 */
unsigned int *allocate_unsigned_int(unsigned int size) {

    unsigned int *return_unsigned_int = malloc(sizeof(unsigned int) * size);
    if (return_unsigned_int == NULL) {
        printf("Allocation fail [0]: returning NULL.");
    }
    return return_unsigned_int;
}

/**
 * A helper function to handle the allocation of the float type.
 * 
 * Can be used to allocate a float in heap or to create an array
 * of float.
 * 
 * As of now, if the memory is not sufficient, the pointer will be lost.
 * This behaviour will be changed.
 * 
 * args:
 *  - size: the the number of floats to allocate.
 * 
 * return:
 *  - float pointer to the location in heap.
 */
float *allocate_float(unsigned int size) {

    float *return_float = malloc(sizeof(float) * size);
    if (return_float == NULL) {
        printf("Allocation fail [0]: returning NULL.");
    }
    return return_float;
}

/**
 * A helper function to handle the allocation of a string.
 * Can be used to allocate a string in heap or to create an array
 * of float.
 * 
 * If the memory is not sufficient, an null character will be returned.
 * 
 * args:
 *  - size: the the number of chars to allocate.
 * 
 * return:
 *  - char pointer to the location in heap.
 */
char *allocate_string(unsigned int size) {

    char *return_string = malloc(sizeof(char) * (size + 1));
    if (return_string == NULL) {
        printf("Allocation fail [1]: returning empty string.");
        return "";
    }
    return_string[size] = '\0';
    return return_string;
}

/**
 * Reallocates a string to a new size.
 * The new size can be less than or more than the old size.
 * 
 * If the memory is not sufficient, the old string is returned.
 * 
 * args:
 *  - target_string: the string pointer to be reallocated.
 *  - new_size: the number of characters in the new string.
 *              must include the termination char.
 * return:
 *  - char pointer to the location in heap.
 */
char *reallocate_string(
    char *target_string,
    unsigned int new_size) {
    char *return_string = realloc(target_string, new_size);
    if (return_string == NULL) {
        printf("Allocation fail [2]: returning original string.");
        return target_string;
    }
    return return_string;
}

/**
 * Given a string and its length, any extra null characters will be deallocated.
 * 
 * args:
 *  - target_string: the string to be modified.
 *  - length: the original size of the string.
 * 
 * return:
 *  - a pointer to the new reallocated string.
 */
char *string_truncate_trailing_nulls(
    char *target_string,
    unsigned int length) {

    char *return_string = target_string;
    for (unsigned int index = 0; index < length; ++index) {
        if (return_string[index] == '\0') {
            if (length == (index + 1)) {
                return return_string;
            }
            return_string =
                reallocate_string(return_string, length - (index + 1));
        }
    }
    return return_string;
}

/**
 * Given a string determine its length.
 * 
 * args:
 *  - target_string: the string to be examined.
 * 
 * return:
 *  - the character count to and including the nearest null char.
 */
unsigned int string_length(char *target_string) {

    unsigned int length = 0;
    char current_char;
    do {
        current_char = target_string[length++];
    } while (current_char != '\0');
    return length - 1;
}

/**
 * Given a target string, a reference string, a character count, and a starting index,
 * copy `count` times characters from starting index of reference string to target string.
 * 
 * args:
 *  - target_string: the string to be copied into.
 *  - reference_string: the string from which to copy.
 *  - count: the number of characters to copy.
 *  - starting_index: the position in reference_string from which copying begins.
 * 
 * return:
 *  - the same pointer to target_string
 */
char *string_mid_index_copy(
    char *target_string,
    char *reference_string,
    unsigned int count,
    unsigned int starting_index) {

    for (unsigned int index = 0; index < count; ++index) {
        target_string[index] = reference_string[index + starting_index];
    }
    return target_string;
}

/**
 * Given a target string, a reference string, and their length, copy contents of
 * reference_string into target_string.
 * 
 * IMPORTANT: both strings should have the same length as the copying will not 
 * stop even if a null char is encountered.
 * 
 * args:
 *  - target_string: the string to be copied into.
 *  - reference_string: the string from which to copy.
 *  - count: the number of characters to copy.
 *  - starting_index: the position in reference_string from which copying begins.
 * 
 * return:
 *  - the same pointer to target_string
 */
char *string_simple_copy(
    char *target_string,
    char *reference_string,
    unsigned int length) {
    for (int i = 0; i < length; ++i) {
        target_string[i] = reference_string[i];
    }
    return target_string;
}

/**
 * Given a string matrix, a row and a column indicies, find the absolute order of the string
 * referenced by the row and column indicies.
 * 
 * An element with row index of i and column index of j in a table of width n will have 
 * an absolute index of I = i * n + j.
 * 
 * Accounting for cell size c: I = (i * n + j) * c.
 * 
 * args:
 *  - table: the table. To get the width n and cell size c.
 *  - row: the row index i.
 *  - column: the column index j.
 * 
 * return:
 *  - the absolute index I.
 */
unsigned int string_mat_get_absolute_index(
    string_mat table,
    unsigned int row,
    unsigned int column) {
    unsigned int return_unsigned_int =
        (row * table.cell_size[0] * table.column_count[0]) +
        (table.cell_size[0] * column);
    return return_unsigned_int;
}

/**
 * Given a string matrix, a row and a column indicies, find the string that resides in
 * the cell denoted by the row and column.
 * 
 * 
 * args:
 *  - table: the table containing the string.
 *  - row: the row index of the string.
 *  - column: the column index of the string.
 * 
 * return:
 *  - the string with indicies (row, column).
 */
char *string_mat_cartesian_index(
    string_mat table,
    unsigned int row,
    unsigned int column) {
    char *return_string = allocate_string(table.cell_size[0]);
    unsigned int absolute_index =
        string_mat_get_absolute_index(table, row, column);
    return_string =
        string_mid_index_copy(
            return_string,
            table.inter_padded_strings, table.cell_size[0], absolute_index);
    return_string =
        string_truncate_trailing_nulls(return_string, table.cell_size[0]);
    return return_string;
}

/**
 * Given a string that contains comma separated values, build a string matrix containing 
 * the data.
 * 
 * The function reads the string 2 times. 
 * First, to find the number of rows, columns, and 
 * largest cell size. 
 * Second, to build the string and pad all the values will null chars as to have the size of 
 * the largest value.
 * 
 * 
 * args:
 *  - raw_string: the string with comma separated values.
 * 
 * return:
 *  - a string matrix with the data.
 */
string_mat build_table(char *raw_string) {
    string_mat table;

    unsigned int comma_count = 0;
    unsigned int current_comma_count = 0;

    unsigned int largest_cell = 0;
    unsigned int current_largest_cell = 0;

    unsigned int row_count = 0;

    unsigned long raw_string_index = 0;
    char current_char = raw_string[raw_string_index++];
    char last_char = '\n';

    char while_exit_flag = 0;

    while (current_char != '\0') {
        switch (current_char) {
            case ',':
                ++current_comma_count;
                if (current_largest_cell > largest_cell) {
                    largest_cell = current_largest_cell;
                }
                current_largest_cell = 0;
                last_char = ',';
                break;

            case '\n':
                if (last_char == '\n') {
                    continue;
                }
                if (current_largest_cell > largest_cell) {
                    largest_cell = current_largest_cell;
                }
                current_largest_cell = 0;
                ++row_count;
                if (current_comma_count > comma_count) {
                    comma_count = current_comma_count;
                }
                current_comma_count = 0;
                last_char = '\n';
                break;

            case -1:
                if (last_char == '\n') {
                    --row_count;
                }

                break;

            default:
                ++current_largest_cell;
                last_char = 1;

                break;
        }
        current_char = raw_string[raw_string_index++];
    }

    raw_string_index = 0;

    table.inter_padded_strings =
        calloc(largest_cell * row_count * (comma_count + 1),
               sizeof(char) * largest_cell * row_count * (comma_count + 1));
    table.column_count = malloc(sizeof(int));
    table.row_count = malloc(sizeof(int));
    table.cell_size = malloc(sizeof(int));

    table.cell_size[0] = largest_cell;
    table.column_count[0] = comma_count + 1;
    table.row_count[0] = row_count;

    unsigned int cells_left = largest_cell;
    unsigned int index = 0;
    while (1) {
        if (while_exit_flag) {
            break;
        }
        last_char = raw_string[raw_string_index++];
        switch (last_char) {
            case ',':
                for (; cells_left > 0; --cells_left) {
                    table.inter_padded_strings[index++] = '\0';
                }
                cells_left = largest_cell;
                break;

            case '\n':
                for (; cells_left > 0; --cells_left) {
                    table.inter_padded_strings[index++] = '\0';
                }
                cells_left = largest_cell;
                break;

            case '\0':
                while_exit_flag = 1;
                break;

            default:
                table.inter_padded_strings[index++] = last_char;
                --cells_left;
                break;
        }
    }
    return table;
}

/**
 * Given a string matrix, validate that it has at least 2 rows and that it has at 
 * `column_count` number of columns.
 * 
 * 
 * args:
 *  - table: the table to be validated.
 *  - column_count: the number of columns that should be in the table.
 * 
 * return:
 *  - returns 1 if the table passes validation and 0 if it does not.
 */
char validate_table(
    string_mat table,
    unsigned int column_count) {
    if (table.row_count[0] > 2 && table.column_count[0] == column_count) {
        return 1;
    }
    return 0;
}

/**
 * Given a count of Persons, allocate `count` number of Person structs.
 * 
 * As of now, if the memory is not sufficient, the pointer will be lost.
 * This behaviour will be changed.
 * 
 * args:
 *  - count: the number of Person structs to allocate memory for.
 * 
 * return:
 *  - a pointer to the beginning of the Person array.
 */
Person *allocate_person(unsigned int count) {
    Person *return_person = malloc(sizeof(Person) * count);
    if (return_person == NULL) {
        printf("Allocation fail [3]: returning NULL.");
    }
    return return_person;
}

/**
 * Given a string matrix, and a relevant row in the table, use the data in 
 * table to make a new Person struct.
 * 
 * NOTE: to deallocate a Person struct create using this function, use 
 * the deallocate_person function.
 * 
 * 
 * args:
 *  - table: the table containing the data for the Person.
 *  - row: the relevant row in the table.
 * 
 * return:
 *  - a Person struct with memory allocated for it and data filled in.
 */
Person string_mat_to_person(
    string_mat table,
    unsigned int row) {
    Person return_person;

    return_person.id = allocate_string(table.cell_size[0] + 1);
    return_person.name = allocate_string(table.cell_size[0] + 1);
    return_person.age = allocate_unsigned_int(1);
    return_person.weight = allocate_unsigned_int(1);

    return_person.id = string_simple_copy(
        return_person.id, string_mat_cartesian_index(table, row, 0),
        table.cell_size[0]);

    return_person.name = string_simple_copy(
        return_person.name, string_mat_cartesian_index(table, row, 1),
        table.cell_size[0]);

    char *temp_age = allocate_string(table.cell_size[0]);
    temp_age = string_simple_copy(
        temp_age,
        string_mat_cartesian_index(table, row, 2),
        table.cell_size[0]);

    char *temp_weight = allocate_string(table.cell_size[0]);
    temp_weight = string_simple_copy(
        temp_weight, string_mat_cartesian_index(table, row, 3),
        table.cell_size[0]);

    sscanf(temp_weight, "%u", return_person.weight);

    sscanf(temp_age, "%u", return_person.age);

    return return_person;
}

/**
 * Given a table and a count, allocate memory for `count` Persons and 
 * fill in their data from `table`.
 * 
 * 
 * args:
 *  - count: the number of Person structs to accommodate in the array.
 * 
 * return:
 *  - a pointer for an array of Person structs.
 */
Person *build_model(string_mat table, unsigned int count) {
    Person *people = allocate_person(count);

    for (unsigned int i = 0; i < count; ++i) {
        people[i] = string_mat_to_person(table, i + 1);
    }
    return people;
}

/**
 * Given a count, an number of new_lines, 
 * and a string, print the string `count` times and then 
 * print a new line `new_line` times.
 * 
 * args:
 *  - count: number of times to print string.
 *  - new_lines: number of new lines to print after 
 *               the last print of string.
 *  - string: the string to be printed.
 * 
*/
void print_times(
    unsigned int count,
    unsigned int new_lines,
    char *string) {
    for (unsigned int i = 0; i < count; ++i) {
        printf(string);
    }
    for (unsigned int i = 0; i < new_lines; ++i) {
        printf("\n");
    }
}

/**
 * Given an array of floats and its length, find their sum.
 * 
 * 
 * args:
 *  - data_set: the array to sum.
 *  - length: the length of the array.
 * 
 * return:
 *  - the sum of the array as a float.
 */
float sum_float(float *data_set, unsigned int length) {
    unsigned int aggregate_value = data_set[0];
    for (unsigned int i = 1; i < length; ++i) {
        aggregate_value += data_set[i];
    }
    return aggregate_value;
}

/**
 * Given an array of unsigned ints and its length, find their sum.
 * 
 * args:
 *  - data_set: the array to sum.
 *  - length: the length of the array.
 * 
 * return:
 *  - the sum of the array as an unsigned int
 */
unsigned int sum_unsigned_int(
    unsigned int *data_set,
    unsigned int length) {
    unsigned int aggregate_value = data_set[0];
    for (unsigned int i = 1; i < length; ++i) {
        aggregate_value += data_set[i];
    }
    return aggregate_value;
}

/**
 * Given a table, print its contents to the console.
 * The padding will be one plus the length of the longest string 
 * in any respective column. 
 * 
 * args:
 *  - table: the table to print.
 */
void print_table(string_mat table) {
    unsigned int current_largest_cell;
    unsigned int largest_cell = 0;
    char *temp_string;
    unsigned int *padding_of_each_column = allocate_unsigned_int(table.column_count[0]);

    for (unsigned int column = 0; column < table.column_count[0]; ++column) {
        for (unsigned int row = 0; row < table.row_count[0]; ++row) {
            temp_string = string_mat_cartesian_index(table, row, column);
            temp_string = string_truncate_trailing_nulls(temp_string, table.cell_size[0]);
            current_largest_cell = string_length(temp_string);
            free(temp_string);

            if (current_largest_cell > largest_cell) {
                largest_cell = current_largest_cell;
                current_largest_cell = 0;
            };
        }
        padding_of_each_column[column] = largest_cell + 1;
        largest_cell = 0;
        current_largest_cell = 0;
    }
    for (unsigned int row = 0; row < table.row_count[0]; ++row) {
        if (row == 0 || row == 1) {
            print_times(sum_unsigned_int(padding_of_each_column, table.column_count[0]) + table.column_count[0] + 1, 1, "-");
        }
        for (unsigned int column = 0; column < table.column_count[0]; ++column) {
            printf("|%-*s", padding_of_each_column[column], string_mat_cartesian_index(table, row, column));
        };
        printf("|\n");
    }
    print_times(sum_unsigned_int(padding_of_each_column, table.column_count[0]) + table.column_count[0] + 1, 2, "-");
    free(padding_of_each_column);
}

/**
 * Given a Person, format and print its information. 
 * 
 * args:
 *  - target_person: the person to print information of.
 */
void print_person(Person target_person) {
    printf(
        "Person(\tid=%s,\n\tname=%s,\n\tage=%u,\n\tweight=%u\n)\n",
        target_person.id, target_person.name, target_person.age[0],
        target_person.weight[0]);
}

/**
 * Given a string matrix, deallocate memory for all of its members.
 * 
 * args:
 *  - table: the string matrix to deallocate members of.
 */
void deallocate_string_mat(string_mat table) {
    free(table.cell_size);
    free(table.column_count);
    free(table.row_count);
    free(table.inter_padded_strings);
}

/**
 * Given a string Person struct, deallocate memory for all of its members.
 * 
 * args:
 *  - target_person: the Person struct to deallocate members of.
 */
void deallocate_person(Person target_person) {
    free(target_person.age);
    free(target_person.id);
    free(target_person.name);
    free(target_person.weight);
}

/**
 * Given an file stream, allocate and return a string that contain it contents.
 * 
 * args:
 *  - input_file: the file steam to dump contents of.
 * 
 * return:
 *  - a string with the contents of the file stream.
 */
char *get_string_from_file_stream(FILE *input_file) {
    char *return_string;
    char current_char;
    unsigned int index = 0;

    fseek(input_file, 0, SEEK_END);
    return_string = malloc(sizeof(char) * ftell(input_file));
    fseek(input_file, 0, SEEK_SET);

    current_char = getc(input_file);
    while (current_char != EOF) {
        return_string[index++] = current_char;
        current_char = getc(input_file);
    }
    return return_string;
}

/**
 * Given an array of floats and its length, find the minumum value.
 * 
 * 
 * args:
 *  - data_set: the array to find the minimum of.
 *  - length: the length of the array.
 * 
 * return:
 *  - the minimum value of the array as a float.
 */
float min(float *data_set, unsigned int length) {
    float best_value = data_set[0];
    for (unsigned int i = 1; i < length; ++i) {
        if (best_value > data_set[i]) {
            best_value = data_set[i];
        }
    }
    return best_value;
}

/**
 * Given an array of floats and its length, find the maximum value.
 * 
 * 
 * args:
 *  - data_set: the array to find the maximum of.
 *  - length: the length of the array.
 * 
 * return:
 *  - the maximum value of the array as a float.
 */
float max(float *data_set, unsigned int length) {
    float best_value = data_set[0];
    for (unsigned int i = 1; i < length; ++i) {
        if (best_value < data_set[i]) {
            best_value = data_set[i];
        }
    }
    return best_value;
}

/**
 * Given an array of floats and its length, find the average value.
 * 
 * 
 * args:
 *  - data_set: the array to find the average of.
 *  - length: the length of the array.
 * 
 * return:
 *  - the average value of the array as a float.
 */
float average(float *data_set, unsigned int length) {
    return (sum_float(data_set, length) * 1.0f) / length;
}

/**
 * Given two strings, find if they are the same.
 * 
 * NOTE: the comparison stops only when the first null char is encountered.
 * 
 * args:
 *  - first_string
 *  - second_string
 * 
 * return:
 *  - returns 1 if the string are equivalent and 0 if they are not.
 */
char string_compare(char *first_string, char *second_string) {
    unsigned int index = 0;
    char current_char_1 = first_string[index];
    char current_char_2 = second_string[index++];

    while (1) {
        if (current_char_1 == current_char_2) {
            if (current_char_1 == '\0') {
                return 1;
            } 
            current_char_1 = first_string[index];
            current_char_2 = second_string[index++];
        } else {
            return 0;
        }
    }
}

/**
 * Given a table, a column index, and a search_string, search in all the cells
 * of the column in the table for the search_string.
 * 
 * 
 * args:
 *  - table: the table containing the column.
 *  - column: the column to be searched.
 *  - search_string: the string to be searched for.
 * 
 * return:
 *  - returns the row index of the string if found and -1 if not.
 */
unsigned int search_column(string_mat table,
    unsigned int column,
    char *search_string) {
    for (unsigned int row = 0; row < table.row_count[0]; ++row) {
        if (string_compare(search_string, string_mat_cartesian_index(table, row, column))) {
            return row;
        }
    }
    return -1;
}

/**
 * Given a stirng, its length and a target_character and a replacement, search
 * the length of the string for target_character and replace its first instance
 * with replacement.
 * 
 * 
 * args:
 *  - target_string: the string to modify.
 *  - target_char: the char to swap.
 *  - replacement: the char to swap with.
 *  - length: the length of the string.
 * 
 * return:
 *  - the same pointer to target_string.
 */
char *char_replace(char *target_string,
    char target_char,
    char replacement,
    unsigned int length) {
    for (unsigned int index = 0; index < length; ++index) {
        if (target_string[index] == target_char) {
            target_string[index] = replacement;
        }
    }
    return target_string;
}


/**
 * main() houses all user-interaction elements and all output elements 
 * aside from error messages.
 *
 *  It will:
 *      1- find the data file.
 *      2- print the raw string from the data file.
 *      3- build a table.
 *      4- convert the table to list of Persons.
 *      5- compute the minimum, maximum and average age.
 *      6- print the ages to the console.
 *      7- wait for user input to examine other options.
 * args:
 *  - the path to the data file.
 */

int main(
    int argc,
    char *argv[]) {



/* ------ Finding the path to the data file ------ */

    FILE *input_file;
    char file_path[261];
    if (argc > 1) {
        input_file = fopen(argv[1], "r");
        if (input_file) {
            goto after_file_load;
        }
    }
    input_file = fopen(DATA_FILE_NAME, "r");
    if (input_file) {
        goto after_file_load;
    }

    printf(
        "Enter path to the CSV file (must be less than 260 characters):\n> ");
    fgets(file_path, 260, stdin);
    char_replace(file_path, '\n', '\0', 260);
    printf("\n\n");

    input_file = fopen(file_path, "r");
    if (!input_file) {
        printf("No such file found. Please enter a valid path to the file.");
        fclose(input_file);
        return 1;
    }

after_file_load:



/* ------ dumping the file into a string and printing it ------ */

    char *raw_string = get_string_from_file_stream(input_file);
    fclose(input_file);

    printf("%s\n", raw_string);
    print_times(50, 2, "-");


/* ------ building the table and validating it ------ */

    string_mat table = build_table(raw_string);
    free(raw_string);

    if (!validate_table(table, 4)) {
        printf("The CSV file is corrupt.\n");
        return 1;
    }



/* ------ allocating space for the model and building it ------ */

    unsigned int people_count = table.row_count[0] - 1;
    Person *people = allocate_person(people_count);
    people = build_model(table, people_count);



/* ------ extracting, calculating and printing the 
                                    values of members from the model ------ */

    unsigned int calc_attr_unsigned_int;
    float calc_attr_float;
    float *all_attributes = malloc(sizeof(float) * people_count);

        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].age[0] * 1.0f;
        }
        calc_attr_float = average(all_attributes, people_count);
        printf("The average age is %0.2f\n", calc_attr_float);

        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].age[0] * 1.0f;
        }
        calc_attr_unsigned_int = min(all_attributes, people_count);
        printf("The minimum age is %d\n", calc_attr_unsigned_int);

        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].age[0] * 1.0f;
        }
        calc_attr_unsigned_int = max(all_attributes, people_count);
        printf("The maximum age is %d\n", calc_attr_unsigned_int);



/* ------ prompting user for input and selecting one of the options based on it ------ */

    char user_input[20];
    printf("Type \"table\" to show data");
    printf("\nor \"average age\" or \"average weight\" for those averages");
    printf("\nor \"min age\" or \"min weight\" for their minimum");
    printf("\nor \"max age\" or \"min weight\" for their maximum");
    printf("\nor \"model\" to print all person structs");
    printf("\nor the id of a person to print their struct (19 characters maximum)");
    printf("\nor \"exit\" to quit the program.");
    printf("\n\nmode> ");

    fgets(user_input, 20, stdin);
    char_replace(user_input, '\n', '\0', 20);
    printf("\n");

    if (string_compare(user_input, "exit")) {
        printf("\n\n");
        goto after_mode_execution;
    }
    else if (string_compare(user_input, "table")) {
        print_table(table);
        goto after_mode_execution;
    } else if (string_compare(user_input, "model")) {
        for (unsigned int i = 0; i < people_count; ++i) {
            print_person(people[i]);
        }
        goto after_mode_execution;
    } else if (string_compare(user_input, "average age")) {
        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].age[0] * 1.0f;
        }
        calc_attr_float = average(all_attributes, people_count);
        printf("The average age is %0.2f\n", calc_attr_float);
        print_times(50, 2, "-");
        goto after_mode_execution;

    } else if (string_compare(user_input, "average weight")) {
        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].weight[0] * 1.0f;
        }
        calc_attr_float = average(all_attributes, people_count);
        printf("The average weight is %0.2f\n", calc_attr_float);
        print_times(50, 2, "-");
        goto after_mode_execution;

    } else if (string_compare(user_input, "min age")) {
    for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].age[0] * 1.0f;
        }
        calc_attr_unsigned_int = min(all_attributes, people_count);
        printf("The minimum age is %d\n", calc_attr_unsigned_int);
        print_times(50, 2, "-");
        goto after_mode_execution;

    } else if (string_compare(user_input, "max age")) {
        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].age[0] * 1.0f;
        }
        calc_attr_unsigned_int = max(all_attributes, people_count);
        printf("The maximum age is %d\n", calc_attr_unsigned_int);
        print_times(50, 2, "-");
        goto after_mode_execution;

    } else if (string_compare(user_input, "min weight")) {
        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].weight[0] * 1.0f;
        }
        calc_attr_unsigned_int = min(all_attributes, people_count);
        printf("The minimum weight is %d\n", calc_attr_unsigned_int);
        print_times(50, 2, "-");
        goto after_mode_execution;

    } else if (string_compare(user_input, "max weight")) {
        for (unsigned int i = 0; i < people_count; ++i) {
            all_attributes[i] = people[i].weight[0] * 1.0f;
        }
        calc_attr_unsigned_int = max(all_attributes, people_count);
        printf("The maximum weight is %0d\n", calc_attr_unsigned_int);
        print_times(50, 2, "-");
        goto after_mode_execution;

    } else {
        int search_result = search_column(table, 0, user_input);
        if (search_result > 0) {
            print_person(people[search_result - 1]);
            goto after_mode_execution;
        }
    }
    printf("Invalid input or non-existant id: exiting program.\n");
    return 1;



/* ------ freeing memory and exiting the program ------ */

after_mode_execution:
    free(all_attributes);
    for (unsigned int i = 0; i < people_count; ++i) {
        deallocate_person(people[i]);
    }
    free(people);
    deallocate_string_mat(table);
    printf("\n");
    return 0;
}
