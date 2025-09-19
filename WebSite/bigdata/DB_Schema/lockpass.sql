-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
-- 產生時間： 2025 年 05 月 25 日 23:18
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
-- 資料表結構 `lockpass`
--

CREATE TABLE `lockpass` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `crtdatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料建立時間',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '資料日期時間',
  `IP` char(20) CHARACTER SET armscii8 DEFAULT NULL COMMENT '裝置連線IP',
  `MAC` char(12) CHARACTER SET ascii NOT NULL COMMENT '裝置卡編號(網卡編號)',
  `cardnumber` char(12) CHARACTER SET ascii NOT NULL COMMENT '卡號',
  `status` int(1) NOT NULL COMMENT '狀態'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='門鎖刷卡紀錄';

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `lockpass`
--
ALTER TABLE `lockpass`
  ADD PRIMARY KEY (`id`),
  ADD KEY `card` (`cardnumber`,`MAC`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `lockpass`
--
ALTER TABLE `lockpass`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
