<?php
// Program-1: Form for RFID data input
?>
<!DOCTYPE html>
<html>
<head>
    <title>RFID Data Entry</title>
    <style>
        .form-container {
            max-width: 600px;
            margin: 20px auto;
            padding: 20px;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
        .form-group {
            margin-bottom: 15px;
        }
        label {
            display: block;
            margin-bottom: 5px;
        }
        input[type="text"], input[type="number"] {
            width: 100%;
            padding: 8px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }
        input[type="submit"] {
            background-color: #4CAF50;
            color: white;
            padding: 10px 15px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <div class="form-container">
        <h2>Enter RFID Data</h2>
        <form action="rfid_insert.php" method="POST">
            <div class="form-group">
                <label for="systime">System Time (YYYYMMDDHHMMSS):</label>
                <input type="text" id="systime" name="systime" required pattern="\d{14}" placeholder="e.g., 20250508123045">
            </div>
            <div class="form-group">
                <label for="IP">IP Address:</label>
                <input type="text" id="IP" name="IP" placeholder="e.g., 192.168.1.1">
            </div>
            <div class="form-group">
                <label for="rfidkey">RFID Key (12 characters):</label>
                <input type="text" id="rfidkey" name="rfidkey" required maxlength="12" pattern="[A-Za-z0-9]{12}">
            </div>
            <div class="form-group">
                <label for="activate">Activate Status (0 or 1):</label>
                <input type="number" id="activate" name="activate" required min="0" max="1" value="0">
            </div>
            <div class="form-group">
                <label for="startdatetime">Start DateTime (YYYYMMDDHHMMSS):</label>
                <input type="text" id="startdatetime" name="startdatetime" pattern="\d{14}" placeholder="e.g., 20250508123045">
            </div>
            <div class="form-group">
                <label for="enddatetime">End DateTime (YYYYMMDDHHMMSS):</label>
                <input type="text" id="enddatetime" name="enddatetime" pattern="\d{14}" placeholder="e.g., 20250508123045">
            </div>
            <input type="submit" value="Submit RFID Data">
        </form>
    </div>
</body>
</html>