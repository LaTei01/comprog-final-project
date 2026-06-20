#ifndef INVENTORY_H
#define INVENTORY_H

#include "structs.h"

// ########## INVENTORY MANAGEMENT FUNCTION DECLARATIONS ##########

// Displays the inventory sub-menu and dispatches to the appropriate function
// Returns 0 when the user chooses to go back
int ManageInventory(Products** PRODUCTS_LIST);

// Searches the list by name, brand, or type (case-insensitive substring match)
// Products with itemStock == 0 are tagged "(OUT OF STOCK)" in the results
void searchProduct(Products** PRODUCTS_LIST);

// Adds a new product node to the tail of the circular doubly linked list
// Checks for duplicate names before inserting and auto-assigns itemIndex
void addProduct(Products** PRODUCTS_LIST);

// Locates a product by itemIndex and updates any non-empty fields provided
void updateProduct(Products** PRODUCTS_LIST);

// Locates a product by itemIndex, confirms with the user, then unlinks and deletes the node
// Adjusts head pointer if needed
void deleteProduct(Products** PRODUCTS_LIST);

// Prints full details (ID, name, brand, type, price, stock) for every product
// Products with itemStock == 0 are tagged "(OUT OF STOCK)"
void displayProductsDetails(Products* PRODUCTS_LIST);

// Prints products grouped by type: Lip Product, Eye Product, Skin Product, Others
void displayInventory(Products* PRODUCTS_LIST);

// Scans the list and prints any products whose stock is at or below the LOW_STOCK_THRESHOLD defined in inventory.cpp
void checkLowStock(Products* PRODUCTS_LIST);

#endif // INVENTORY_H