idlist=[]
agelist=[]
weightlist=[]
namelist=[]



#assigning variables and lists
with open("Data.csv", "r") as file:
    for line in file:
        print(line)
        id, name,age,weight =line.split(',')
        
        try:
            idInput=int(id)
            idlist.append(idInput)

        except:
            continue

        try:
            ageInput=int(age)
            agelist.append(ageInput)

        except:
            continue

        try:
            weightInput=int(weight)
            weightlist.append(weightInput)

        except:
            continue

        try:
            nameInput=name
            namelist.append(nameInput)

        except:
            continue

        

age_Average= (sum(agelist))/len(agelist)
weight_Average= (sum(weightlist))/len(weightlist)

   
#testing the current progress....
#print(id)
#print(namelist)
#print(agelist)
#print(weightlist)
#print(idlist)
#data_file.close()
print("The average is: ", age_Average)
#print("The average is: ", weight_Average)
