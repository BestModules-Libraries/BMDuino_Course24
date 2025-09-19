-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2025-09-19 11:37:42
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
-- 資料表結構 `bmp`
--

CREATE TABLE `bmp` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii NOT NULL COMMENT '裝置MAC值',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '使用者更新時間',
  `pressure` double NOT NULL COMMENT '大氣壓力值(hPa)',
  `temperature` double NOT NULL COMMENT '溫度(攝氏溫度)'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='氣壓資料表';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
