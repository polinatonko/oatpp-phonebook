CREATE TABLE EntryDb (
    id INTEGER PRIMARY KEY,
    name VARCHAR NOT NULL,
    phone VARCHAR UNIQUE NOT NULL,
    address VARCHAR NOT NULL
);

INSERT INTO EntryDb 
(name, phone, address) VALUES ('Ivan', '+375295556688', 'Minsk');

INSERT INTO EntryDb 
(name, phone, address) VALUES ('Olga', '+375295556689', 'Kopyl');

INSERT INTO EntryDb 
(name, phone, address) VALUES ('Boris', '+375295556680', 'Grodno');