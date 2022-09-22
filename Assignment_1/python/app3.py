from sys import argv
from os import listdir
from typing import Any, Callable


# The expected file name of the data file.
# If no argument is given, the program will search for this name in the working directory.
DATA_FILE_NAME = "Data.csv"


class Person:
    """
    A dataclass to model a person's data.
    To create a new person, id, name, age and weight are required.

    Attributes:
        - __id: private str
        - name: str
        - age: int
        - weight: int

    Properties:
        - id

    Class-wide methods:
        - apply_func_on_all_attr_list
        - index_by_id

    Methods:
        - age_verb
        _ gain_weight

    """

    def __init__(self, init_id: "str", init_name: "str", init_age: "int", init_weight: "int"):
        self.__id = init_id
        self.name = init_name
        self.age = init_age
        self.weight = init_weight

    @classmethod
    def apply_func_on_all_attr_list(cls,
                                    list_of_people: "list[Person]",
                                    func: "Callable[[list[Any]], Any]",
                                    attr: "str") -> "Any":
        """Given a list of persons and an attribute, put the values of the attribute of all the persons in the list in a list and pass it to a given function.

        Args:
            list_of_people (list[Person]): A list containing Person objects.
            func (Callable[[list[Any]], Any]): A function that takes a list as an first argument.
            attr (str): An attribute common to all Person objects.

        Returns:
            Any: The value returned by the function provided.
        """
        return func([getattr(person, attr) for person in list_of_people])

    @classmethod
    def index_by_id(cls,
                    list_of_people: "list[Person]",
                    id: "str") -> "Person":
        """Given a list of Persons, find a provided id, and return the first Person with the id in the list. If the id is not found, a ValueError is raised by the list.index() method.

        Args:
            list_of_people (list[Person]): a list containing person objects that all have an id.
            id (str): a string with the id to be searched.

        Returns:
            Person: the Person with the matching id.
        """
        list_of_id: "list[str]" = [person.id for person in list_of_people]
        return list_of_people[list_of_id.index(id)]

    @property
    def id(self) -> "str":
        """A property that returns the id of the person. Used to prevent writing to the __id attribute.

        Returns:
            str: the id of the person object.
        """
        return self.__id

    def age_verb(self, years: "int"):
        """Given a period in years, increment the Person's age by years.

        Args:
            years (int): period in years to add to the Person's age.
        """
        self.age += years

    def gain_weight(self, gain: "int"):
        """Given a weight, increment the Person's weight by the weight given.

        Args:
            gain (int): the weight to add to the Person's weight.
        """
        self.weight += gain

    def console_greet(self):
        """Great Person and print its first name to the console.
        """
        print(f'Hello {self.name.split(" ")[0]}')

    def __repr__(self) -> "str":
        return f"Person(\tid={self.id},\n\tname={self.name},\n\tage={self.age},\n\tweight={self.weight}\n)"


def read_file(path: "str") -> "str":
    """Given a file path, open the file in read mode and dump return its contents as a string.
    If the file is not found, a error message is printed and the exit function is called with exit code 1. The file is closed before return.

    Args:
        path (str): the path to file to be read.

    Returns:
        str: the contents of the file as a string.
    """
    try:
        with open(path, "r") as file:
            return_string = file.read()
        return return_string
    except FileNotFoundError:
        __exit_p(1, "No such file found. Please enter a valid path to the file.")


def build_table(raw_string: "str") -> "list[list[str]]":
    """Given a string that represents comma separated values, 
    return a list that contains multiple lists which contain a string 
    representing a single cell of the table.

    An inner list represents a row of the table.

    Any cell of the table can be accessed with 
    the following notation: 

    `table[row][column]`

    where row and column are zero-based indices.

    Args:
        raw_string (str): a string with comma separated values.

    Returns:
        list[list[str]]: a list of lists of string, inner lists representing rows.
    """
    return [line.split(",") for line in raw_string.strip().split("\n")]


def validate_table(table: "list[list[Any]]", column_count: "int | None" = None) -> bool:
    """Given a table, validate that it is at least 2 rows long and that each row has a given column_count. 

    If no column_count is given the length of the first row will be used as a reference.

    If the table passes, True is returned. Otherwise False is returned.

    The function does not handle exceptions. A possible exception may be an IndexError.

    Args:
        table (list[list[Any]]): a table of the notation `table[row][column]`.
        column_count (int | None, optional): the exacted column count for each row. Defaults to None.

    Returns:
        bool: True if the table passes. False if it fails.
    """
    if len(table) < 2:
        return False
    initial_column_count = column_count if column_count else len(table[0])
    for row in table[1:]:
        if len(row) == initial_column_count:
            continue
        else:
            return False
    return True


def __build_model(data_table: "list[list[str]]"):
    """Given a data_table that contains id, name, age, and weight of Person objects, return list of Persons based on that data.

    The function will try to convert any str to an int where needed. 
    Upon a ValueError, the function will print an error message 
    and exit the program with code 1.

    Args:
        data_table (list[list[str]]): a table with the notation `table[row][column]`

    Returns:
        list[Person]: a list of Person objects with the data from the table.
    """
    return_list: list[Person] = []
    for row in data_table:
        try:
            return_list.append(
                Person(row[0], row[1], int(row[2]), int(row[3])))
        except ValueError:
            __exit_p(1, "Failed to build model. A most probable cause is a failure to cast a table cell into a number. Please insure that there are no leading zeroes or delimiters in the numbers.")
    return return_list


def print_table(table: "list[list[str]]"):
    """Given a table, print its contents to the console.

    The padding will be one plus the length of the longest string in any respective column. 

    Args:
        table (list[list[str]]): a table with the notation `table[row][column]`
    """
    number_of_columns = len(table[0])
    padding_of_each_column: "list[int | float]" = []
    for column_index in range(len(table[0])):
        padding_of_each_column.append(
            int(max(
                [len(table[row_index][column_index]) for row_index in range(len(table))])) + 1)

    for row_index, line in enumerate(table):
        if row_index in [0, 1]:
            print_times(int(sum(padding_of_each_column)) +
                        number_of_columns + 1, 1)
        print("|", end="")
        for column_index, item in enumerate(line):
            print(
                f"{item.ljust(int(padding_of_each_column[column_index]))}|", end="")
        print()
    print_times(int(sum(padding_of_each_column)) +
                number_of_columns + 1, 2)


def reduce(func: "Callable[[Any, Any], Any]",
           data_set: "list[Any]",
           initial_value: "None | Any" = None) -> "Any":
    """Given a function, a data_set, and an optional initial_value, 
    loop through the data_set and pass each value and the previous 
    result of func to func again and continue till the data_set is exhausted.

    If an initial_value is provided it will be used for the first value of data_set for the first iteration.

    If it was not provided, the first and second values of the data_set will be used instead.

    The function does not handle exception or check the length of the data set. A possible exception is a IndexError.

    Args:
        condition_func (Callable[[Any, Any], Any]): the function to be applied on the data_set.
        data_set (list[Any]): a list of values compatible with func.
        initial_value (None | Any, optional): a seed value for the first iteration. Defaults to None.

    Returns:
        Any: the cumulative reduction value of the data_set
    """
    compiled_value: "Any"
    start: "int" = 0
    if not initial_value:
        start = 1
        compiled_value = data_set[0]
    else:
        compiled_value = initial_value

    for value in data_set[start:]:
        compiled_value = func(compiled_value, value)

    return compiled_value


def sum(data_set: "list[int | float]") -> "float":
    """Give a list of integers or floats, calculate the sum of the data_set.

    The return value will always be a float.

    Args:
        data_set (list[int | float]): a list of real numbers.

    Returns:
        float: the sum of the data_set.
    """
    return float(reduce(lambda x, y: x + y, data_set))


def average(data_set: "list[int | float]") -> "float":
    """Given a list of integers or floats, calculate the average value of the data_set.

    The return value will always be a float.

    Args:
        data_set (list[int | float]): a list of real numbers.

    Returns:
        float: the average of the data_set.
    """
    return float(sum(data_set) / len(data_set))


def min(data_set: "list[int | float]") -> "float":
    """Given a list of integers or floats, find the minimum value of the data_set.

    The return value will always be a float.

    Args:
        data_set (list[int | float]): a list of real numbers.

    Returns:
        float: the minimum value of the data_set.
    """
    return float(reduce(lambda x, y: y if y < x else x, data_set))


def max(data_set: "list[int | float]") -> "float":
    """Given a list of integers or floats, find the maximum value of the data_set.

    The return value will always be a float.

    Args:
        data_set (list[int | float]): a list of real numbers.

    Returns:
        float: the maximum value of the data_set.
    """
    return float(reduce(lambda x, y: y if y > x else x, data_set))


def print_times(count: "int" = 50, new_lines: "int" = 1, string: "str" = "-"):
    """Given a optional count, an optional number of new_lines, and an optional string, print the string `count` times and then print a new line `new_line` times.

    If no arguments are given the equivalent of the following string will be printed:

    "--------------------------------------------------\\n"

    Args:
        count (int, optional): number of times to print string. Defaults to 50.
        new_lines (int, optional): number of new lines to print after the last print of string. Defaults to 1.
        string (str, optional): the string to be printed. Defaults to "-".
    """
    print(count * string + new_lines * "\n", end="")


def __exit_p(code: "int", message: "str" = ""):
    """Provides a unified way to exit the program. Prints an optional message and then exits returning a provided exit code.

    Args:
        code (int): the exit code.
        message (str, optional): exit message. Defaults to "".
    """
    print(message)
    exit(code)


def main():
    """The first function to be called if the module is executed directly.

    It houses all user-interaction elements and all output elements aside from error messages.

    It will:
        1- find the data file.
        2- print the raw string from the data file.
        3- build a table.
        4- convert the table to list of Persons.
        5- compute the minimum, maximum and average age.
        6- print the ages to the console.
        7- wait for user input to examine other options.
        
    Args:
        - the path to the data file.
    """
    file_path: "str"
    if len(argv) > 1:
        file_path = argv[-1]
    else:
        if not (DATA_FILE_NAME in listdir()):
            file_path = input("Enter path to the CSV file:\npath> ")
            print()
        else:
            file_path = DATA_FILE_NAME

    raw_string: "str" = read_file(file_path)
    print(raw_string)
    print_times(50, 2)

    table = build_table(raw_string)
    if not validate_table(table, 4):
        __exit_p(1, "The CSV file is corrupt.")
    people: "list[Person]" = __build_model(table[1:])

    attr = "age"
    print(
        f"""\
The average {attr} is {Person.apply_func_on_all_attr_list(people, average, attr)}
The minimum {attr} is {Person.apply_func_on_all_attr_list(people, min, attr)}
The maximum {attr} is {Person.apply_func_on_all_attr_list(people, max, attr)}
""")
    print_times(50, 2)

    print("""Type "table" to show data""")
    print("""or "average" before an attribute to show the average""")
    print("""or "min" before an attribute to show the minimum""")
    print("""or "max" before an attribute to show the maximum""")
    print("""or "model" to print all person objects""")
    print("""or the id of a person object to print it""")
    print("""or "exit" to quit the program.""", end="\n\n")

    mode: "list[str]" = input("mode> ").strip().split(" ")
    print()

    if (mode[0] == "exit" and len(mode) == 1):
        __exit_p(0)

    elif ("table" in mode and len(mode) == 1):
        print_table(table)

    elif ("model" in mode and len(mode) == 1):
        for person in people:
            print(person)
        print_times(50, 2)

    elif ("average" in mode and len(mode) == 2):
        if not (mode[1] in dir(people[0])):
            __exit_p(1, f"""The attribute "{mode[1]}" could not be found""")
        print(
            f"""The average {mode[1]} is {Person.apply_func_on_all_attr_list(people, average, mode[1])}""")

    elif ("min" in mode and len(mode) == 2):
        if not (mode[1] in dir(people[0])):
            __exit_p(1, f"""The attribute "{mode[1]}" could not be found""")
        print(
            f"""The minimum {mode[1]} is {format(Person.apply_func_on_all_attr_list(people, min, mode[1]), "0.2f")}""")

    elif ("max" in mode and len(mode) == 2):
        if not (mode[1] in dir(people[0])):
            __exit_p(1, f"""The attribute "{mode[1]}" could not be found""")
        print(
            f"""The maximum {mode[1]} is {format(Person.apply_func_on_all_attr_list(people, max, mode[1]), "0.2f")}""")
        print_times(50, 2)

    elif (len(mode) == 1 and mode[0].isnumeric()):
        try:
            print(Person.index_by_id(people, mode[0]))
            print()
        except ValueError:
            __exit_p(1, "The provided id not present in the group.")

    else:
        __exit_p(1, "Invalid input: exiting program.")


if __name__ == "__main__":
    main()
    exit(0)
