import json
data = {"DEVICE": "E89F6DE869F8", "SENSOR": "RELAY", "SET": 4, "OPERATION": "HIGH"}
# aaa = json.load(data)
aaa = json.dumps(data, indent=4)
print(aaa)
print(data)
