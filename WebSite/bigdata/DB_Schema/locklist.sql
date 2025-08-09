-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
<<<<<<< HEAD
-- 產生時間： 2025 年 06 月 29 日 18:01
-- 伺服器版本： 5.5.68-MariaDB
-- PHP 版本： 8.2.27
=======
-- 產生時間： 2025 年 05 月 25 日 23:17
-- 伺服器版本： 5.5.68-MariaDB
-- PHP 版本： 8.2.21
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 資料庫： `big`
--

-- --------------------------------------------------------

--
-- 資料表結構 `locklist`
--

CREATE TABLE `locklist` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `crtdatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料建立時間',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '資料日期時間',
  `mid` int(11) NOT NULL COMMENT '連接rfidlist主鍵Key值',
  `lockdevice` char(12) CHARACTER SET ascii NOT NULL COMMENT '門鎖裝置編號',
<<<<<<< HEAD
  `cardnumber` char(12) CHARACTER SET ascii NOT NULL COMMENT '卡號',
  `lactivate` int(11) NOT NULL COMMENT '狀態',
  `startdatetime` char(14) CHARACTER SET ascii DEFAULT NULL COMMENT '啟用日期時間',
  `enddatetime` char(14) CHARACTER SET ascii DEFAULT NULL COMMENT '停用日期時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='集團門鎖卡號權限資料表';
=======
  `lactivate` int(11) NOT NULL COMMENT '狀態',
  `startdatetime` char(14) CHARACTER SET ascii DEFAULT NULL COMMENT '啟用日期時間',
  `enddatetime` char(14) CHARACTER SET ascii DEFAULT NULL COMMENT '停用日期時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='某卡號授權許可門禁裝置';
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `locklist`
--
ALTER TABLE `locklist`
  ADD PRIMARY KEY (`id`),
<<<<<<< HEAD
  ADD UNIQUE KEY `midkey` (`mid`,`lockdevice`,`systime`),
  ADD UNIQUE KEY `checkcard` (`lockdevice`,`cardnumber`);
=======
  ADD UNIQUE KEY `midkey` (`mid`,`lockdevice`,`systime`);
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `locklist`
--
ALTER TABLE `locklist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
