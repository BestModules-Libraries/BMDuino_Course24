-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2025-09-19 11:55:11
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
-- 資料表結構 `dhtdata`
--

CREATE TABLE `dhtdata` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) CHARACTER SET ascii DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料輸入時間',
  `temperature` float NOT NULL COMMENT '溫度',
  `humidity` float NOT NULL COMMENT '濕度',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '使用者更新時間'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='溫溼度資料表';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
