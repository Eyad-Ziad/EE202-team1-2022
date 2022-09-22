class person: 
    def __init__(self, id,name,age,weight):
        self.id =id
        self.name = name
        self.age = age
        self.weight = weight


with open("./data/Data.csv", "r") as file:
    raw_string = file.read()

print(raw_string)
