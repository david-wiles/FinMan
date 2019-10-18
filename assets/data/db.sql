create table account
(
    acct_num  integer not null
        constraint account_pk
            primary key,
    owner     text    not null,
    custodian text    not null,
    balance   decimal default 0 not null,
    type      text    not null,
    interest  decimal default 0
);

create unique index account_acct_num_uindex
    on account (acct_num);

create table asset
(
    id    integer not null
        constraint asset_pk
            primary key autoincrement,
    value decimal not null,
    name  text    not null,
    type  text    not null
);

create unique index asset_id_uindex
    on asset (id);

create table auth_user
(
    username  text not null
        constraint auth_user_pk
            primary key,
    pass_hash text not null,
    family_id integer
);

create unique index auth_user_username_uindex
    on auth_user (username);

create table budget
(
    id          integer not null
        constraint budget_pk
            primary key autoincrement,
    user        text    not null
        references auth_user
            on update cascade on delete cascade,
    goal_acct   integer
        references account
            on update cascade on delete cascade,
    goal_amount decimal,
    description text    not null
);

create unique index budget_id_uindex
    on budget (id);

create table debt
(
    id            integer   not null
        constraint debt_pk
            primary key autoincrement,
    principal     decimal   not null,
    interest      decimal   not null,
    start_date    timestamp default current_timestamp not null,
    maturity_date timestamp not null,
    type          text      not null,
    from_acct     integer   not null
        references account
            on update cascade on delete set null
);

create unique index debt_id_uindex
    on debt (id);

create table family
(
    id    integer not null
        constraint family_pk
            primary key autoincrement,
    owner text    not null
        references auth_user
            on update cascade on delete cascade
);

alter table auth_user
    add foreign key(family_id) references family
    on update cascade on delete set null;

create unique index family_id_uindex
    on family (id);

create table income
(
    id            integer not null
        constraint income_pk
            primary key autoincrement,
    hours         decimal default 0 not null,
    amount        decimal not null,
    pay_frequency integer default 1 not null,
    to_acct       integer not null
        references account
            on update cascade on delete cascade,
    owner         text    not null
        references auth_user
            on update cascade on delete cascade
);

create unique index income_id_uindex
    on income (id);

create table investment
(
    id         integer not null
        constraint investment_pk
            primary key autoincrement,
    type       text    not null,
    ticker     text    not null,
    buy_price  decimal not null,
    num_shares decimal not null,
    buy_date   timestamp default current_timestamp not null,
    acct       integer
        references account
            on update cascade on delete cascade,
    owner      text    not null
        references auth_user
            on update cascade on delete cascade
);

create unique index investment_id_uindex
    on investment (id);

create table "transaction"
(
    id        integer not null
        constraint transaction_pk
            primary key autoincrement,
    type      text    not null,
    amount    decimal   default 0 not null,
    from_acct integer
        references account
            on update cascade on delete cascade,
    to_acct   integer
        references account
            on update cascade on delete cascade,
    date      timestamp default current_timestamp not null
);