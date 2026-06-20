#include "orders.h"
#include "filehandling.h"

#include <iostream>
#include <fstream>
using namespace std;

// ########## ORDER MANAGEMENT ##########

int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    int orderChoice;

    // Loop replaces recursion, preventing stack overflow on long sessions
    while (true) {
        cout << "\n########## Order Management ##########" << endl;
        cout << "1. Validate Stock Availability" << endl;
        cout << "2. Place Order"                 << endl;
        cout << "3. Display Order Details"       << endl;
        cout << "4. Transaction History"         << endl;
        cout << "5. Back to Main Menu"           << endl;
        cout << "Enter your choice: ";

        if (!(cin >> orderChoice)) {    // Handle non-integer input
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (orderChoice) {
            case 1: validateStockAvailability(*PRODUCTS_LIST);    break;
            case 2: placeOrder(PRODUCTS_LIST, ORDERS_LIST);       break;
            case 3: displayOrderDetails(*ORDERS_LIST);            break;
            case 4: showTransactionHistory();                     break;
            case 5: return 0;
            default: cout << "Invalid choice. Please enter 1 - 5." << endl; break;
        }
    }
}

void validateStockAvailability(Products* PRODUCTS_LIST) {

    if (PRODUCTS_LIST == nullptr) { // Check if there are no products yet in the list
        cout << "No products available." << endl;
        return;
    }

    int targetId, quantity;

    cout << "Enter Product ID: ";
    if (!(cin >> targetId) || targetId <= 0) {  // Reject non-integer or non-positive ID
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid ID. Must be a positive whole number." << endl;
        return;
    }

    cout << "Enter desired quantity: ";
    if (!(cin >> quantity) || quantity <= 0) {   // Reject non-integer or non-positive quantity
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid quantity. Must be a positive whole number." << endl;
        return;
    }

    Products* current = PRODUCTS_LIST;
    do {
        if (current->itemIndex == targetId) {
            if (quantity <= current->itemStock) // Stock is sufficient
                cout << "In stock. Available: " << current->itemStock << endl;
            else                                // Stock insufficient
                cout << "Insufficient stock. Available: " << current->itemStock << endl;
            return;
        }
        current = current->nextItem;
    } while (current != PRODUCTS_LIST);

    cout << "Product ID not found." << endl;
}

void placeOrder(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    if (*PRODUCTS_LIST == nullptr) {    // Check if there are no products in the list
        cout << "No products available." << endl;
        return;
    }

    string customerName;
    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, customerName);
    if (customerName.empty()) {         // Reject empty customer name
        cout << "Customer name cannot be empty." << endl;
        return;
    }

    int targetId, quantity;

    cout << "Enter Product ID: ";
    if (!(cin >> targetId) || targetId <= 0) {  // Reject non-integer or non-positive ID
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid ID. Must be a positive whole number." << endl;
        return;
    }

    cout << "Enter Quantity: ";
    if (!(cin >> quantity) || quantity <= 0) {   // Reject non-integer or non-positive quantity
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid quantity. Must be a positive whole number." << endl;
        return;
    }

    Products* current = *PRODUCTS_LIST;
    do {
        if (current->itemIndex == targetId) {   // The searched itemIndex is found

            // Validate stock availability before confirming the order
            if (quantity > current->itemStock) {
                cout << "Insufficient stock. Available: " << current->itemStock << endl;
                return;
            }

            float total = current->itemPrice * quantity;

            cout << "\n########## Order Summary ##########" << endl;
            cout << "Customer: " << customerName  << endl;
            cout << "Item    : " << current->itemName << " x" << quantity << endl;
            cout << "Total   : P" << total << endl;
            cout << "Confirm order? (Y/N): ";

            char confirm;
            cin >> confirm;
            if (confirm != 'Y' && confirm != 'y') {     // Order cancelled, nothing is deducted or saved
                cout << "Order cancelled." << endl;
                return;
            }

            // Automatically deduct inventory quantity after a successful order
            current->itemStock -= quantity;
            saveData(*PRODUCTS_LIST);

            // Record the completed order as a new node in ORDERS_LIST (order history for this session)
            Orders* newOrder       = new Orders();
            newOrder->customerName = customerName;
            newOrder->itemName     = current->itemName;
            newOrder->orderAmount  = quantity;
            newOrder->totalPrice   = total;
            newOrder->nextOrder    = nullptr;
            newOrder->prevOrder    = nullptr;

            if (*ORDERS_LIST == nullptr) {      // There are no orders in the list yet
                newOrder->nextOrder = newOrder;
                newOrder->prevOrder = newOrder;
                *ORDERS_LIST = newOrder;        // Makes the new order the head of the order list

            } else {                            // There are existing orders
                Orders* tail              = (*ORDERS_LIST)->prevOrder;
                tail->nextOrder           = newOrder;
                newOrder->prevOrder       = tail;
                newOrder->nextOrder       = *ORDERS_LIST;
                (*ORDERS_LIST)->prevOrder = newOrder;   // Appends the order at the last
            }

            // Generate transaction summary / receipt for this order
            saveReceipt(customerName, current->itemName, quantity, total);
            appendTransaction(customerName, current->itemName, quantity, total);

            cout << "Order placed successfully. Receipt saved to receipt.txt" << endl;
            return;
        }
        current = current->nextItem;
    } while (current != *PRODUCTS_LIST);

    cout << "Product ID not found." << endl;
}

void displayOrderDetails(Orders* ORDERS_LIST) {
    if (ORDERS_LIST == nullptr) {
        cout << "No orders placed this session." << endl;
        return;
    }

    cout << "\n########## Order Details (This Session) ##########" << endl;
    cout << "--------------------------------------------------" << endl;

    float runningTotal = 0;
    Orders* current = ORDERS_LIST;
    do {
        cout << "Customer : " << current->customerName << endl;
        cout << "Item     : " << current->itemName     << endl;
        cout << "Qty      : " << current->orderAmount  << endl;
        cout << "Subtotal : P" << current->totalPrice  << endl;
        cout << "--------------------------------------------------" << endl;
        runningTotal += current->totalPrice;
        current = current->nextOrder;
    } while (current != ORDERS_LIST);

    cout << "Grand Total: P" << runningTotal << endl;
}

void showTransactionHistory() {
    ifstream file("transactions.txt");
    if (!file.is_open()) {
        cout << "No transaction history yet." << endl;
        return;
    }

    cout << "\n########## Transaction History (All Time) ##########" << endl;

    string line;
    bool hasContent = false;
    while (getline(file, line)) {
        cout << line << endl;
        hasContent = true;
    }
    file.close();

    if (!hasContent)
        cout << "No transaction history yet." << endl;
}