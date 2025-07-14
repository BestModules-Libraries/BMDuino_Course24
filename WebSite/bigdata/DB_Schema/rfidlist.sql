-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2025-05-08 04:59:59
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
-- 資料表結構 `rfidlist`
--

CREATE TABLE `rfidlist` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料創立時間',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '資料新增日期時間',
  `IP` char(20) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL,
  `rfidkey` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT 'RFID卡號',
  `activate` int(11) NOT NULL DEFAULT 0 COMMENT '狀態',
  `startdatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '啟用時間',
  `enddatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '結束時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='RFID 清單列表';

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `rfidlist`
--
ALTER TABLE `rfidlist`
  ADD PRIMARY KEY (`id`),
  ADD KEY `rfidkey` (`rfidkey`,`activate`,`systime`,`IP`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `rfidlist`
--
ALTER TABLE `rfidlist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵', AUTO_INCREMENT=2;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
