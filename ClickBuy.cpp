#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <map>
#include<conio.h>
#include<windows.h> 
using namespace std;

// Product Class
class Product {
private:
    int id;
    string name;
    double price;
    int stock;

public:
    Product() : id(0), name(""), price(0), stock(0) {}
    Product(int id, const string& name, double price, int stock)
        : id(id), name(name), price(price), stock(stock) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    void setStock(int newStock) { stock = newStock; }
};

// ShoppingCart Class
class ShoppingCart {
private:
    unordered_map<int, int> items; // Product ID -> Quantity

public:
    void addItem(const Product& product, int quantity) {
        if (quantity <= 0) {
            throw invalid_argument("Quantity must be positive.");
        }
        items[product.getId()] += quantity;
    }

    void removeItem(int productId) {
        if (items.find(productId) == items.end()) {
            throw invalid_argument("Product not found in cart.");
        }
        items.erase(productId);
    }

    double calculateTotal(const unordered_map<int, Product>& productCatalog) const {
        double total = 0.0;
        for (const auto& item : items) {
            int productId = item.first;
            int quantity = item.second;
            total += productCatalog.at(productId).getPrice() * quantity;
        }
        return total;
    }

    void displayCart(const unordered_map<int, Product>& productCatalog) const {
        if (items.empty()) {
            cout << "Shopping cart is empty." << endl;
            return;
        }
        for (const auto& item : items) {
            int productId = item.first;
            int quantity = item.second;
            const Product& product = productCatalog.at(productId);
            cout << "Product: " << product.getName() << ", Quantity: " << quantity
                 << ", Total: " << product.getPrice() * quantity << endl;
        }
    }

    bool isEmpty() const {
        return items.empty();
    }

    const unordered_map<int, int>& getItems() const {
        return items;
    }

    void clear() {
        items.clear();
    }
};

// Order Class
class Order {
private:
    int orderId;
    vector<pair<Product, int>> orderItems; // Pair of Product and Quantity
    double totalAmount;

public:
    Order(int orderId, const vector<pair<Product, int>>& orderItems, double totalAmount)
        : orderId(orderId), orderItems(orderItems), totalAmount(totalAmount) {}

    void displayOrder() const {
        cout << "Order ID: " << orderId << endl;
        for (const auto& item : orderItems) {
            const Product& product = item.first;
            int quantity = item.second;
            cout << "Product: " << product.getName() << ", Quantity: " << quantity
                 << ", Subtotal: " << product.getPrice() * quantity << endl;
        }
        cout << "Total Amount: " << totalAmount << endl;
    }
};

// ECommercePlatform Class
class ECommercePlatform {
private:
    unordered_map<int, Product> productCatalog;
    ShoppingCart shoppingCart;
    vector<Order> orderHistory;
    int nextOrderId = 1;

public:
    void addProduct(const Product& product) {
        productCatalog[product.getId()] = product;
    }

    void browseProducts() const {
        if (productCatalog.empty()) {
            cout << "No products available." << endl;
            return;
        }
        for (const auto& entry : productCatalog) {
            const Product& product = entry.second;
            cout << "Product ID: " << product.getId()
                 << ", Name: " << product.getName()
                 << ", Price: " << product.getPrice()
                 << ", Stock: " << product.getStock() << endl;
                
        }
    }

    void addItemToCart(int productId, int quantity) {
        if (productCatalog.find(productId) == productCatalog.end()) {
            throw invalid_argument("Product ID not found.");
        }
        Product& product = productCatalog[productId];
        if (quantity > product.getStock()) {
            throw invalid_argument("Insufficient stock for product: " + product.getName());
        }
        shoppingCart.addItem(product, quantity);
        product.setStock(product.getStock() - quantity);
        cout << "Added " << quantity << " of " << product.getName() << " to the cart." << endl;
    }

    void placeOrder() {
        if (shoppingCart.isEmpty()) {
            throw runtime_error("Shopping cart is empty.");
        }

        double totalAmount = shoppingCart.calculateTotal(productCatalog);
        vector<pair<Product, int>> orderItems;

        for (const auto& item : shoppingCart.getItems()) {
            int productId = item.first;
            int quantity = item.second;
            orderItems.push_back({productCatalog.at(productId), quantity});
        }

        Order newOrder(nextOrderId++, orderItems, totalAmount);
        orderHistory.push_back(newOrder);
        newOrder.displayOrder();

        shoppingCart.clear(); // Empty the cart after placing the order
    }

    void viewOrderHistory() const {
        if (orderHistory.empty()) {
            cout << "No orders placed yet." << endl;
            return;
        }
        for (const auto& order : orderHistory) {
            order.displayOrder();
        }
    }

    void viewCart() const {
        shoppingCart.displayCart(productCatalog);
    }
};

// User Class
class User {
protected:
    string username;
    string password;

public:
    User() : username(""), password("") {}
    User(const string& username, const string& password)
        : username(username), password(password) {}

    string getUsername() const { return username; }
    bool authenticate(const string& enteredPassword) const {
        return password == enteredPassword;
    }
};

// Manager Class
class Manager : public User {
public:
    Manager() = default;
    Manager(const string& username, const string& password)
        : User(username, password) {}
};

// Customer Class
class Customer : public User {
public:
    Customer() = default;
    Customer(const string& username, const string& password)
        : User(username, password) {}
};

void heading() {
    cout << "____________________________________________________________" << endl;
    cout << "|                                                          |" << endl;
    cout << "|                        FASH-HUB                          |" << endl;
    cout << "|                                                          |" << endl;
    cout << "____________________________________________________________" << endl;
}

// Main Function
int main() {
    ECommercePlatform platform;

    // Sample users
    map<string, Manager> managers = {
        {"admin", Manager("admin", "admin123")}
    };

    map<string, Customer> customers = {
        {"user", Customer("user", "user123")}
    };

    bool running = true;

    while (running) {
        bool loggedIn = false;
        bool isManager = false;
        string currentUser;

        while (!loggedIn) {
            heading();
            cout << "Login as:\n";
            cout << "1. Manager\n";
            cout << "2. Customer\n";
            cout << "Enter your choice: ";
            int userType;
            cin >> userType;

            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (userType == 1 && managers.find(username) != managers.end()) {
                if (managers[username].authenticate(password)) {
                    loggedIn = true;
                    isManager = true;
                    currentUser = username;
                    cout << "Manager login successful.\n";
                } else {
                    cout << "Invalid credentials. Please try again.\n";
                }
            } else if (userType == 2 && customers.find(username) != customers.end()) {
                if (customers[username].authenticate(password)) {
                    loggedIn = true;
                    isManager = false;
                    currentUser = username;
                    cout << "Customer login successful.\n";
                } else {
                    cout << "Invalid credentials. Please try again.\n";
                }
            } else {
                cout << "Invalid credentials. Please try again.\n";
            }
        }

        while (loggedIn) {
            cout << "\nE-Commerce Platform Menu:\n";
            if (isManager) {
                heading();
                cout << "1. Add Product\n";
                cout << "2. Browse Products\n";
                cout << "3. View Order History\n";
                cout << "4. Logout\n";
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;

                try {
                    switch (choice) {
                        case 1: {
                            int id, stock;
                            string name;
                            double price;
                            cout << "Enter Product ID: ";
                            cin >> id;
                            cout << "Enter Product Name: ";
                            cin.ignore();
                            getline(cin, name);
                            cout << "Enter Product Price: ";
                            cin >> price;
                            cout << "Enter Product Stock: ";
                            cin >> stock;
                            platform.addProduct(Product(id, name, price, stock));
                            cout << "Product added successfully.\n";
                            break;
                        }
                        case 2:
                            platform.browseProducts();
                            break;
                        case 3:
                            platform.viewOrderHistory();
                            break;
                        case 4:
                            loggedIn = false;
                            cout << "Logged out successfully.\n";
                             Sleep(2000);
                            system("cls"); 
                            break;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            break;
                    }
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            } else {
                heading();
                cout << "1. Browse Products\n";
                cout << "2. Add Item to Cart\n";
                cout << "3. View Cart\n";
                cout << "4. Place Order\n";
                cout << "5. View Order History\n";
                cout << "6. Logout\n";
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;

                try {
                    switch (choice) {
                        case 1:
                            platform.browseProducts();
                            break;
                        case 2: {
                            int productId, quantity;
                            cout << "Enter Product ID to add to cart: ";
                            cin >> productId;
                            cout << "Enter Quantity: ";
                            cin >> quantity;
                            platform.addItemToCart(productId, quantity);
                            break;
                        }
                        case 3:
                            platform.viewCart();
                            break;
                        case 4:
                            platform.placeOrder();
                            break;
                        case 5:
                            platform.viewOrderHistory();
                            break;
                        case 6:
                            loggedIn = false;
                            cout << "Logged out successfully.\n";
                            Sleep(2000); 
                            system("cls"); 
                            break;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            break;
                    }
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        }
    }

    return 0;
}
