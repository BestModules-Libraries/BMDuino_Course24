-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
-- 產生時間： 2025 年 10 月 18 日 10:16
-- 伺服器版本： 5.5.68-MariaDB
-- PHP 版本： 8.2.27

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
-- 資料表結構 `bodytemp`
--

CREATE TABLE `bodytemp` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii NOT NULL COMMENT '裝置編號',
  `crtdatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料創造時間',
  `IP` char(20) CHARACTER SET ascii NOT NULL COMMENT '連線IP位址',
  `bodytemperature` float NOT NULL COMMENT '體溫',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '資料時間'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='體溫資料表';


ALTER TABLE `bodytemp`
  ADD PRIMARY KEY (`id`),
  ADD KEY `systimeMAC` (`MAC`,`systime`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
