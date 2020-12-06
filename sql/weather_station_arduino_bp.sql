-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Erstellungszeit: 06. Dez 2020 um 16:14
-- Server-Version: 10.4.11-MariaDB
-- PHP-Version: 7.4.5

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Datenbank: `weather_station_arduino`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `arduino_station_weather_hwid`
--

CREATE TABLE `arduino_station_weather_hwid` (
  `ID_PRIMARY` int(11) NOT NULL,
  `HWID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `arduino_station_weather_information`
--

CREATE TABLE `arduino_station_weather_information` (
  `ID_PRIMARY` int(11) NOT NULL,
  `HWID` int(11) NOT NULL COMMENT 'The hardware ID of the device that send an request',
  `UNIX_TIMESTAMP` float NOT NULL COMMENT 'The time the server received the query',
  `TEMP` float NOT NULL COMMENT 'input temperature',
  `HUMIDITY` float NOT NULL COMMENT 'input humidity',
  `LPG` float NOT NULL COMMENT 'input lpg',
  `CO` float NOT NULL COMMENT 'input CO',
  `SMOKE` float NOT NULL COMMENT 'input smoke'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Indizes der exportierten Tabellen
--

--
-- Indizes für die Tabelle `arduino_station_weather_hwid`
--
ALTER TABLE `arduino_station_weather_hwid`
  ADD PRIMARY KEY (`ID_PRIMARY`);

--
-- Indizes für die Tabelle `arduino_station_weather_information`
--
ALTER TABLE `arduino_station_weather_information`
  ADD PRIMARY KEY (`ID_PRIMARY`);

--
-- AUTO_INCREMENT für exportierte Tabellen
--

--
-- AUTO_INCREMENT für Tabelle `arduino_station_weather_hwid`
--
ALTER TABLE `arduino_station_weather_hwid`
  MODIFY `ID_PRIMARY` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT für Tabelle `arduino_station_weather_information`
--
ALTER TABLE `arduino_station_weather_information`
  MODIFY `ID_PRIMARY` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
