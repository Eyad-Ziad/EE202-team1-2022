from typing import Any, Callable
from typing_extensions import Self
from sys import argv


class Person:
    def __init__(self, init_id: str, init_name: str, init_age: int, init_weight: int) -> None:
        self.__id = init_id
        self.name = init_name
        self.age = init_age
        self.weight = init_weight
        self.relations: list[tuple[Self, str]] = []

    @property
    def id(self):
        return self.__id

    def age_verb(self, years: int):
        self.age += years

    def gain_weight(self, gain: int):
        self.weight += gain

    def console_greet(self):
        print(f'Hello {self.name.split(" ")[0]}')

    def new_relation(self, person: Self, relation: str, relation_self: str | None = None):
        self.relations.append((person, relation))
        person.relations.append(
            (self, relation if not relation_self else relation_self))

    def __repr__(self) -> str:
        return f"Person(id={self.id}, name={self.name})"


def read_file(path: str) -> str:
    with open(path, "r") as file:
        return file.read()


def print_raw_string(raw_string: str) -> None:
    print(raw_string)


def build_table(raw_string: str) -> list[list[str]]:
    return [line.split(",") for line in raw_string.strip().split("\n")]


def build_model(raw_string: str) -> list[Person]:
    return_list: list[Person] = []
    for line in raw_string.strip().split("\n")[1:]:
        person_args: list[Any] = line.split(",")
        person_args[2] = int(person_args[2])
        person_args[3] = int(person_args[3])
        return_list.append(Person(*person_args))
    return return_list


def average(data_set: list[int]) -> float:
    return sum(data_set) / len(data_set)


def apply_func_on_all_attr(
    func: Callable[[list[Any]], Any],
    attr: str,
    list_of_people: list[Person]
) -> Any:
    return func([getattr(person, attr) for person in list_of_people])


def p_separator(number_of_new_lines: int):
    print("-----------------------------------------", end="");
    for i in range(number_of_new_lines):  # type:ignore
        print("\n", end="")

def main():
    raw_string = read_file(argv[1])
    people = build_model(raw_string)
    
    print(raw_string)
    
    for index, line in enumerate(build_table(raw_string)):
        if index in [0, 1]:
            p_separator(1)
        print(f"""|{line[0].rjust(4, " ")}|{line[1].ljust(21, " ")}|{line[2].rjust(4)}|{line[3].rjust(7, " ")}|""")
    
    p_separator(2)
    
    for person in people:
        print(person)

    p_separator(2)
    
    print(f"""The average age is {apply_func_on_all_attr(average, "age", people)}""")
    
    p_separator(2)

main()
