--
-- Файл сгенерирован с помощью SQLiteStudio v3.2.1 в чт апр. 18 17:58:31 2019
--
-- Использованная кодировка текста: UTF-8
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Таблица: cards_methods
CREATE TABLE cards_methods (id_card REFERENCES repair_cards (id), id_method REFERENCES methods (id), description CHAR (255));

-- Таблица: clients
CREATE TABLE clients (id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR (255) NOT NULL, address CHAR (255) NOT NULL, post_index CHAR (50), phone CHAR (100), fax CHAR (100), email CHAR (100), www CHAR (100), person CHAR (100), note CHAR (255));

-- Таблица: methods
CREATE TABLE methods (id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR (255) UNIQUE NOT NULL);

-- Таблица: products
CREATE TABLE products (id INTEGER PRIMARY KEY AUTOINCREMENT, code CHAR (255) NOT NULL UNIQUE, name CHAR (255) NOT NULL);

-- Таблица: repair_cards
CREATE TABLE repair_cards (id INTEGER PRIMARY KEY NOT NULL, repairer_id INTEGER NOT NULL REFERENCES repairers (id), product_id INTEGER NOT NULL REFERENCES products (id), client_id INTEGER NOT NULL REFERENCES clients (id), receive_date DATE, ready_date DATE, return DATE, state_id INTEGER NOT NULL REFERENCES states (id), complaints CHAR (255), reason CHAR (255), note CHAR (500), bar_code CHAR (20) NOT NULL, cost_for_client INT, cost_repair INT, receive_date2 DATE, sendDate DATE);

-- Таблица: repairers
CREATE TABLE repairers (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name CHAR (255) NOT NULL UNIQUE);

-- Таблица: states
CREATE TABLE states (id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR (255) UNIQUE);

COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
