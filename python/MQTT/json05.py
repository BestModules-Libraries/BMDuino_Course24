import json #將json套件載入
f=open("meal.json", "r", encoding="utf_8")
#開啟一個meal.json的檔案用於讀入，並且用utf_8為文字編碼
listMeal = json.load(f) #運用json物件將meal.json的檔案載入，轉成json元素

for meal in listMeal:   #將二維的json元素，用for each迴圈轉成一維
    for key in meal:    #讀取一為json元素中每一個key的資料
        print("%s：%s" %(key, meal[key]))    #印出 key與以此key之json元素之內容
    print("折扣：%.2f" %(float(meal[key])*0.9))    #印出折扣後的數值
    print("="*20)   #印出20個"="