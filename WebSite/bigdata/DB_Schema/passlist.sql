-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2025-05-12 14:21:13
-- 伺服器版本： 10.4.32-MariaDB
-- PHP 版本： 8.2.12

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
-- 資料表結構 `passlist`
--

CREATE TABLE `passlist` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料建立時間',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '資料日期時間',
  `mid` int(11) NOT NULL COMMENT '連接rfidlist主鍵Key值',
  `lockdevice` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '門鎖裝置編號',
  `aactivate` int(11) NOT NULL COMMENT '狀態',
  `startdatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '啟用日期時間',
  `enddatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '停用日期時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `passlist`
--
ALTER TABLE `passlist`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `midkey` (`mid`,`lockdevice`,`systime`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `passlist`
--
ALTER TABLE `passlist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
