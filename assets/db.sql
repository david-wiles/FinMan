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
