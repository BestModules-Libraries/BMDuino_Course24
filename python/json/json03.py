import json #將json套件載入
jsonStr='''
    [    
        {"編號": "A","品名": "雙人分享餐","單價": 120},
        {"編號": "B","品名": "歡樂全家餐","單價": 399},
        {"編號": "C","品名": "情人精緻餐","單價": 540}
    ]
'''
#產生一個多維的jsonstr字串(格式類似json)

listMeal=json.loads(jsonStr)    #將jsonstr字串轉成對應的json物件

for meal in listMeal:   # 用迴圈，把多維的json展成一個一個json 物件(只有一維)
    print("編號：%s"%(meal["編號"])) #印出json物件中的：編號
    print("品名：%s"%(meal["品名"])) #印出json物件中的:品名
    print("單價：%d"%(meal["單價"])) #印出json物件中的:單價
    print("="*40)   #印出40個"="

