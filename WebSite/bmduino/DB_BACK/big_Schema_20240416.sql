-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- 主機： 127.0.0.1
-- 產生時間： 2024-04-16 15:24:46
-- 伺服器版本： 10.4.28-MariaDB
-- PHP 版本： 8.2.4

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
-- 資料表結構 `airpressure`
--

CREATE TABLE `airpressure` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `pressure` double NOT NULL COMMENT '大氣壓力值(hPa)',
  `temperature` double DEFAULT 0 COMMENT '溫度(攝氏溫度)'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `area`
--

CREATE TABLE `area` (
  `mid` int(11) NOT NULL COMMENT '主鍵',
  `sysdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料輸入時間',
  `areaid` varchar(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '區域代碼',
  `areaname` varchar(50) NOT NULL COMMENT '區域名稱'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `bmp`
--

CREATE TABLE `bmp` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置MAC值',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `pressure` double NOT NULL COMMENT '大氣壓力值(hPa)',
  `temperature` double NOT NULL COMMENT '溫度(攝氏溫度)'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `coutytbl`
--

CREATE TABLE `coutytbl` (
  `cid` varchar(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '縣市ID',
  `cname` varchar(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '	縣市名稱'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `cwbsite`
--

CREATE TABLE `cwbsite` (
  `id` int(11) NOT NULL DEFAULT 0 COMMENT '主鍵',
  `dataorder` varchar(14) NOT NULL COMMENT '時間維度',
  `sysdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料更新時間',
  `sid` varchar(20) NOT NULL COMMENT '站台ID',
  `sname` varchar(60) DEFAULT NULL COMMENT '站台名稱',
  `sdatetime` datetime NOT NULL COMMENT '資料時間',
  `lat` double NOT NULL COMMENT '緯度',
  `lon` double NOT NULL COMMENT '經度',
  `hight` int(11) NOT NULL COMMENT '海拔',
  `wdir` int(11) NOT NULL COMMENT '風向',
  `wspeed` int(11) NOT NULL COMMENT '風速',
  `temp` double NOT NULL COMMENT '溫度',
  `humid` double NOT NULL COMMENT '濕度',
  `bar` double NOT NULL COMMENT '氣壓',
  `rain` double NOT NULL COMMENT '雨量',
  `cid` varchar(14) NOT NULL COMMENT '縣市ID',
  `cname` varchar(40) DEFAULT NULL COMMENT '縣市名稱',
  `tid` varchar(14) NOT NULL COMMENT '鄉鎮ID',
  `tname` varchar(60) DEFAULT NULL COMMENT '鄉鎮名稱'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `device`
--

CREATE TABLE `device` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `deviceid` varchar(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `devicename` varchar(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `address` varchar(200) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `longitude` varchar(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '經度',
  `latitude` varchar(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '緯度'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `devicelist`
--

CREATE TABLE `devicelist` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `Did` int(11) NOT NULL,
  `mac` varchar(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `dht`
--

CREATE TABLE `dht` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) NOT NULL COMMENT '裝置MAC值',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `temperature` float NOT NULL COMMENT '攝氏溫度',
  `humidity` float NOT NULL COMMENT '濕度',
  `systime` varchar(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `dhtdata`
--

CREATE TABLE `dhtdata` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '用戶端IP',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料輸入時間',
  `temperature` float NOT NULL COMMENT '溫度',
  `humidity` float NOT NULL COMMENT '濕度',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `dhtdata1`
--

CREATE TABLE `dhtdata1` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `temperature` float NOT NULL COMMENT '溫度',
  `humidity` float NOT NULL COMMENT '濕度',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `diplomatype`
--

CREATE TABLE `diplomatype` (
  `id` char(2) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '學歷代碼',
  `degree` char(30) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '學歷名稱',
  `degreeename` char(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '學歷英文名稱'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `environment`
--

CREATE TABLE `environment` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `dataorder` varchar(14) NOT NULL COMMENT '時間維度',
  `sysdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料更新時間',
  `sid` varchar(20) NOT NULL COMMENT '站台ID',
  `sname` varchar(60) DEFAULT NULL COMMENT '站台名稱',
  `sdatetime` datetime NOT NULL COMMENT '資料時間',
  `lat` double NOT NULL COMMENT '緯度',
  `lon` double NOT NULL COMMENT '經度',
  `hight` int(11) NOT NULL COMMENT '海拔',
  `wdir` int(11) NOT NULL COMMENT '風向',
  `wspeed` int(11) NOT NULL COMMENT '風速',
  `temp` double NOT NULL COMMENT '溫度',
  `humid` double NOT NULL COMMENT '濕度',
  `bar` double NOT NULL COMMENT '氣壓',
  `rain` double NOT NULL COMMENT '雨量',
  `cid` varchar(14) NOT NULL COMMENT '縣市ID',
  `cname` varchar(40) DEFAULT NULL COMMENT '縣市名稱',
  `tid` varchar(14) NOT NULL COMMENT '鄉鎮ID',
  `tname` varchar(60) DEFAULT NULL COMMENT '鄉鎮名稱'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `globalweather`
--

CREATE TABLE `globalweather` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `dataorder` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '時間維度',
  `sysdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料更新時間',
  `sid` varchar(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '站台ID',
  `sname` varchar(60) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '站台名稱',
  `sdatetime` datetime NOT NULL COMMENT '資料時間',
  `timezone` char(10) NOT NULL COMMENT '時區',
  `lat` char(16) NOT NULL COMMENT '緯度',
  `lon` char(16) NOT NULL COMMENT '經度',
  `country` char(10) NOT NULL COMMENT '國家',
  `wdir` int(11) NOT NULL COMMENT '風向',
  `wspeed` double NOT NULL COMMENT '風速',
  `temp` double NOT NULL COMMENT '溫度',
  `humid` double NOT NULL COMMENT '濕度'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `gps`
--

CREATE TABLE `gps` (
  `id` int(11) NOT NULL,
  `mac` varchar(12) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `updatetime` varchar(22) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `latitude` varchar(22) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `longitude` varchar(22) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `hauser`
--

CREATE TABLE `hauser` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `userid` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者帳號',
  `username` char(24) NOT NULL COMMENT '使用者名稱',
  `userkey` char(64) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '使用金鑰',
  `keyactive` int(11) DEFAULT 0 COMMENT '金鑰是否作用中',
  `updatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `crtdatatime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '系統建立時間'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `language`
--

CREATE TABLE `language` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `language` char(12) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '電影語言',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料創立時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `login`
--

CREATE TABLE `login` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `account` varchar(16) NOT NULL COMMENT '登錄帳號',
  `pwd` varchar(25) DEFAULT NULL COMMENT '密碼',
  `ipaddr` varchar(15) NOT NULL COMMENT '登陸ip address',
  `loginin` datetime NOT NULL COMMENT '登錄時間',
  `loginout` datetime NOT NULL COMMENT '登出時間',
  `isguest` tinyint(1) NOT NULL COMMENT '是否遊客',
  `staytime` int(11) NOT NULL COMMENT '登陸時間',
  `accountid` varchar(10) NOT NULL COMMENT '客戶號碼',
  `loginok` tinyint(1) NOT NULL COMMENT '登錄是否成功'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `logtype`
--

CREATE TABLE `logtype` (
  `typeid` char(2) NOT NULL COMMENT '主鍵與類別',
  `typename` char(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '登錄型態內容',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '系統時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `lux`
--

CREATE TABLE `lux` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `luxvalue` float NOT NULL COMMENT '亮度(Lux)',
  `systime` varchar(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `luxday`
--

CREATE TABLE `luxday` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置MAC值',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `cnt` int(11) NOT NULL COMMENT '資料個數',
  `luxvalue` float NOT NULL COMMENT '亮度(Lux)',
  `day` char(8) NOT NULL COMMENT '日期',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `member`
--

CREATE TABLE `member` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `username` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '帳號名稱',
  `name` char(25) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '名字',
  `sex` tinyint(4) NOT NULL COMMENT '性別',
  `diploma` char(2) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '學歷'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `memberdiploma`
--

CREATE TABLE `memberdiploma` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `memberid` int(11) NOT NULL COMMENT '會員編號',
  `diplomatype` char(2) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '學歷種類',
  `name` char(200) NOT NULL COMMENT '學校名稱',
  `graduated` int(11) NOT NULL DEFAULT 1 COMMENT '是否畢業',
  `national` int(11) NOT NULL DEFAULT 1 COMMENT '國立/私立',
  `global` int(11) NOT NULL DEFAULT 0 COMMENT '是否為國外學歷',
  `strdate` char(6) NOT NULL COMMENT '開始年月',
  `strend` char(6) NOT NULL COMMENT '結束年月',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料創立時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `members`
--

CREATE TABLE `members` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `account` char(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '會員帳號',
  `pwd` char(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '會員密碼',
  `name` char(30) NOT NULL COMMENT '會員姓名',
  `ename` char(60) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '英文名字',
  `address` char(120) DEFAULT NULL COMMENT '會員地址',
  `phone` char(14) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '會員手機號碼',
  `tel` char(14) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '會員市內電話',
  `email` varchar(100) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '會員電子信箱',
  `invoice_carrie` char(12) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '電子載具'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `mq3data`
--

CREATE TABLE `mq3data` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `mac` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置網路卡編號',
  `IP` char(20) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料創立時間',
  `mq3` bigint(20) NOT NULL COMMENT '酒精濃度值',
  `sysdatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `mq4`
--

CREATE TABLE `mq4` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `systime` varchar(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `mqvalue` bigint(20) NOT NULL COMMENT '甲烷數值'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `mq7`
--

CREATE TABLE `mq7` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `systime` varchar(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `mqvalue` bigint(20) NOT NULL COMMENT '甲烷數值'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `noise`
--

CREATE TABLE `noise` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` varchar(12) NOT NULL COMMENT '裝置MAC值',
  `IP` char(20) DEFAULT NULL COMMENT '連線IP位址',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `systime` varchar(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `dbvalue` double NOT NULL COMMENT '噪音分貝值'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `ow_citylist`
--

CREATE TABLE `ow_citylist` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '網路卡編號',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料創建時間',
  `sid` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '城市代碼',
  `sname` varchar(60) NOT NULL COMMENT '城市名稱',
  `country` char(16) NOT NULL COMMENT '國碼',
  `state` char(16) NOT NULL COMMENT '省、洲',
  `lon` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '經度',
  `lat` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '緯度'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `ow_curent`
--

CREATE TABLE `ow_curent` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '網路卡編號',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料創建時間',
  `dataorder` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '資料系統時間',
  `sysdatatime` datetime NOT NULL COMMENT '資料擷取時間',
  `timezone` char(8) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '時區',
  `siteid` char(10) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '站台編號',
  `sitename` varchar(60) NOT NULL COMMENT '站台名稱',
  `country` char(6) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '國碼',
  `lon` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '經度',
  `lat` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '緯度',
  `temp` double NOT NULL COMMENT '溫度',
  `temp_min` double NOT NULL COMMENT '最低溫',
  `temp_max` double NOT NULL COMMENT '最高溫',
  `pressure` decimal(10,0) NOT NULL COMMENT '氣壓',
  `humidity` double NOT NULL COMMENT '濕度',
  `windspeed` double NOT NULL COMMENT '風速',
  `winddeg` double NOT NULL COMMENT '風向'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `parkingdata`
--

CREATE TABLE `parkingdata` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `parkId` char(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '停車處ID',
  `parkName` varchar(120) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '停車處名稱',
  `totalSpace` int(11) NOT NULL COMMENT '車格',
  `surplusSpace` int(11) NOT NULL COMMENT '空位車格',
  `payGuide` text CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '停車說明',
  `introduction` varchar(250) NOT NULL COMMENT '停車處介紹',
  `address` varchar(250) NOT NULL COMMENT '停車處住址',
  `wgsX` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT 'X座標',
  `wgsY` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT 'Y座標',
  `areaId` char(4) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '區域代碼',
  `areaName` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '區域名稱',
  `systemdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料創始時間',
  `datatime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '資料更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `particle`
--

CREATE TABLE `particle` (
  `id` int(11) NOT NULL,
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '網路卡號',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料時間',
  `pm1` double NOT NULL COMMENT 'pm1.0',
  `pm25` double NOT NULL COMMENT 'pm2.5',
  `pm10` double NOT NULL COMMENT 'pm10',
  `systime` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `pathtbl`
--

CREATE TABLE `pathtbl` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `mac` varchar(12) NOT NULL COMMENT '裝置MAC值',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() COMMENT '資料輸入時間',
  `updatetime` varchar(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `longitude` varchar(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '經度',
  `latitude` varchar(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '緯度'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `pmbox`
--

CREATE TABLE `pmbox` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `userid` int(11) NOT NULL COMMENT '使用者ID',
  `roomid` int(11) NOT NULL COMMENT '房間ID',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '網路卡編號',
  `boxname` char(100) NOT NULL COMMENT '裝置名稱',
  `crtdatatime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '系統建立時間',
  `updatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `pmboxsensor`
--

CREATE TABLE `pmboxsensor` (
  `mid` int(11) NOT NULL COMMENT '環控盒子主鍵',
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '網路卡編號',
  `sensortype` char(2) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '感測器類別',
  `sensorname` char(100) NOT NULL COMMENT '感測器名稱',
  `crtdatatime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '系統建立時間',
  `active` tinyint(4) DEFAULT NULL COMMENT '是否運作'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `pmdevice`
--

CREATE TABLE `pmdevice` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `status` char(1) NOT NULL COMMENT '狀態',
  `sysdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料更新時間',
  `mac` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置mac',
  `deviceid` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置號碼',
  `devicename` char(25) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '裝置名稱',
  `deviceaddr` varchar(250) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '裝置所在地住址',
  `longitude` double NOT NULL COMMENT '經度',
  `latitude` double NOT NULL COMMENT '緯度',
  `areaid` char(16) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '偵測區域'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='蚊子機裝機所在地';

-- --------------------------------------------------------

--
-- 資料表結構 `sensor`
--

CREATE TABLE `sensor` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `MAC` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '裝置MAC值',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料輸入時間',
  `systime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者更新時間',
  `sensor1` double DEFAULT NULL COMMENT '第一個感測器',
  `sensor2` double DEFAULT NULL COMMENT '第二個感測器',
  `sensor3` double DEFAULT NULL COMMENT '第三個感測器'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `sensortype`
--

CREATE TABLE `sensortype` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `sid` char(2) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '感測器代碼',
  `sname` char(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '感測器名稱',
  `ename` char(32) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '英文名稱',
  `ps` varchar(200) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '備註'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `sextype`
--

CREATE TABLE `sextype` (
  `id` tinyint(4) NOT NULL COMMENT '主鍵',
  `sextypename` char(10) NOT NULL COMMENT '中文性別',
  `sextypeename` char(20) NOT NULL COMMENT '英文性別'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `stocktrans`
--

CREATE TABLE `stocktrans` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `datatime` char(8) NOT NULL COMMENT '資料擷取日期',
  `description` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '解說',
  `stockid` char(6) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '股票代號',
  `trandate` date NOT NULL COMMENT '交易日期',
  `qty` int(11) NOT NULL COMMENT '成交股數',
  `tranprice` bigint(20) NOT NULL COMMENT '成交金額',
  `starprice` double NOT NULL COMMENT '開盤價',
  `highprice` double NOT NULL COMMENT '最高價',
  `lowprice` double NOT NULL COMMENT '最低價',
  `closeprice` double NOT NULL COMMENT '收盤價',
  `lose` double NOT NULL COMMENT '漲跌價差',
  `records` bigint(20) NOT NULL COMMENT '成交筆數',
  `systemdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料建立時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `towntbl`
--

CREATE TABLE `towntbl` (
  `tid` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '鄉鎮ID',
  `tname` varchar(60) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '鄉鎮名稱',
  `cid` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '縣市ID',
  `cname` char(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '縣市名稱'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `user`
--

CREATE TABLE `user` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `user` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '帳號名稱',
  `username` char(30) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '使用者名稱',
  `pwd` char(120) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '密碼',
  `email` char(200) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '電子郵件',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '資料創立時間'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `userlog`
--

CREATE TABLE `userlog` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `userid` int(11) NOT NULL COMMENT '使用者主鍵',
  `type` char(2) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT 'Login型態',
  `logindatetime` char(14) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '登錄日期時間',
  `fromip` char(16) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '使用者來源IP',
  `op` char(200) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '使用者動作內容',
  `crtdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp() COMMENT '自動帶入時間戳記'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 資料表結構 `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL COMMENT '主鍵',
  `account` char(10) NOT NULL DEFAULT '' COMMENT '登錄帳號',
  `password` char(10) NOT NULL DEFAULT '' COMMENT '登錄密碼',
  `name` char(10) NOT NULL DEFAULT '' COMMENT '名字',
  `sex` char(2) NOT NULL DEFAULT '' COMMENT '性別',
  `birthday` date DEFAULT NULL COMMENT '生日',
  `year` tinyint(4) DEFAULT 0 COMMENT '生日(年)',
  `month` tinyint(4) DEFAULT 0 COMMENT '生日(月)',
  `day` tinyint(4) DEFAULT 0 COMMENT '生日(日)',
  `telephone` varchar(20) NOT NULL DEFAULT '' COMMENT '電話',
  `cellphone` char(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL DEFAULT '' COMMENT '手機',
  `address` varchar(50) NOT NULL DEFAULT '' COMMENT '住址',
  `email` varchar(50) NOT NULL DEFAULT '' COMMENT '電子郵件',
  `url` char(50) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT '' COMMENT '個人網站網址',
  `comment` text DEFAULT NULL COMMENT '備註'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- --------------------------------------------------------

--
-- 替換檢視表以便查看 `vuserlog`
-- (請參考以下實際畫面)
--
CREATE TABLE `vuserlog` (
`id` int(11)
,`userid` int(11)
,`crtdatetime` timestamp
,`type` char(2)
,`typename` char(100)
,`logindatetime` char(14)
,`fromip` char(16)
,`op` char(200)
);

-- --------------------------------------------------------

--
-- 替換檢視表以便查看 `vuserlogjob`
-- (請參考以下實際畫面)
--
CREATE TABLE `vuserlogjob` (
`id` int(11)
,`userid` int(11)
,`account` char(10)
,`username` char(10)
,`password` char(10)
,`sex` char(2)
,`birthday` date
,`telephone` varchar(20)
,`cellphone` char(20)
,`address` varchar(50)
,`email` varchar(50)
,`url` char(50)
,`comment` text
,`logtypeid` char(2)
,`logtypename` char(100)
,`logindatetime` char(14)
,`ip` char(16)
,`job` char(200)
);

-- --------------------------------------------------------

--
-- 資料表結構 `wind`
--

CREATE TABLE `wind` (
  `id` int(11) NOT NULL,
  `sysdatetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `speed` float NOT NULL COMMENT '風速',
  `way` int(12) NOT NULL COMMENT '風向',
  `temp` float NOT NULL COMMENT '溫度',
  `humid` float NOT NULL COMMENT '濕度',
  `IP` char(20) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT '連上IP位址',
  `DT` char(12) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL COMMENT 'YYYYMMDDHHMM',
  `mac` char(16) CHARACTER SET ascii COLLATE ascii_general_ci DEFAULT NULL COMMENT '連上網卡號碼'
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci COMMENT='吳厝國小樹屋風速資訊表';

-- --------------------------------------------------------

--
-- 檢視表結構 `vuserlog`
--
DROP TABLE IF EXISTS `vuserlog`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW `vuserlog`  AS SELECT `a`.`id` AS `id`, `a`.`userid` AS `userid`, `a`.`crtdatetime` AS `crtdatetime`, `a`.`type` AS `type`, `b`.`typename` AS `typename`, `a`.`logindatetime` AS `logindatetime`, `a`.`fromip` AS `fromip`, `a`.`op` AS `op` FROM (`userlog` `a` join `logtype` `b`) WHERE convert(`a`.`type` using utf8mb4) = `b`.`typeid` ORDER BY `a`.`userid` ASC, `a`.`crtdatetime` ASC ;

-- --------------------------------------------------------

--
-- 檢視表結構 `vuserlogjob`
--
DROP TABLE IF EXISTS `vuserlogjob`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW `vuserlogjob`  AS SELECT `a`.`id` AS `id`, `a`.`userid` AS `userid`, `b`.`account` AS `account`, `b`.`name` AS `username`, `b`.`password` AS `password`, `b`.`sex` AS `sex`, `b`.`birthday` AS `birthday`, `b`.`telephone` AS `telephone`, `b`.`cellphone` AS `cellphone`, `b`.`address` AS `address`, `b`.`email` AS `email`, `b`.`url` AS `url`, `b`.`comment` AS `comment`, `a`.`type` AS `logtypeid`, `c`.`typename` AS `logtypename`, `a`.`logindatetime` AS `logindatetime`, `a`.`fromip` AS `ip`, `a`.`op` AS `job` FROM ((`userlog` `a` left join `users` `b` on(`a`.`userid` = `b`.`id`)) join `logtype` `c`) WHERE convert(`a`.`type` using utf8mb4) = `c`.`typeid` ;

--
-- 已傾印資料表的索引
--

--
-- 資料表索引 `airpressure`
--
ALTER TABLE `airpressure`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `area`
--
ALTER TABLE `area`
  ADD PRIMARY KEY (`mid`),
  ADD UNIQUE KEY `areaid` (`areaid`);

--
-- 資料表索引 `bmp`
--
ALTER TABLE `bmp`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `coutytbl`
--
ALTER TABLE `coutytbl`
  ADD PRIMARY KEY (`cid`);

--
-- 資料表索引 `cwbsite`
--
ALTER TABLE `cwbsite`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `sid` (`sid`);

--
-- 資料表索引 `device`
--
ALTER TABLE `device`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `devicelist`
--
ALTER TABLE `devicelist`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `Did` (`Did`,`mac`);

--
-- 資料表索引 `dht`
--
ALTER TABLE `dht`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `dhtdata`
--
ALTER TABLE `dhtdata`
  ADD PRIMARY KEY (`id`),
  ADD KEY `MAC` (`MAC`,`systime`);

--
-- 資料表索引 `dhtdata1`
--
ALTER TABLE `dhtdata1`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `diplomatype`
--
ALTER TABLE `diplomatype`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `environment`
--
ALTER TABLE `environment`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `checkduplicate` (`sid`,`sdatetime`);

--
-- 資料表索引 `globalweather`
--
ALTER TABLE `globalweather`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `gps`
--
ALTER TABLE `gps`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `hauser`
--
ALTER TABLE `hauser`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `accountid` (`userid`);

--
-- 資料表索引 `language`
--
ALTER TABLE `language`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `login`
--
ALTER TABLE `login`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `logtype`
--
ALTER TABLE `logtype`
  ADD PRIMARY KEY (`typeid`);

--
-- 資料表索引 `lux`
--
ALTER TABLE `lux`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `luxday`
--
ALTER TABLE `luxday`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `member`
--
ALTER TABLE `member`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `memberdiploma`
--
ALTER TABLE `memberdiploma`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `s1` (`memberid`,`diplomatype`);

--
-- 資料表索引 `members`
--
ALTER TABLE `members`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `mq3data`
--
ALTER TABLE `mq3data`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `mq4`
--
ALTER TABLE `mq4`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `mq7`
--
ALTER TABLE `mq7`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `noise`
--
ALTER TABLE `noise`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `ow_citylist`
--
ALTER TABLE `ow_citylist`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `ow_curent`
--
ALTER TABLE `ow_curent`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `parkingdata`
--
ALTER TABLE `parkingdata`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `parkId` (`parkId`);

--
-- 資料表索引 `particle`
--
ALTER TABLE `particle`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `dataview` (`MAC`,`systime`);

--
-- 資料表索引 `pathtbl`
--
ALTER TABLE `pathtbl`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `pmbox`
--
ALTER TABLE `pmbox`
  ADD PRIMARY KEY (`id`),
  ADD KEY `userid` (`userid`),
  ADD KEY `roomid` (`roomid`);

--
-- 資料表索引 `pmboxsensor`
--
ALTER TABLE `pmboxsensor`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `pmdevice`
--
ALTER TABLE `pmdevice`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `checkid` (`deviceid`,`status`),
  ADD KEY `deviceid` (`deviceid`) USING BTREE,
  ADD KEY `mac` (`mac`) USING BTREE;

--
-- 資料表索引 `sensor`
--
ALTER TABLE `sensor`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `sensortype`
--
ALTER TABLE `sensortype`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `sid` (`sid`);

--
-- 資料表索引 `sextype`
--
ALTER TABLE `sextype`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `stocktrans`
--
ALTER TABLE `stocktrans`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `towntbl`
--
ALTER TABLE `towntbl`
  ADD PRIMARY KEY (`tid`);

--
-- 資料表索引 `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `checkuser` (`user`);

--
-- 資料表索引 `userlog`
--
ALTER TABLE `userlog`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- 資料表索引 `wind`
--
ALTER TABLE `wind`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `ckduplicate` (`mac`,`DT`),
  ADD KEY `dt` (`DT`);

--
-- 在傾印的資料表使用自動遞增(AUTO_INCREMENT)
--

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `airpressure`
--
ALTER TABLE `airpressure`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `bmp`
--
ALTER TABLE `bmp`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `device`
--
ALTER TABLE `device`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `devicelist`
--
ALTER TABLE `devicelist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `dht`
--
ALTER TABLE `dht`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `dhtdata`
--
ALTER TABLE `dhtdata`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `dhtdata1`
--
ALTER TABLE `dhtdata1`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `environment`
--
ALTER TABLE `environment`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `globalweather`
--
ALTER TABLE `globalweather`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `gps`
--
ALTER TABLE `gps`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `hauser`
--
ALTER TABLE `hauser`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `language`
--
ALTER TABLE `language`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `login`
--
ALTER TABLE `login`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `lux`
--
ALTER TABLE `lux`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `luxday`
--
ALTER TABLE `luxday`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `member`
--
ALTER TABLE `member`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `memberdiploma`
--
ALTER TABLE `memberdiploma`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `members`
--
ALTER TABLE `members`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `mq3data`
--
ALTER TABLE `mq3data`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `mq4`
--
ALTER TABLE `mq4`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `mq7`
--
ALTER TABLE `mq7`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `noise`
--
ALTER TABLE `noise`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `ow_citylist`
--
ALTER TABLE `ow_citylist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `ow_curent`
--
ALTER TABLE `ow_curent`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `parkingdata`
--
ALTER TABLE `parkingdata`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `particle`
--
ALTER TABLE `particle`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `pathtbl`
--
ALTER TABLE `pathtbl`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `pmbox`
--
ALTER TABLE `pmbox`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `pmboxsensor`
--
ALTER TABLE `pmboxsensor`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `sensor`
--
ALTER TABLE `sensor`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `sensortype`
--
ALTER TABLE `sensortype`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `sextype`
--
ALTER TABLE `sextype`
  MODIFY `id` tinyint(4) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `user`
--
ALTER TABLE `user`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `userlog`
--
ALTER TABLE `userlog`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';

--
-- 使用資料表自動遞增(AUTO_INCREMENT) `wind`
--
ALTER TABLE `wind`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
