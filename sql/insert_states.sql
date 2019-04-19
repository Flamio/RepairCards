--
-- Файл сгенерирован с помощью SQLiteStudio v3.2.1 в пт апр. 19 11:36:06 2019
--
-- Использованная кодировка текста: UTF-8
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

INSERT INTO states (id, name) VALUES (1, 'готов');
INSERT INTO states (id, name) VALUES (2, 'возвращен');
INSERT INTO states (id, name) VALUES (3, 'в ремонте');
INSERT INTO states (id, name) VALUES (4, 'отправлен');

COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
