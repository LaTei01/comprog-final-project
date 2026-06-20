#ifndef ORDERS_H
#define ORDERS_H

#include "structs.h"

// ########## ORDER MANAGEMENT FUNCTION DECLARATIONS ##########

// Displays the orders sub-menu and dispatches to the appropriate function
// Loops until the user chooses to go back. Returns 0 on exit
int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);

// Asks for a product ID and desired quantity, then reports whether stock
// is sufficient without modifying anything. Pure check, no side effects
void validateStockAvailability(Products* PRODUCTS_LIST);

// Accepts and processes ONE customer order in a single pass: asks for customer name, product ID, and quantity; validates stock;
// asks for confirmation; deducts stock; appends an Orders record to ORDERS_LIST (the order history); writes receipt.txt; and appends to transactions.txt
void placeOrder(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);

// Prints every completed order on record in ORDERS_LIST (customer, item, quantity, total)
// For example is the in-memory order history for this session plus whatever was loaded, with a grand total
void displayOrderDetails(Orders* ORDERS_LIST);

// Reads and prints the full append-only transactions.txt log every completed order across all sessions, each with its own timestamp
void showTransactionHistory();

#endif // ORDERS_H