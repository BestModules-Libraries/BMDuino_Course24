<?php
// Program-3: Display all RFID records
include "iotcnn.php";

$link = Connection();
?>
<!DOCTYPE html>
<html>
<head>
    <title>RFID Records</title>
    <style>
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
        }
        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
        }
        tr:nth-child(even) {
            background-color: #f9f9f9;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
        }
        .back-link {
            display: inline-block;
            margin-bottom: 20px;
            text-decoration: none;
            color: #4CAF50;
        }
    </style>
</head>
<body>
    <div class="container">
        <a href="rfid_form.php" class="back-link">Back to Form</a>
        <h2>RFID Records</h2>
        <table>
            <tr>
                <th>ID</th>
                <th>Create DateTime</th>
                <th>System Time</th>
                <th>IP</th>
                <th>RFID Key</th>
                <th>Activate</th>
                <th>Start DateTime</th>
                <th>End DateTime</th>
            </tr>
            <?php
            $sql = "SELECT * FROM rfidlist";
            $result = mysqli_query($link, $sql);

            if (mysqli_num_rows($result) > 0) {
                while ($row = mysqli_fetch_assoc($result)) {
                    echo "<tr>";
                    echo "<td>" . $row['id'] . "</td>";
                    echo "<td>" . $row['crtdatetime'] . "</td>";
                    echo "<td>" . $row['systime'] . "</td>";
                    echo "<td>" . ($row['IP'] ?? 'N/A') . "</td>";
                    echo "<td>" . $row['rfidkey'] . "</td>";
                    echo "<td>" . $row['activate'] . "</td>";
                    echo "<td>" . ($row['startdatetime'] ?? 'N/A') . "</td>";
                    echo "<td>" . ($row['enddatetime'] ?? 'N/A') . "</td>";
                    echo "</tr>";
                }
            } else {
                echo "<tr><td colspan='8'>No records found</td></tr>";
            }

            mysqli_close($link);
            ?>
        </table>
    </div>
</body>
</html>