import json #將json套件載入
listMeal=[{"編號":"A", "品名":"雙人分享餐", "單價":120},
          {"編號":"B", "品名":"歡樂全家餐", "單價":399},
          {"編號":"C", "品名":"情人精緻餐", "單價":540}]

#產生一個dictMeal字串，是多維的字典資料
f=open("meal.json","w",encoding="utf_8")
#開啟一個meal.json的檔案用於寫入，並且用utf_8為文字編碼

json.dump(listMeal, f,  ensure_ascii=False, indent=4)
#將listMeal 用json dump方法，並且有縮排，轉寫入meal.json的檔案的內容
f.close()   #關閉meal.json的檔案，不再讀寫
print("JSON餐點記錄建置完成")   #印出JSON餐點記錄建置完成"
