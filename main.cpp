#include <iostream>
using namespace std;

#include "structs.h"
#include "filehandling.h"
#include "inventory.h"
#include "orders.h"

// ########## MAIN MENU ##########

int MainMenu() {
    int choice;
    cout << "\nWelcome to the BeauteQ Inventory Management System!" << endl;
    cout << "1. Manage Inventory" << endl;
    cout << "2. Manage Orders"    << endl;
    cout << "3. Exit"             << endl;
    cout << "\nEnter your choice: ";
    cin >> choice;
    return choice;
}

// ########## MAIN FUNCTION ##########

int main() {
    Products* PRODUCTS_LIST = nullptr;
    Orders*   ORDERS_LIST   = nullptr;

    loadData(&PRODUCTS_LIST);

    int choice = MainMenu();
    while (choice != 3) {
        switch (choice) {
            case 1: ManageInventory(&PRODUCTS_LIST);               break;
            case 2: ManageOrders(&PRODUCTS_LIST, &ORDERS_LIST);    break;
            default: cout << "Invalid choice. Please try again." << endl; break;
        }
        choice = MainMenu();
    }

    saveData(PRODUCTS_LIST);
    cout << "Exiting BeauteQ. Goodbye!" << endl;
    return 0;
}