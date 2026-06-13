#include <iostream> 
#include <list>
#include <string>

using namespace std;

struct Products{ //structure for managing products
    int itemIndex;
    string itemName;
    float itemPrice;
    int itemStock;
    Products* nextItem;
    Products* prevItem;
};

struct Orders{ //structure for managing orders
    string customerName;
    string itemName;
    int orderAmount;
    float totalPrice; // to be edited
    Orders *nextOrder;
    Orders *prevOrder;
};

//function declarations 
int mainMenu();

int manageInventory(Products** PRODUCTS_LIST);
void searchProduct(Products** PRODUCTS_LIST); 
void addProduct(Products** PRODUCTS_LIST);  
void updateProduct(Products** PRODUCTS_LIST);
void deleteProduct(Products** PRODUCTS_LIST);
void displayProductsDetails(Products* PRODUCTS_LIST);
void displayInventory(Products* PRODUCTS_LIST);

int manageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void showCustomerOrders(Orders* ORDERS_LIST);
void validateStockAvailability(Products* PRODUCTS_LIST, Orders* ORDERS_LIST);
void acceptAndProcessOrders(Products* PRODUCTS_LIST, Orders* ORDERS_LIST); //includes auto deduct and save receipt
void displayOrderDetails(Orders* ORDERS_LIST);

int main(){ //main function
    Products* PRODUCTS_LIST = nullptr;
    Orders* ORDERS_LIST = nullptr;
    int choice;

    choice = mainMenu();

    while(choice != 3){
    switch(choice){
        case 1:
            manageInventory(&PRODUCTS_LIST);
            break;
        case 2:
            manageOrders(&PRODUCTS_LIST, &ORDERS_LIST);
            break;
        case 3:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}
    return 0;
}

int mainMenu(){ //main menu function
    int choice;
    cout << "Welcome to the Inventory Management System!\n";
    cout << "1. Manage Inventory\n";
    cout << "2. Manage Orders\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: \n";
    cin >> choice;
    switch (choice){
        case 1:
        cout << "Managing inventory...\n";
        break;
        case 2:
        cout << "Managing orders...\n";
        break;
        case 3:
        cout << "Exiting the program. Have a good day!\n";
        break;
        default:
        cout << "Invalid choice. Please try again.\n";
    }
    return choice;
};


int manageInventory(Products** PRODUCTS_LIST){ //inventory management function
    int inventoryChoice;

    cout << "Inventory Management\n";
    cout << "1. Add Product" << endl;
    cout << "2. Update Product" << endl;
    cout << "3. Delete Product" << endl;
    cout << "4. Display All Product Details\n";
    cout << "Enter your choice: ";
    cin >> inventoryChoice;

    switch (inventoryChoice){
        case 1:
        addProduct(PRODUCTS_LIST);
        break;
        case 2:
        updateProduct(PRODUCTS_LIST);
        break;
        case 3:
        deleteProduct(PRODUCTS_LIST);
        break;
        case 4:
        displayProductsDetails(*PRODUCTS_LIST);
        break;
        default:
        cout << "Invalid choice. Please try again.\n";
    }
    return 0;
};

void searchProduct(Products** PRODUCTS_LIST){ //search product function
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

void addProduct(Products** PRODUCTS_LIST){ //add product function
    Products* newProduct = new Products();
    string itemName;
    float itemPrice;
    int itemStock;
    Products* current = *PRODUCTS_LIST; //to assign index to new product
    
    cout<< "Enter Product Name: \n";
    cin >> itemName;
    cout << "Enter Product Price: \n";
    cin >> itemPrice;
    cout << "Enter Product Stock: \n";
    cin >> itemStock;


    newProduct->itemName = itemName;
    newProduct->itemPrice = itemPrice;
    newProduct->itemStock = itemStock;
    newProduct->nextItem = nullptr;
    newProduct->prevItem = nullptr;
    
    if (*PRODUCTS_LIST == nullptr){
        newProduct->itemIndex = 1;
        *PRODUCTS_LIST = newProduct;
    }else{
        int count=1;
        Products* current = *PRODUCTS_LIST;
        while (current->nextItem != nullptr){
            current = current->nextItem;
            count++;
        }
        newProduct->itemIndex = count + 1;
        current->nextItem = newProduct;
        newProduct->prevItem = current;
    }
}

void updateProduct(Products** PRODUCTS_LIST){ //update product function
    string searchName;
    cout << "Enter product name: \n";
    cin >> searchName;
    Products* current = *PRODUCTS_LIST;
    while (current != nullptr){
        if (current->itemName == searchName){
            cout << "Enter new product name: \n";
            cin >> current->itemName;
            cout << "Enter new product price: \n";
            cin >> current->itemPrice;
            break;
        }else{
            current = current->nextItem;
        }
    }
};

void deleteProduct(Products** PRODUCTS_LIST){ //delete product function

    if(*PRODUCTS_LIST==nullptr){
      cout<<"There are no products yet.";
      return;
    }

    string toBeDeleted;
    cout << "Enter the product name to be deleted: ";
    cin >> toBeDeleted;
    Products* current = *PRODUCTS_LIST;

    while (current != nullptr){
        if (current->itemName==toBeDeleted){
            current->prevItem->nextItem = current->nextItem;
            current->nextItem->prevItem = current->prevItem;
            delete current;
            return;
        }else{
            cout << "Item does not exist";
            current = current->nextItem;
        }
    }
};

void displayProductsDetails(Products* PRODUCTS_LIST){
    Products* current = PRODUCTS_LIST;
    while (current != nullptr){
        cout << "Index: " << current->itemIndex << endl;
        cout << "Product Name: " << current->itemName << endl;
        cout << "Product Price: "<< current->itemPrice << endl;
        cout << "Product Stock: " << current->itemStock << endl;
        cout << "-----------------------------" << endl;
        current = current->nextItem;
    }
    return;
}
void displayInventory(Products* PRODUCTS_LIST){
    return;
}

//functions for order management
int manageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST){ //order management function
    int orderChoice;
    cout << "Order Management\n";
    cout << "1. Show Customer Orders\n";
    cout << "2. Validate Stock Availability\n";  
    cout << "3. Accept and Process Orders\n";
    cout << "4. Display Order Details\n";
    cout << "Enter your choice: ";
    cin >> orderChoice;
    
    switch (orderChoice){
        case 1:
        showCustomerOrders(*ORDERS_LIST);
        break;
        case 2:
        validateStockAvailability(*PRODUCTS_LIST, *ORDERS_LIST);
        break;
        case 3:
        acceptAndProcessOrders(*PRODUCTS_LIST, *ORDERS_LIST);
        break;
        case 4:
        displayOrderDetails(*ORDERS_LIST);
        break;
        default:
        cout << "Invalid choice. Please try again.\n";

    }
    return 0;
};

void showCustomerOrders(Orders* ORDERS_LIST){ //show customer orders function
    Orders* current = ORDERS_LIST;
    while (current != nullptr){
        cout << "Customer Name: " << current->customerName << endl;
        cout << "Item Name: " << current->itemName << endl;
        cout << "Order Amount: " << current->orderAmount << endl;
        cout << "Total Price: " << current->totalPrice << endl;
        cout << "-----------------------------" << endl;
        current = current->nextOrder;
    }
}

void validateStockAvailability(Products* PRODUCTS_LIST, Orders* ORDERS_LIST){
    return;
}
void acceptAndProcessOrders(Products* PRODUCTS_LIST, Orders* ORDERS_LIST){
    return;
} //includes auto deduct and save receipt
void displayOrderDetails(Orders* ORDERS_LIST){
    return;
}
