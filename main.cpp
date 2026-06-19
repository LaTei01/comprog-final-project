#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>



using namespace std;


struct Products{
    int itemIndex;
    string itemName;
    string itemBrand; // added
    string itemType; // added
    float itemPrice;
    int itemStock; // corrected

    Products* nextItem;
    Products* prevItem;

};


struct Orders{
    string customerName;
    string itemName;
    int orderAmount;
    float totalPrice; 
    Orders *nextOrder;
    Orders *prevOrder;
};


int MainMenu();

// ########## INVENTORY MANAGEMENT FUNCTION DECLARATION ##########

int ManageInventory(Products** PRODUCTS_LIST);
void searchProduct(Products** PRODUCTS_LIST);
void addProduct(Products** PRODUCTS_LIST);
void updateProduct(Products** PRODUCTS_LIST);
void deleteProduct(Products** PRODUCTS_LIST);
void displayProductsDetails(Products* PRODUCTS_LIST);
void displayInventory(Products* PRODUCTS_LIST);
void checkLowStock(Products* PRODUCTS_LIST);

// ########## ORDERS MANAGEMENT FUNCTIONS DECLARATION ##########

int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void showCustomerOrders(Orders* ORDERS_LIST);
void validateStockAvailability(Products* PRODUCTS_LIST);
void addToCart(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void viewAndCheckout(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void displayOrderDetails(Orders* ORDERS_LIST);

// ########## FILE INPUT AND OUTPUT FUNCTIONS DECLARATION ##########

void loadData(Products** PRODUCTS_LIST);
void saveData(Products* PRODUCTS_LIST);
void saveReceipt(Orders* ORDERS_LIST, string customerName, float total);

// ########## MAIN FUNCTION ##########

int main(){
    Products* PRODUCTS_LIST = nullptr;
    Orders* ORDERS_LIST = nullptr;
    int choice;

    // loadData(&PRODUCTS_LIST);

    choice = MainMenu();

    while (choice != 3) {
        switch (choice) {
            case 1:
                ManageInventory(&PRODUCTS_LIST);
                break;
            case 2:
                ManageOrders(&PRODUCTS_LIST, &ORDERS_LIST);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        choice = MainMenu();
    }

    // saveData(PRODUCTS_LIST);
    cout << "Exiting the program. Goodbye!" << endl;
    return 0;
}


int MainMenu(){
    int choice;

    cout << "Welcome to the Beauteq Inventory Management System!" << endl;
    cout << "1. Manage Inventory" << endl;
    cout << "2. Manage Orders" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";

    cin >> choice;
    return choice;
}  




int ManageInventory(Products** PRODUCTS_LIST){
    int inventoryChoice;

    cout << "\n########## Inventory Management ##########" << endl;
    cout << "1. Search Product" << endl;
    cout << "2. Add Product" << endl;
    cout << "3. Update Product" << endl;
    cout << "4. Delete Product" << endl;
    cout << "5. Display All Product Details" << endl;
    cout << "6. Display In-Store Inventory" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    cin >> inventoryChoice;

    switch (inventoryChoice) {
        case 1:
            searchProduct(PRODUCTS_LIST);
            break;
        case 2:
            addProduct(PRODUCTS_LIST);
            break;
        case 3:
            updateProduct(PRODUCTS_LIST);
            break;
        case 4:
            deleteProduct(PRODUCTS_LIST);
            break;
        case 5:
            displayProductsDetails(*PRODUCTS_LIST);
            break;
        case 6:
            displayInventory(*PRODUCTS_LIST);
            break;
        case 7:
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }

    ManageInventory(PRODUCTS_LIST);
    return 0;
};


void searchProduct(Products** PRODUCTS_LIST){
    string searchName;
    cout << "Enter the product name to search: ";
    cin >> searchName;
    Products* current = *PRODUCTS_LIST;
    while (current != nullptr){
        if (current->itemName==searchName){
            cout << "Index: " << current->itemIndex;
            cout << "Product Name: " << current->itemName;
            cout << "Product Price: "<< current->itemPrice;
            break;
        }else{
            current = current->nextItem;
        }
    }
}


void addProduct(Products** PRODUCTS_LIST){
    Products newProduct;


    string itemName;
    float itemPrice;
    cout<< "Enter Product Name: \n";
    cin >> itemName;
    cout<< "Enter Product Price: \n";
    cin >> itemPrice;


    newProduct.itemName = itemName;
    newProduct.itemPrice = itemPrice;
    newProduct.nextItem = nullptr;
    newProduct.prevItem = nullptr;
}


void updateProduct(Products** PRODUCTS_LIST) {
}

void deleteProduct(Products** PRODUCTS_LIST) {
}

void displayProductsDetails(Products* PRODUCTS_LIST) {

}

void displayInventory(Products* PRODUCTS_LIST) {

}

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
}

void validateStockAvailability(Products* PRODUCTS_LIST) {

}

void addToCart(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {

}

void viewAndCheckout(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {

}

void displayOrderDetails(Orders* ORDERS_LIST) {

}

// ########## FILE I/O ##########

void loadData(Products** PRODUCTS_LIST) {

}

void saveData(Products* PRODUCTS_LIST) {

}

void saveReceipt(Orders* ORDERS_LIST, string customerName, float total) {
}



