import json 
with open("./sample_gen.json", 'r') as fp:
   data =  json.load(fp)

for item in data:
    print(item)

