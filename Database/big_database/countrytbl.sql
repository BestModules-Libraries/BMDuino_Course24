-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
-- 產生時間： 2025 年 09 月 19 日 17:39
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
-- 資料表結構 `countrytbl`
--

CREATE TABLE `countrytbl` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `countryid` char(2) CHARACTER SET ascii NOT NULL COMMENT '國家代碼',
  `cname` char(120) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '國家名稱',
  `ename` char(200) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '英文名稱',
  `state` char(30) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '洲別',
  `crtdatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料產生日期時間'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='全球國家名稱代碼表';


--
-- 資料表索引 `countrytbl`
--
ALTER TABLE `countrytbl`
  ADD PRIMARY KEY (`id`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `countrytbl`
--
ALTER TABLE `countrytbl`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵', AUTO_INCREMENT=0;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
