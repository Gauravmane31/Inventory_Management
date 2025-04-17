#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>  
using namespace std;

struct itemEntry
{
    float unitPrice;
    int copies, product_id, sold;
    char name[30];
    char company[30];
};

struct orderEntry
{
    char name[30];
    int product_id;
    int quantity;
    float totalPrice;
};

class Store
{
public:
    int numItem;
    itemEntry database[100];
    orderEntry orders[100];
    int numOrders;

    Store()
    {
        numItem = 0;
        numOrders = 0;
    }

    

void loadFromFile(const string &filename)
{
    ifstream inFile(filename);
    if (!inFile.is_open())
        return;

    numItem = 0;
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string name, company, id, copies, price, sold;

        getline(ss, name, ',');
        getline(ss, company, ',');
        getline(ss, id, ',');
        getline(ss, copies, ',');
        getline(ss, price, ',');
        getline(ss, sold, ',');

        strcpy(database[numItem].name, name.c_str());
        strcpy(database[numItem].company, company.c_str());
        database[numItem].product_id = stoi(id);
        database[numItem].copies = stoi(copies);
        database[numItem].unitPrice = stof(price);
        database[numItem].sold = stoi(sold);

        numItem++;
    }
    inFile.close();
}

void saveToFile(const string &filename)
{
    ofstream outFile(filename);
    for (int i = 0; i < numItem; i++)
    {
        outFile << database[i].name << ","
                << database[i].company << ","
                << database[i].product_id << ","
                << database[i].copies << ","
                << database[i].unitPrice << ","
                << database[i].sold << "\n";
    }
    outFile.close();
}

    int checkId(int pid)
    {
        for (int i = 0; i < numItem; i++)
        {
            if (database[i].product_id == pid)
            {
                cout << "\n\t\tProduct ID already exists. Please check the ID and try again.";
                return 1;
            }
        }
    }
    
    void insertItem(char itemName[], char company[], int pid, int c, float p)
    {

        if (numItem >= 100)
        {
            cout << "\n\tDatabase full. Cannot insert more items.\n";
            return;
        }

        strcpy(database[numItem].name, itemName);
        strcpy(database[numItem].company, company);
        database[numItem].product_id = pid;
        database[numItem].copies = c;
        database[numItem].unitPrice = p;
        database[numItem].sold = 0;
        cout << "\n\t\t ITEM INSERTED SUCCESSFULLY";
        numItem++;
    }

    itemEntry *searchItem(char itemName[], int pid)
    {
        for (int i = 0; i < numItem; i++)
        {
            if ((strcmp(itemName, database[i].name) == 0) && (database[i].product_id == pid))
            {
                return &database[i];
            }
        }
        return NULL;
    }

    void displayAllItems(bool isadmin = false)
    {
        cout << "\n\t\t-------------> ITEM LIST <--------------\n";
        for (int i = 0; i < numItem; i++)
        {
            cout << "\n\t\t  ITEM: " << database[i].name
                 << " | COMPANY:" << database[i].company
                 << " | ID: " << database[i].product_id
                 << " | STOCK: " << database[i].copies;
            if (isadmin == false)
            {
                cout << " | PRICE: " << database[i].unitPrice;
            }
        }
    }

    bool placeOrder(char itemName[], int pid, int quantity)
    {
        itemEntry *item = searchItem(itemName, pid);
        if (item != NULL && item->copies >= quantity)
        {
            item->copies -= quantity;
            item->sold += quantity;

            orders[numOrders].product_id = pid;
            strcpy(orders[numOrders].name, itemName);
            orders[numOrders].quantity = quantity;
            orders[numOrders].totalPrice = quantity * item->unitPrice;
            numOrders++;

            cout << "\n\t\tOrder placed successfully for " << quantity << " unit(s) of " << item->name << ".\n"
                 << "\t\tYour Cart Price:" << (long long)quantity * item->unitPrice;
            return true;
        }
        else
        {
            cout << "\n\t\tOrder failed. Not enough stock or item not found.";
            return false;
        }
    }

    void displayOrderHistory()
    {
        if (numOrders == 0)
        {
            cout << "\n\t\t  No orders placed yet.";
            return;
        }
        cout << "\n\t\t-------------> ORDER HISTORY <--------------\n";
        for (int i = 0; i < numOrders; i++)
        {
            cout << "\n\t\t  ITEM: " << orders[i].name
                 << " | ID: " << orders[i].product_id
                 << " | QUANTITY: " << orders[i].quantity
                 << " | TOTAL: Rs. " << orders[i].totalPrice;
        }
    }

    void showInventoryReport(bool isAdmin = false)
    {
        long long inventoryC=0;
        cout << "\n\t\t-------------> INVENTORY REPORT <--------------\n";
        for (int i = 0; i < numItem; i++)
        {
            cout << "\n\t\t  ITEM: " << database[i].name
                 << " | COMPANY: " << database[i].company
                 << " | ID: " << database[i].product_id
                 << " | PRICE: Rs. " << database[i].unitPrice;

            if (isAdmin)
            {
                cout << " | STOCK: " << database[i].copies
                     << " | SOLD: " << database[i].sold
                     << " | TOTAL COST:" << database[i].unitPrice * database[i].copies;
                     inventoryC+=database[i].unitPrice * database[i].copies;
            }
        }
        if (inventoryC==0)
        {
            cout<<"\n\t\t  Inventory is Empty";
        }else{
        cout<<"\n\n\t\t  Total Inventory Cost:"<<inventoryC;
        }
    }
    void clearInventory()
    {
        numItem = 0;
        ofstream outFile("", ios::trunc);
        outFile.close();
        cout << "\n\t\tInventory cleared successfully.";
    }
};

int main()
{
    Store store;
    char name[30];
    int product_id, choice;
    string userMode;

    store.loadFromFile("inventory.csv");

    cout << "\n\t\t------Welcome to ACSES Inventory Management System------\n";
    cout << "\n\t\t Are you an Admin or User? (type: admin/user): ";
    cin >> userMode;
    cin.ignore();

    if (userMode == "admin")
    {
        string passKey;
        cout << "\n\t\t\t Enter Admin Pass Key: ";
        cin >> passKey;
        cin.ignore();
        if (passKey != "admin123")
        {
            cout << "\n\t\t Invalid Pass Key. Access Denied.\n";
            return 0;
        }

        int copies;
        float price;
        char company[30];

        do
        {
            cout << "\n\n\t\t----------------> ADMIN MENU <-----------------";
            cout << "\n\n\t\t  1. Insert Item";
            cout << "\n\t\t  2. Search Item";
            cout << "\n\t\t  3. Display All Items";
            cout << "\n\t\t  4. Inventory Report";
            cout << "\n\t\t  5. Clear Entire Inventory";
            cout << "\n\t\t  6. Exit";
            cout << "\n\n\t\tEnter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                cout << "\n\t\t  Enter Item Name: ";
                cin.getline(name, 30);
                cout << "\t\t  Enter Company Name: ";
                cin.getline(company, 30);
                cout << "\t\t  Enter Product ID: ";
                cin >> product_id;
                // int check;
                // check = store.checkId(product_id);
                // if (check == 1)
                //     break;
                cout << "\t\t  Enter Number of Copies: ";
                cin >> copies;
                cout << "\t\t  Enter Unit Price: ";
                cin >> price;
                cin.ignore();
                store.insertItem(name, company, product_id, copies, price);
                store.saveToFile("inventory.csv");
                break;
            case 2:
                cout << "\n\t\t  Enter Item Name: ";
                cin.getline(name, 30);
                cout << "\t\t  Enter Product ID: ";
                cin >> product_id;
                cin.ignore();
                {
                    itemEntry *found = store.searchItem(name, product_id);
                    if (found)
                    {
                        cout << "\n\t\tItem Found: " << found->name
                             << " | Company: " << found->company
                             << " | ID: " << found->product_id
                             << " | Stock: " << found->copies
                             << " | Price: Rs. " << found->unitPrice
                             << " | Sold: " << found->sold;
                    }
                    else
                    {
                        cout << "\n\tItem not found.";
                    }
                }
                break;
            case 3:
                store.displayAllItems();
                break;
            case 4:
                store.showInventoryReport(true);
                break;
            case 5:
                store.clearInventory();
                break;
            case 6:
                store.saveToFile("inventory.csv");
                cout << "\n\t\t--------Exiting Admin Panel. Thank You!--------\n\n";
                break;
            default:
                cout << "\n\tInvalid Choice!";
            }
        } while (choice != 6);
    }
    else if (userMode == "user")
    {
        int orderQty;
        do
        {
            cout << "\n\n\t\t----------------> USER MENU <-----------------\n";
            cout << "\n\t\t  1. Search Item";
            cout << "\n\t\t  2. Display All Items";
            cout << "\n\t\t  3. Place Order";
            cout << "\n\t\t  4. View Order History";
            cout << "\n\t\t  5. Exit\n";
            cout << "\n\t\tEnter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                cout << "\n\t\t  Enter Item Name: ";
                cin.getline(name, 30);
                cout << "\t\t  Enter Product ID: ";
                cin >> product_id;
                cin.ignore();
                {
                    itemEntry *found = store.searchItem(name, product_id);
                    if (found)
                    {
                        cout << "\n\t\tItem Found: " << found->name
                             << " | Company: " << found->company
                             << " | ID: " << found->product_id
                             << " | Price: Rs. " << found->unitPrice;
                    }
                    else
                    {
                        cout << "\n\t\tItem not found.";
                    }
                }
                break;
            case 2:
                store.displayAllItems(false);
                break;
            case 3:
                cout << "\n\t\t  Enter Item Name: ";
                cin.getline(name, 30);
                cout << "\t\t  Enter Product ID: ";
                cin >> product_id;
                cout << "\t\t  Enter Quantity to Order: ";
                cin >> orderQty;
                cin.ignore();
                store.placeOrder(name, product_id, orderQty);
                store.saveToFile("inventory.csv");
                break;
            case 4:
                store.displayOrderHistory();
                break;
            case 5:
                cout << "\n\tExiting User Panel. Thank You!\n";
                break;
            default:
                cout << "\n\tInvalid Choice!";
            }
        } while (choice != 5);
    }
    else
    {
        cout << "\n\tInvalid mode entered. Exiting program.";
    }

    return 0;
}
