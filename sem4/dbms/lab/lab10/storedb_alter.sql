alter table shelf_location 
    add constraint fk_shelf_store_id foreign key (store_id) references store(store_id),
    add constraint fk_shelf_product_id foreign key (product_id) references product(product_id);

alter table product_category
    add constraint fk_category_product_id foreign key (product_id) references product(product_id),
    add constraint fk_category_id foreign key (category_id) references category(category_id);

alter table stock
    add constraint fk_stock_store_id foreign key (store_id) references store(store_id),
    add constraint fk_stock_product_id foreign key (product_id) references product(product_id);

alter table bill
    add constraint fk_bill_payment_id foreign key (payment_id) references payment(payment_id);

alter table bill_item
    add constraint fk_bill_id foreign key (bill_id) references bill(bill_id),
    add constraint fk_bill_product_id foreign key (product_id) references product(product_id) on delete cascade;

alter table payment
    add constraint fk_payment_bill_id foreign key (bill_id) references bill(bill_id);

alter table card_payment
    add constraint fk_card_payment_id foreign key (card_payment_id) references payment(payment_id);