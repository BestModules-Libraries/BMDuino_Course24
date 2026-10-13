-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： localhost
-- 產生時間： 2025 年 04 月 17 日 23:08
-- 伺服器版本： 5.5.68-MariaDB
-- PHP 版本： 8.2.21

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
-- 資料表結構 `sonic`
--

CREATE TABLE `sonic` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '連線IP網址',
  `crtdatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料輸入時間',
  `distance` float NOT NULL COMMENT '距離值',
  `systime` char(14) CHARACTER SET ascii NOT NULL COMMENT '使用者更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='溫溼度感測器資料';

--
-- 傾印資料表的資料 `sonic`
--

INSERT INTO `sonic` (`id`, `MAC`, `IP`, `crtdatetime`, `distance`, `systime`) VALUES
(1, 'AABBCCDDEEFF', '114.33.165.41', '2025-04-17 14:53:15', 45.56, '20250417222101'),
(2, 'AABBCCDDEEFF', '114.33.165.41', '2025-04-17 14:53:13', 45.6, '20250417222847'),
(3, 'AABBCCDDEEFF', '114.33.165.41', '2025-04-17 14:53:12', 34.45, '20250417222947'),
(4, 'AABBCCDDEEFF', '114.33.165.41', '2025-04-17 14:53:07', 34.45, '20250417222950'),
(5, 'AABBCCDDEEFF', '114.33.165.41', '2025-04-17 14:52:49', 34.45, '20250417225249'),
(6, 'AABBCCDDEEFF', '114.33.165.41', '2025-04-17 15:07:32', 34.45, '20250417230732');

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `sonic`
--
ALTER TABLE `sonic`
  ADD PRIMARY KEY (`id`),
  ADD KEY `mac` (`MAC`,`systime`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `sonic`
--
ALTER TABLE `sonic`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵', AUTO_INCREMENT=7;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
