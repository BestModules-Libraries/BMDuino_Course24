-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
-- 產生時間： 2025 年 09 月 19 日 17:44
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
-- 資料庫： `ncnuiot`
--

-- --------------------------------------------------------

--
-- 資料表結構 `coutytbl`
--

CREATE TABLE `coutytbl` (
  `cid` char(2) CHARACTER SET ascii NOT NULL COMMENT '縣市ID',
  `cname` char(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '縣市名稱'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='台灣縣市資料代碼表';


--
-- 資料表索引 `coutytbl`
--
ALTER TABLE `coutytbl`
  ADD PRIMARY KEY (`cid`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
