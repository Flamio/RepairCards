begin transaction;

alter table products
    add column isOwen integer;

CREATE TABLE products_ (id INTEGER PRIMARY KEY AUTOINCREMENT, code CHAR (255), name CHAR (255), isOwen INTEGER);
CREATE TABLE repair_cards_ (id INTEGER PRIMARY KEY NOT NULL, repairer_id INTEGER, product_id INTEGER, client_id INTEGER, receive_date DATE, ready_date DATE, return DATE, state_id INTEGER, complaints CHAR (255), reason CHAR (255), note CHAR (500), bar_code CHAR (20), cost_for_client INT, cost_repair INT, receive_date2 DATE, sendDate DATE, createYear integer, createMonth integer);
CREATE TABLE cards_methods_ (id_card integer, id_method integer, description CHAR (255));

commit;

begin transaction;
INSERT INTO repair_cards_ SELECT id, repairer_id, product_id, client_id, receive_date, ready_date, return, state_id, complaints, reason, note , bar_code, cost_for_client, cost_repair, receive_date2, sendDate, createYear, createMonth FROM repair_cards;
INSERT INTO products_ SELECT id, code, name, 1 FROM products;
INSERT INTO cards_methods_ SELECT * FROM cards_methods;

delete from cards_methods;

commit;
drop table repair_cards;
drop table products;

ALTER TABLE products_ RENAME TO products;
ALTER TABLE repair_cards_ RENAME TO repair_cards;

INSERT INTO cards_methods SELECT * FROM cards_methods_;

drop table cards_methods_;

update products
    set isOwen = 1;

