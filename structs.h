#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
using namespace std;

// ########## NODE DEFINITIONS ##########

struct Products {
    int    itemIndex;
    string itemName;
    string itemBrand;
    string itemType;
    float  itemPrice;
    int    itemStock;

    Products* nextItem;
    Products* prevItem;
};

struct Orders {
    string customerName;
    string itemName;
    int    orderAmount;
    float  totalPrice;

    Orders* nextOrder;
    Orders* prevOrder;
};

#endif // STRUCTS_H