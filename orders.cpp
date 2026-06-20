#include "orders.h"
#include "filehandling.h"

#include <iostream>
using namespace std;

// ########## ORDER MANAGEMENT ##########

int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    int orderChoice;

    cout << "\n########## Order Management ##########" << endl;
    cout << "1. Show Customer Orders" << endl;
    cout << "2. Validate Stock Availability" << endl;
    cout << "3. Add Item to Cart" << endl;
    cout << "4. View Cart & Checkout" << endl;
    cout << "5. Display Order Details" << endl;
    cout << "6. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    cin >> orderChoice;

    switch (orderChoice) {
        case 1:
            showCustomerOrders(*ORDERS_LIST);
            break;
        case 2:
            validateStockAvailability(*PRODUCTS_LIST);
            break;
        case 3:
            addToCart(PRODUCTS_LIST, ORDERS_LIST);
            break;
        case 4:
            viewAndCheckout(PRODUCTS_LIST, ORDERS_LIST);
            break;
        case 5:
            displayOrderDetails(*ORDERS_LIST);
            break;
        case 6:
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }

    ManageOrders(PRODUCTS_LIST, ORDERS_LIST);
    return 0;
}

void showCustomerOrders(Orders* ORDERS_LIST) {
    if (ORDERS_LIST == nullptr) {       // Check if there are no orders in the list
        cout << "No orders yet." << endl;
        return;
    }

    Orders* current = ORDERS_LIST;
    do {
        cout << "--------------------------------------------------" << endl;
        cout << "Customer: " << current->customerName << endl;
        cout << "Item: "     << current->itemName     << endl;
        cout << "quantity: "      << current->orderAmount  << endl;
        cout << "Total: P"   << current->totalPrice   << endl;
        current = current->nextOrder;
    } while (current != ORDERS_LIST);
    cout << "--------------------------------------------------" << endl;
}

void validateStockAvailability(Products* PRODUCTS_LIST) {

    if (PRODUCTS_LIST == nullptr) { // Check of there is no products yet in the list
        cout << "No products available." << endl;
        return;
    }

    int targetId, quantity;
    cout << "Enter Product ID: ";
    cin >> targetId;
    cout << "Enter desired quantity: ";
    cin >> quantity;

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

void addToCart(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    if (*PRODUCTS_LIST == nullptr) {    // Check if there are no procucts in the list
        cout << "No products available." << endl;
        return;
    }

    int targetId, quantity;
    string customerName;

    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, customerName);
    cout << "Enter Product ID: ";
    cin >> targetId;
    cout << "Enter Quantity: ";
    cin >> quantity;

    Products* current = *PRODUCTS_LIST;
    do {
        if (current->itemIndex == targetId) {   // The searched itemIndex is found
            if (quantity > current->itemStock) {
                cout << "Insufficient stock. Available: " << current->itemStock << endl;
                return;
            }

            Orders* newOrder = new Orders();    // Creates new node pointer instance
            newOrder->customerName = customerName;
            newOrder->itemName     = current->itemName;
            newOrder->orderAmount  = quantity;
            newOrder->totalPrice   = current->itemPrice * quantity;
            newOrder->nextOrder    = nullptr;
            newOrder->prevOrder    = nullptr;

            if (*ORDERS_LIST == nullptr) {      // There are no orders in the list yet
                newOrder->nextOrder = newOrder;
                newOrder->prevOrder = newOrder;
                *ORDERS_LIST = newOrder;        // Makes the new order the head of the order list

            } else {                            // There are existing orders
                Orders* tail = (*ORDERS_LIST)->prevOrder;
                tail->nextOrder             = newOrder;
                newOrder->prevOrder         = tail;
                newOrder->nextOrder         = *ORDERS_LIST;
                (*ORDERS_LIST)->prevOrder   = newOrder;     // Appends the order at the last
            }

            cout << "Added to cart: " << current->itemName << " x" << quantity << " — P" << newOrder->totalPrice << endl;
            return;
        }
        current = current->nextItem;
    } while (current != *PRODUCTS_LIST);

    cout << "Product ID not found." << endl;
}

void viewAndCheckout(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    if (*ORDERS_LIST == nullptr) {      // Check if the order list empty
        cout << "Cart is empty." << endl;
        return;
    }

    float grandTotal = 0;
    string customerName;

    cout << "\n########## Cart Summary ##########" << endl;
    Orders* current = *ORDERS_LIST;
    do {
        // Prints: itemName x orderAmount - P(totalPrice)
        cout << current->itemName << " x" << current->orderAmount << " — P" << current->totalPrice << endl;
        grandTotal += current->totalPrice; // Add totalPrices per item to the grandTotal
        customerName = current->customerName;
        current = current->nextOrder;
    } while (current != *ORDERS_LIST);

    cout << "Grand Total: P" << grandTotal << endl;
    cout << "Confirm purchase? (Y/N): ";
    char confirm;
    cin >> confirm;

    if (confirm != 'Y' && confirm != 'y') {     // Cancels checkout
        cout << "Checkout cancelled." << endl;
        return;
    }

    // deduct stock
    current = *ORDERS_LIST;
    do {                                        // Substracts ordered amount if everything goes thru
        Products* prod = *PRODUCTS_LIST;
        do {
            if (prod->itemName == current->itemName) {
                prod->itemStock -= current->orderAmount;
                break;
            }
            prod = prod->nextItem;
        } while (prod != *PRODUCTS_LIST);       // Iterates through the PRODUCTS_LIST to reduce itemStock
        current = current->nextOrder;
    } while (current != *ORDERS_LIST);          // Iterates through the ORDERS_LIST to determine orderAmount to subtract

    saveReceipt(*ORDERS_LIST, customerName, grandTotal);
    saveData(*PRODUCTS_LIST);               

    Orders* toDelete = *ORDERS_LIST;           
    do {
        Orders* next = toDelete->nextOrder;
        delete toDelete;
        toDelete = next;
    } while (toDelete != *ORDERS_LIST);         // Iterates through the ORDERS_LIST and frees the dynamic memory
    *ORDERS_LIST = nullptr;                    

    cout << "Order complete. Receipt saved to receipt.txt" << endl;
}

void displayOrderDetails(Orders* ORDERS_LIST) {
    if (ORDERS_LIST == nullptr) {
        cout << "No orders to display." << endl;
        return;
    }

    cout << "\n########## Order Details ##########" << endl;
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