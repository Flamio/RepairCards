begin transaction;

alter table repair_cards
    add column isOwen integer;

alter table repair_cards
    add column createYear integer;

alter table repair_cards
    add column createMonth integer;

commit transaction;
