#include <stdio.h>

typedef struct _Person {
    char id[10];
    char name[25];
    unsigned int age;
    unsigned int weight;
} Person;

char *char_replace(char *target_string, char target_char, char replacement, unsigned int length) {
    for (unsigned int index = 0; index < length; ++index) {
        if (target_string[index] == target_char) {
            target_string[index] = replacement;
        }
    }
    return target_string;
}


int main() {
    char file_path[260];
    char temp_string[25];
    char current_char;

    Person people[100]
    unsigned int index = 0;
    FILE *input_file;

    fgets(file_path, 260, stdin);
    input_file = fopen(file_path, "r");

    current_char = getc(input_file);
    while (current_char = EOF) {
        if (current_char == ',')
    }

    return 0;
}
