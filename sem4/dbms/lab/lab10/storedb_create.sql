create table store(
    store_id int auto_increment primary key,
    store_name varchar(50) not null,
    address varchar(50) not null
);

create table shelf_location(
    shelf_location_id int auto_increment primary key,
    store_id int not null,
    product_id int not null,
    floor_number int not null,
    aisle_name varchar(50) not null,
    shelf_number int not null
);

create table product(
    product_id int auto_increment primary key,
    sku int unique not null,
    product_name varchar(50) not null,
    price int not null
);

create table category(
    category_id int auto_increment primary key,
    category_name varchar(50) not null,
    category_desc varchar(50) not null
);

create table product_category(
    product_category_id int auto_increment primary key,
    product_id int not null,
    category_id int not null,
    constraint product_category_unique unique(product_id, category_id)   
);

create table stock(
    stock_id int auto_increment primary key,
    store_id int not null,
    product_id int not null,
    shelf_qty int not null,
    record_threshold int not null,
    storage_qty int not null,
    stock_date date not null,
    constraint stock_unique unique(store_id, product_id)
);  

create table bill(
    bill_id int auto_increment primary key,
    payment_id int ,
    bill_date date not null,
    total_bill int not null
);

create table bill_item(
    bill_item_id int auto_increment primary key,
    bill_id int not null,
    product_id int not null,
    quantity int not null,
    total_price int not null
);

create table payment(
    payment_id int auto_increment primary key,
    bill_id int not null,
    payment_type varchar(50) not null,
    payment_date date not null,
    payment_amount int not null
);

create table card_payment(
    card_payment_id int auto_increment primary key,
    card_number int not null,
    expiry_date date not null,
    auth_reference_number int not null
);
