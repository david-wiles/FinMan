create table account
(
    id        integer not null
        constraint account_pk
            primary key autoincrement,
    acct_num  integer     not null,
    owner     text    not null,
    custodian text,
    amount    integer default 0 not null,
    type      text    not null,
    interest  numeric,
    name      text
);

create unique index account_acct_num_uindex
    on account (acct_num);

create table auth_user
(
    id        integer not null
        constraint auth_user_pk
            primary key autoincrement,
    username  text    not null,
    pass_hash text    not null
);

create unique index auth_user_username_uindex
    on auth_user (username);

create table income
(
    id        integer not null
        constraint income_pk
            primary key autoincrement,
    person    integer not null,
    amount    numeric default 0 not null,
    frequency integer default 1 not null,
    type      text    not null,
    to_acct   integer not null,
    hours     integer
);

create table "transaction"
(
    type      text    not null,
    id        integer not null
        constraint transaction_pk
            primary key autoincrement,
    amount    integer default 0 not null,
    from_acct integer not null,
    to_acct   integer,
    date      date    default current_date not null
);

