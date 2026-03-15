import json #將json套件仔入
dictMeal={"編號":"A", "品名":"雙人分享餐", "單價":120}#產生一個字典字串
jsonStr=json.dumps(dictMeal, ensure_ascii=False, indent=4)
#t將字典字串dictMeal 轉成json格式，並未之縮排(indent=4)，然後把格式化之json物件，轉成對應的字串回傳到jsonStr
print(jsonStr)  #印出jsonStr

