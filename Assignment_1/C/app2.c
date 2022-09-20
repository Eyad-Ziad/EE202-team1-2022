#include <stdio.h>
#include <stdlib.h>

typedef struct _string_mat {
    char *inter_padded_strings;
    unsigned long *cell_size;
    unsigned long *column_count;
    unsigned long *row_count;
} string_mat;

typedef struct _Person {
    char *id;
    char *name;
    unsigned int *age;
    unsigned int *weight;
} Person;

unsigned int *allocate_unsigned_int(unsigned int size) {
    unsigned int *return_unsigned_int = malloc(sizeof(unsigned int) * size);
    if (return_unsigned_int == NULL) {
        printf("Allocation fail [0]: returning NULL.");
    }
    return return_unsigned_int;
}

char *allocate_string(unsigned int size) {
    char *return_string = malloc(sizeof(char) * (size + 1));
    if (return_string == NULL) {
        printf("Allocation fail [1]: returning empty string.");
        return "";
    }
    return_string[size] = '\0';
    return return_string;

}

char *reallocate_string(char *target_string, unsigned int new_size) {
    char *return_string = realloc(target_string, new_size);
    if (return_string == NULL) {
        printf("Allocation fail [2]: returning original string.");
        return target_string;
    }
    return return_string;
}

char *string_truncate_trailing_nulls(char *target_string, unsigned int length) {
    char *return_string = target_string;
    for (unsigned int index = 0; index < length; ++index) {
        if (return_string[index] == '\0') {
            if (length == (index + 1)) {
                return return_string;
            }
            return_string = reallocate_string(return_string, length - (index + 1));
        }
    }
    return return_string;
}

char *string_mid_index_copy(char *target_string, char *reference_string, unsigned int count, unsigned int starting_index) {
    for (unsigned int index = 0; index < count; ++index) {
        target_string[index] = reference_string[index + starting_index];
    }
    return target_string;
}

char *string_simple_copy(char *target_string, char *reference_string, unsigned int length) {
    for (int i = 0; i < length; ++i) {
        target_string[i] = reference_string[i];
    }
    return target_string;
}

unsigned int string_mat_get_absolute_index(string_mat table, unsigned int row, unsigned int column) {
    unsigned int return_unsigned_int = (row * table.cell_size[0] * table.column_count[0]) + (table.cell_size[0] * column);
    return return_unsigned_int;
}

char *string_mat_cartesian_index(string_mat table, unsigned int row, unsigned int column) {
    char *return_string = allocate_string(table.cell_size[0]);
    unsigned int absolute_index = string_mat_get_absolute_index(table, row, column);
    return_string = string_mid_index_copy(return_string, table.inter_padded_strings, table.cell_size[0], absolute_index);
    return_string = string_truncate_trailing_nulls(return_string, table.cell_size[0]);
    return return_string;

}

string_mat csv_to_string_mat(char *path) {

    string_mat table;
    
    unsigned int comma_count = 0;
    unsigned int current_comma_count = 0;

    unsigned int largest_cell = 0;
    unsigned int current_largest_cell = 0;

    unsigned int row_count = 0;

    FILE *input_file = fopen(path, "r");
    char current_char = getc(input_file);
    char last_char = '\n';

    char while_exit_flag = 0;

    while (current_char != EOF) {
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
        current_char = getc(input_file);
    }
    
    fseek(input_file, 0, SEEK_SET);

    table.inter_padded_strings = calloc(largest_cell * row_count * (comma_count + 1), sizeof(char) * largest_cell * row_count * (comma_count + 1));
    table.column_count = malloc(sizeof(int));
    table.row_count = malloc(sizeof(int));
    table.cell_size = malloc(sizeof(int));

    table.cell_size[0] = largest_cell;
    table.column_count[0] = comma_count + 1;
    table.row_count[0] = row_count;


    unsigned int cells_left = largest_cell;
    unsigned int index = 0;
    while(1) {
        if (while_exit_flag) {
            break;
        }
        last_char = getc(input_file);
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

            case -1:
                while_exit_flag = 1;
            break;

            default:
                table.inter_padded_strings[index++] = last_char;
                --cells_left;
            break;
        }
    }
    
    fclose(input_file);
    
    return table;
}

Person *allocate_person(unsigned int count) {
    Person *return_person = malloc(sizeof(Person) * count);
    if (return_person == NULL) {
        printf("Allocation fail [3]: returning NULL.");
    }
    return return_person;
}

Person string_mat_to_person(string_mat table, unsigned int entry) {
    Person return_person;

    return_person.id = allocate_string(table.cell_size[0] + 1);
    return_person.name = allocate_string(table.cell_size[0] + 1);
    return_person.age = allocate_unsigned_int(1);
    return_person.weight = allocate_unsigned_int(1);

    return_person.id = string_simple_copy(return_person.id, string_mat_cartesian_index(table, entry, 0), table.cell_size[0]);

    return_person.name = string_simple_copy(return_person.name, string_mat_cartesian_index(table, entry, 1), table.cell_size[0]);
    

    char *temp_age = allocate_string(table.cell_size[0]);
    temp_age = string_simple_copy(temp_age, string_mat_cartesian_index(table, entry, 2), table.cell_size[0]);

    char *temp_weight = allocate_string(table.cell_size[0]);
    temp_weight = string_simple_copy(temp_weight, string_mat_cartesian_index(table, entry, 3), table.cell_size[0]);

    sscanf(temp_weight, "%u", return_person.weight);

    sscanf(temp_age, "%u", return_person.age);


    return return_person;
}

void deallocate_string_mat(string_mat target_string) {
    free(target_string.cell_size);
    free(target_string.column_count);
    free(target_string.row_count);
    free(target_string.inter_padded_strings);
}

void deallocate_person(Person target_person) {
    free(target_person.age);
    free(target_person.id);
    free(target_person.name);
    free(target_person.weight);
}

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

void p_separator(unsigned int number_of_new_lines) {
    printf("-----------------------------------------");
    for (int count = 0; count < number_of_new_lines; ++count) {
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    FILE *input_file = fopen(argv[1], "r");
    char *raw_string = get_string_from_file_stream(input_file);
    printf("%s", raw_string);
    fclose(input_file);
    free(raw_string);
    printf("-----------------------------------------\n\n\n");

    string_mat table = csv_to_string_mat(argv[1]);
    
    for (int i = 0; i < table.row_count[0]; ++i) {
        if (i == 1 || i == 0) {
            p_separator(1);
        }
        printf("|%4s|%-21s|%4s|%7s|\n", string_mat_cartesian_index(table, i, 0), string_mat_cartesian_index(table, i, 1), string_mat_cartesian_index(table, i, 2), string_mat_cartesian_index(table, i, 3));
    }
    p_separator(1);

    unsigned int person_count = table.row_count[0] - 1;

    Person *people = allocate_person(person_count);
    
    for (int count = 0; count < person_count; ++count) {
        people[count] = string_mat_to_person(table, count + 1);
    }

    for (int i = 0; i < person_count; ++i) {
        printf("Person(\tid: *char = %s,\n\tname: *char = %s,\n\tage: int = %u,\n\tweight: int = %u )\n\n", people[i].id, people[i].name, people[i].age[0], people[i].weight[0]);
    }
    p_separator(2);

    float average_age = 0.0f;
    float age_sum = 0.0f;

    for (int i = 0; i < person_count; ++i) {
        age_sum += people[i].age[0];
    }

    average_age =  age_sum / person_count;

    printf("The average age is %0.2f\n", average_age);
    p_separator(2);

    return 0;

}
