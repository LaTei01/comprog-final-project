#ifndef ORDERS_H
#define ORDERS_H

#include "structs.h"

// ########## ORDER MANAGEMENT FUNCTION DECLARATIONS ##########

// Displays the orders sub-menu and dispatches to the appropriate function.
// Loops until the user chooses to go back. Returns 0 on exit.
int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);

// Prints all orders currently in the ORDERS_LIST (customer, item, qty, total).
void showCustomerOrders(Orders* ORDERS_LIST);

// Asks for a product ID and desired quantity, then reports whether stock
// is sufficient without modifying anything.
void validateStockAvailability(Products* PRODUCTS_LIST);

// Adds a new order node to the ORDERS_LIST.
// Validates that the requested quantity does not exceed available stock.
void addToCart(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);

// Displays the full cart, asks for confirmation, deducts stock, saves a
// receipt, persists inventory, then clears and frees the ORDERS_LIST.
void viewAndCheckout(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);

// Prints a formatted summary of all orders in ORDERS_LIST.
void displayOrderDetails(Orders* ORDERS_LIST);

#endif // ORDERS_H