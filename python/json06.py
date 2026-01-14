import json, os  		            # 引用json與os套件
import matplotlib.pyplot as plt  	# 引用圖表使用套件
# 讀取109年9月臺中市10大易肇事路口.json資料並放入listTrafficEvent串列物件
f=open("109年9月臺中市10大易肇事路口.json", "r",encoding="utf_8")   #開啟109年9月臺中市10大易肇事路口的json檔
listTrafficEvent = json.load(f) #將109年9月臺中市10大易肇事路口的資料讀入後，轉籌json格式，存放在listTrafficEvent
f.close()   #檔案關比
print(listTrafficEvent) #印出109年9月臺中市10大易肇事路口的資料內容
# 將 listTrafficEvent 串列中的每筆字典物件印出來
for trafficEvent in listTrafficEvent:
    for key in trafficEvent:
        show=False  #控制分行==========的旗標
        # 若該筆字典的編號(key)有值(value)即印出
        if (trafficEvent['編號']!=""):
            print("%s：%s" %(key, trafficEvent[key]))
            show=True   #控制分行==========的旗標設為成立
        else:
            break
    if (show==True):    #如果控制分行==========的旗標成立
        print("="*30)    #印出30個"="
        #此敘述用來列印每一筆資料時分隔資料用
        
total=0.0        # 統計肇事數量
listAllEvent=[]  # listAllEvent用來存放車禍主要肇因
for trafficEvent in listTrafficEvent:   #用迴圈解譯一筆一筆主要肇因的資料
    if (trafficEvent['編號']!=""):    #該筆資料是正確的
        listAllEvent+=[trafficEvent['主要肇因']]
        total+=1
print("listAllEvent=",listAllEvent) #列印出主要肇因的資料集

listEvent=[]   		#存放所有車禍主要肇因，此串列存放不重複的車禍主要肇因
listCount=[]  		#存放各主要肇因對應的車禍數量
for event in set(listAllEvent):   # 使用set()移除listAllEvent串列中重複的主要肇因
   print('主要肇因 %s 共 %s 件' %(event,listAllEvent.count(event)))
   listEvent+=[event]   #取得獨立肇因的名字
   listCount+=[listAllEvent.count(event)]   #取得獨立肇因的次數

# 計算各車禍主要肇因的百分比，並放入listPercent串列
listPercent=[]  #計算主要肇因的百分比，並放入listPercent串列
for c in listCount:
    listPercent.append(round((float(c)/total)*100)) #將計算主要肇因的百分比，並放入listPercent串列
    
# 繪製車禍主要肇因的圓餅圖
font={"family":"DFKai-SB"}  #設定繪圖圖表字型
plt.rc("font", **font)  #設定繪圖圖表字型
plt.pie(listPercent, labels=listEvent, autopct="%3.1f%%")   #繪出Pie Chart圖
plt.savefig('event.png', dpi=300) 	# 將圓餅圖出成圖片，檔名為event.png 
plt.show()                 	        # 顯示圓餅圖
os.system('event.png')  	        # 開啟圓餅圖event.png圖片
print("圖表建置成成功")