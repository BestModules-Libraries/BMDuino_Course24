-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
-- 產生時間： 2025 年 05 月 25 日 23:22
-- 伺服器版本： 5.5.68-MariaDB
-- PHP 版本： 8.2.21

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
-- 資料表結構 `lockdevice`
--

CREATE TABLE `lockdevice` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `crtdatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料建立時間',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '資料日期時間',
  `IP` char(20) CHARACTER SET ascii DEFAULT NULL COMMENT '連線IP Address',
  `deviceid` char(12) CHARACTER SET ascii NOT NULL COMMENT '門鎖裝置編號',
  `devicename` char(60) DEFAULT NULL,
  `activate` int(11) NOT NULL COMMENT '狀態',
  `location` char(200) DEFAULT NULL COMMENT '位置',
  `building` char(120) DEFAULT NULL COMMENT '為哪一個區域與建築物與樓層'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='門禁裝置清單';

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `lockdevice`
--
ALTER TABLE `lockdevice`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`deviceid`,`devicename`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `lockdevice`
--
ALTER TABLE `lockdevice`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
