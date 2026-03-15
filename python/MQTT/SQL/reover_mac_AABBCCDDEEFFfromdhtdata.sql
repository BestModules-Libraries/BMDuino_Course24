-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2024-12-04 00:07:33
-- 伺服器版本： 10.4.32-MariaDB
-- PHP 版本： 8.2.12

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
-- 資料表結構 `dhtdata`
--

CREATE TABLE `dhtdata` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '用戶端IP',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料輸入時間',
  `temperature` float NOT NULL COMMENT '溫度',
  `humidity` float NOT NULL COMMENT '濕度',
  `systime` varchar(14) NOT NULL COMMENT '使用者更新時間'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

--
-- 傾印資料表的資料 `dhtdata`
--

INSERT INTO `dhtdata` (`MAC`, `IP`, `crtdatetime`, `temperature`, `humidity`, `systime`) VALUES
( 'AABBCCDDEEFF', '140.128.200.100', '2024-04-15 16:51:32', 34, 34, '20200404195342'),
( 'AABBCCDDEEFF', '140.128.200.100', '2024-04-15 16:51:32', 34, 34, '20230323035533'),
( 'AABBCCDDEEFF', '140.128.200.100', '2024-04-15 16:51:32', 34, 34, '20240401105256'),
('AABBCCDDEEFF', '192.168.99.100', '2024-12-03 23:00:55', 24.56, 56.88, '20241204064812');

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `dhtdata`
--
ALTER TABLE `dhtdata`
  ADD PRIMARY KEY (`id`),
  ADD KEY `MAC` (`MAC`,`systime`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `dhtdata`
--
ALTER TABLE `dhtdata`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵', AUTO_INCREMENT=1559;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
