<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>RFID 停用系統</title>
    <style>
        body {
            font-family: "Microsoft JhengHei", sans-serif;
            margin: 30px;
            background-color: #f0f0f0;
        }
        .container {
            background-color: white;
            padding: 20px;
            border-radius: 12px;
            box-shadow: 0 2px 8px rgba(0,0,0,0.2);
            max-width: 400px;
            margin: auto;
        }
        input[type="text"] {
            width: 100%;
            padding: 8px;
            margin: 6px 0 16px;
            border: 1px solid #ccc;
            border-radius: 6px;
        }
        button {
            padding: 10px 20px;
            background-color: #007BFF;
            color: white;
            border: none;
            border-radius: 6px;
            cursor: pointer;
        }
        button:hover {
            background-color: #0056b3;
        }
        #result {
            margin-top: 20px;
            white-space: pre-wrap;
            background: #f8f8f8;
            padding: 10px;
            border-radius: 6px;
            border: 1px solid #ccc;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>RFID 停用介面</h2>
        <label for="mac">裝置 MAC 位址：</label>
        <input type="text" id="mac" placeholder="例如：B8D61A68DAFC">

        <label for="key">RFID 卡號：</label>
        <input type="text" id="key" placeholder="例如：0219390674">

        <button onclick="deactivateCard()">停用卡片</button>

        <div id="result"></div>
    </div>

    <script>
        // 停用 RFID 卡片的主函式
        function deactivateCard() {
            const mac = document.getElementById("mac").value.trim();
            const key = document.getElementById("key").value.trim();

            if (!mac || !key) {
                alert("請輸入 MAC 與 RFID 卡號");
                return;
            }

            // 構造 API URL，請依實際路徑調整
            const apiUrl = `http://localhost:8888/bigdata/rfid/disact.php?MAC=${encodeURIComponent(mac)}&KEY=${encodeURIComponent(key)}`;

            // 發送 GET 請求
            fetch(apiUrl)
                .then(response => {
                    if (!response.ok) throw new Error("伺服器錯誤");
                    return response.json();
                })
                .then(data => {
                    // 將回應的 JSON 顯示在畫面上
                    const output = `
🔹 裝置 MAC：${data.Device}
🔹 RFID 卡號：${data.Card}
🔹 操作結果：${data.Result}
                    `;
                    document.getElementById("result").innerText = output;
                })
                .catch(error => {
                    document.getElementById("result").innerText = "⚠️ 錯誤：" + error.message;
                });
        }
    </script>
</body>
</html>
