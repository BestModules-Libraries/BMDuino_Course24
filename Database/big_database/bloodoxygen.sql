-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2025-09-19 11:36:35
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
-- 資料表結構 `bloodoxygen`
--

CREATE TABLE `bloodoxygen` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料建立時間',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '資料日期時間',
  `MAC` char(12) CHARACTER SET ascii NOT NULL COMMENT '裝置編號',
  `IP` char(20) CHARACTER SET ascii DEFAULT NULL COMMENT '連線IP Address',
  `blood` float NOT NULL COMMENT '血氧資料',
  `heartbeat` int(11) NOT NULL COMMENT '心跳資料'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='量測血氧資料檔';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
