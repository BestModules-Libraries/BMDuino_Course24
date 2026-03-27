-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
-- 產生時間： 2025 年 07 月 25 日 16:00
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
-- 資料表結構 `site`
--

CREATE TABLE `site` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `areaid` varchar(16) NOT NULL COMMENT '區域主鍵號碼',
  `siteid` char(16) NOT NULL COMMENT '區域代碼',
  `sitename` varchar(80) NOT NULL COMMENT '站台名稱',
  `address` varchar(200) NOT NULL COMMENT '站台位置住址',
  `longitude` char(22) DEFAULT NULL COMMENT '經度',
  `latitude` char(22) DEFAULT NULL COMMENT '緯度'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- 傾印資料表的資料 `site`
--

INSERT INTO `site` (`id`, `areaid`, `siteid`, `sitename`, `address`, `longitude`, `latitude`) VALUES
(1, 'NANTOU', 'NCNUCST01', '國立暨南國際大學科技學院科一館412研究室', '南投縣埔里鎮大學路1號', '120.930743', '23.952283'),
(12, 'TAICHUNG', 'WTES', '清水區吳厝國小', '臺中市清水區吳厝路35號', '120.598321', '24.268216'),
(13, 'NEWTPE', 'xinheES', '新店區新和國小', '新北市新店區安和路三段100號', '121.518435', '24.985529'),
(11, 'TAICHUNG', 'puCS', '靜宜大學資訊學院', '433台中市沙鹿區台灣大道七段200號', '120.583338334', '24.2272643877'),
(17, 'NEWTPE', 'xinhe152C', '新店區新和國小(20230420建站)', '新北市新店區安和路三段100號', '121.518435', '24.985529'),
(23, 'TAINAN', 'YUANLIN3a', 'dddddddddddddd', '彰化縣員林市員東路二段490號', '23.95766373873022', '120.57701330827756'),
(24, 'KIMEN', 'YUANLIN5', '永忠站台', '彰化縣員林市員東路二段490號', '23.95766373873022', '120.57701330827756');

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `site`
--
ALTER TABLE `site`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `siteid` (`siteid`) USING BTREE;

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `site`
--
ALTER TABLE `site`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵', AUTO_INCREMENT=25;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
