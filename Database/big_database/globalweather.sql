-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2025-09-19 12:00:21
-- 伺服器版本： 10.4.22-MariaDB
-- PHP 版本： 8.1.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 資料庫: `big`
--

-- --------------------------------------------------------

--
-- 資料表結構 `globalweather`
--

CREATE TABLE `globalweather` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `dataorder` char(14) CHARACTER SET ascii NOT NULL COMMENT '時間維度',
  `sysdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料更新時間',
  `sid` varchar(20) CHARACTER SET ascii NOT NULL COMMENT '站台ID',
  `sname` varchar(60) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '站台名稱',
  `sdatetime` datetime NOT NULL COMMENT '資料時間',
  `timezone` char(10) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '時區',
  `lat` char(16) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '緯度',
  `lon` char(16) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '經度',
  `country` char(10) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '國家',
  `wdir` int(11) NOT NULL COMMENT '風向',
  `wspeed` double NOT NULL COMMENT '風速',
  `temp` double NOT NULL COMMENT '溫度',
  `humid` double NOT NULL COMMENT '濕度'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='openweather氣象資料表';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
