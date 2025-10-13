# 在此範例中，我們首先使用 json.dumps() 函數
# 將 json_data 字典轉換為 JSON 格式的字串，
# 然後使用 json.loads() 函數
# 將其解析為 JSON 物件（即 Python 中的字典）。
# 接下來，我們使用 for 迴圈遍歷該 JSON 物件的所有鍵（key），
# 並使用 print() 函數來印出每個鍵值對（key-value pair）的內容，
# 其中 str() 函數用於將值轉換為字串。

import json

# 定義一個包含設備資料的字典
json_data = {
  "DEVICE": "E89F6DE869F8",  # 設備 MAC 地址
  "SENSOR": "RELAY",  # 感測器類型
  "SET": 4,  # 設定值
  "OPERATION": "HIGH"  # 操作狀態
}

# 使用 json.dumps() 將字典轉換為 JSON 格式的字串，並進行縮排
my_json = json.dumps(json_data, indent=4)

# 打印 JSON 格式的字串
print(my_json)

# 使用 json.loads() 將 JSON 格式的字串解析為 Python 字典
parsed_json = json.loads(my_json)

# 使用 for 迴圈遍歷字典中的每一個鍵值對，並打印出來
for key in parsed_json:
    print(key + ': ' + str(parsed_json[key]))
    # 打印鍵和值，其中 str() 函數將值轉換為字串
