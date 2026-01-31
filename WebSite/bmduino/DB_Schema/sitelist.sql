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
-- 資料表結構 `sitelist`
--

CREATE TABLE `sitelist` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `Did` int(11) NOT NULL COMMENT '該裝置隸屬於哪一個客戶端',
  `systime` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料更新時間',
  `mac` char(12) NOT NULL COMMENT '客戶端裝置之MAC網卡編號',
  `sensortype` char(2) DEFAULT NULL COMMENT '感測器類別',
  `datatbl` char(30) DEFAULT NULL COMMENT '對應資料表',
  `ps` varchar(300) DEFAULT NULL COMMENT '備註'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- 傾印資料表的資料 `sitelist`
--

INSERT INTO `sitelist` (`id`, `Did`, `systime`, `mac`, `sensortype`, `datatbl`, `ps`) VALUES
(24, 13, '2022-06-18 07:58:30', 'CC50E3B5BB20', '32', 'airpressure', '新和國小大氣壓力感測器'),
(35, 11, '2022-06-24 17:32:14', 'AABBCCDDEEFF', '64', 'mq7', '院長室空氣盒子_一氧化碳感測器'),
(32, 11, '2022-06-13 20:08:32', '441793F001AC', '01', 'dhtData', '院長室空氣盒子_溫溼度感測器'),
(33, 11, '2022-06-18 07:58:24', '441793F001AC', '32', 'airpressure', '院長室空氣盒子_大氣壓力感測器'),
(17, 1, '2021-06-19 08:42:30', 'AABBCCDDEEFF', '63', 'mq4', '暨南大學科一館412室'),
(36, 11, '2022-06-24 17:54:00', 'A4CF125D2F8C', '41', 'noise', '院長室空氣盒子_噪音感測器'),
(20, 12, '2021-06-19 08:42:30', '246F289E432C', '31', 'wind', '吳厝國小樹屋氣象站'),
(21, 13, '2021-06-19 08:42:30', 'CC50E3B5BB20', '31', 'wind', '新北勢新和國小 王漢卿老師 教學氣象站'),
(23, 13, '2021-06-19 08:42:30', 'CC50E3B5BB20', '12', 'lux', '新北市新和國小大氣壓力監控'),
(25, 13, '2021-06-19 08:42:30', 'CC50E3B5BB20', '01', 'dhtData', '溫溼度'),
(26, 1, '2022-03-29 12:20:01', '30AEA44144B8', '31', 'wind', 'aa'),
(34, 11, '2022-06-24 08:33:19', '441793F001AC', '03', 'particle', '院長室空氣盒子_空汙粒子感測器'),
(43, 17, '2023-04-23 11:17:48', '34945435152C', '01', 'dhtData', '20230420_溫度_溼度'),
(42, 17, '2023-04-23 11:15:37', '34945435152C', '31', NULL, '20230420_風速_風向'),
(41, 17, '2023-04-23 11:14:41', '34945435152C', '32', NULL, '20230420_大氣壓力'),
(44, 17, '2023-04-23 11:18:23', '34945435152C', '12', NULL, '20230420_光照度');

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `sitelist`
--
ALTER TABLE `sitelist`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `Did` (`Did`,`mac`,`sensortype`) USING BTREE;

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `sitelist`
--
ALTER TABLE `sitelist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵', AUTO_INCREMENT=45;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
