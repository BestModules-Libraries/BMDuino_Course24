-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2025-09-19 11:54:27
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
-- 資料表結構 `diplomatype`
--

CREATE TABLE `diplomatype` (
  `id` char(2) CHARACTER SET ascii NOT NULL COMMENT '學歷代碼',
  `degree` char(30) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '學歷名稱',
  `degreeename` char(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '學歷英文名稱'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='學歷代碼表';
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
