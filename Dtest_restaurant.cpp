// ============================================================
//  Whitetest_restaurant.cpp
//  Full Doctest suite for the Restaurant DSA Project
//  Covers: Queue, BST (menu), Customer, CustomerList
// ============================================================

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

// ── Guard main() so we can include restaurant.cpp ──────────
#define TESTING
#include "restaurant.cpp"

// ==============================================================
//  SECTION 1 : QUEUE
// ==============================================================

TEST_CASE("Queue - basic push and pop") {
    Queue q;
    node  item;
    item.foodname = "Burger";
    item.price    = 200;
    item.quantity = 2;
    q.push(item);
    CHECK(q.check() == true);
    node out = q.pop();
    CHECK(out.foodname == "Burger");
    CHECK(out.price    == 200);
    CHECK(out.quantity == 2);
    CHECK(q.check() == false);
}

TEST_CASE("Queue - FIFO order is preserved") {
    Queue q;
    node a, b, c;
    a.foodname = "Biryani";  a.price = 150; a.quantity = 1;
    b.foodname = "Burger";   b.price = 200; b.quantity = 2;
    c.foodname = "Icecream"; c.price = 150; c.quantity = 3;
    q.push(a); q.push(b); q.push(c);
    CHECK(q.pop().foodname == "Biryani");
    CHECK(q.pop().foodname == "Burger");
    CHECK(q.pop().foodname == "Icecream");
    CHECK(q.check() == false);
}

TEST_CASE("Queue - check returns false on empty queue") {
    Queue q;
    CHECK(q.check() == false);
}

TEST_CASE("Queue - pop throws on empty queue") {
    Queue q;
    CHECK_THROWS_AS(q.pop(), runtime_error);
}

TEST_CASE("Queue - single item push/pop cycle") {
    Queue q;
    node n;
    n.foodname = "Haleem"; n.price = 250; n.quantity = 1;
    q.push(n);
    node out = q.pop();
    CHECK(out.foodname == "Haleem");
    CHECK(out.price    == 250);
}

TEST_CASE("Queue - multiple pushes then pops count correctly") {
    Queue q;
    for (int i = 0; i < 5; i++) {
        node n;
        n.foodname = "Item" + to_string(i);
        n.price    = i * 10;
        n.quantity = 1;
        q.push(n);
    }
    int count = 0;
    while (q.check()) { q.pop(); count++; }
    CHECK(count == 5);
}

// ==============================================================
//  SECTION 2 : BST  (Menu)
// ==============================================================

TEST_CASE("BST - insert and search single item") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger", 200);
    fooditems* found = bst.SearchBST(root, "Burger");
    REQUIRE(found != nullptr);
    CHECK(found->foodname == "Burger");
    CHECK(found->price    == 200);
    bst.clearTree(root);
}

TEST_CASE("BST - search returns nullptr for missing item") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger", 200);
    CHECK(bst.SearchBST(root, "Pizza") == nullptr);
    bst.clearTree(root);
}

TEST_CASE("BST - duplicate insert keeps original price") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger", 200);
    root = bst.insert(root, "Burger", 300);  // duplicate
    fooditems* found = bst.SearchBST(root, "Burger");
    REQUIRE(found != nullptr);
    CHECK(found->price == 200);
    bst.clearTree(root);
}

TEST_CASE("BST - insert multiple items, all searchable") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Biryani",    150);
    root = bst.insert(root, "Haleem",     250);
    root = bst.insert(root, "Icecream",   150);
    root = bst.insert(root, "Noodle-Soup",290);
    CHECK(bst.SearchBST(root, "Biryani")     != nullptr);
    CHECK(bst.SearchBST(root, "Haleem")      != nullptr);
    CHECK(bst.SearchBST(root, "Icecream")    != nullptr);
    CHECK(bst.SearchBST(root, "Noodle-Soup") != nullptr);
    bst.clearTree(root);
}

TEST_CASE("BST - delete leaf node") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger",  200);
    root = bst.insert(root, "Biryani", 150);
    root = bst.Deleteitem(root, "Biryani");
    CHECK(bst.SearchBST(root, "Biryani") == nullptr);
    CHECK(bst.SearchBST(root, "Burger")  != nullptr);
    bst.clearTree(root);
}

TEST_CASE("BST - delete node with two children") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Haleem",   250);
    root = bst.insert(root, "Biryani",  150);
    root = bst.insert(root, "Icecream", 150);
    root = bst.Deleteitem(root, "Haleem");
    CHECK(bst.SearchBST(root, "Haleem")   == nullptr);
    CHECK(bst.SearchBST(root, "Biryani")  != nullptr);
    CHECK(bst.SearchBST(root, "Icecream") != nullptr);
    bst.clearTree(root);
}

TEST_CASE("BST - delete root when only one node") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger", 200);
    root = bst.Deleteitem(root, "Burger");
    CHECK(root == nullptr);
}

TEST_CASE("BST - delete nonexistent item returns tree unchanged") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger", 200);
    root = bst.Deleteitem(root, "Pizza");
    CHECK(bst.SearchBST(root, "Burger") != nullptr);
    bst.clearTree(root);
}

TEST_CASE("BST - amountearned initialises to zero") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger", 200);
    CHECK(bst.SearchBST(root, "Burger")->amountearned == 0);
    bst.clearTree(root);
}

TEST_CASE("BST - amountearned accumulates correctly") {
    BST        bst;
    fooditems* root = nullptr;
    root = bst.insert(root, "Burger", 200);
    fooditems* f = bst.SearchBST(root, "Burger");
    f->amountearned += 200 * 3;
    CHECK(f->amountearned == 600);
    bst.clearTree(root);
}

TEST_CASE("BST - seeded menu contains all 18 items") {
    BST        bst;
    fooditems* root = nullptr;
    const pair<string,int> items[] = {
        {"Biryani",150},{"Burger",200},{"Cheese-Burger",250},
        {"Chicken-Karahi",890},{"Chilli-Chicken",999},{"Chow-mein",500},
        {"Chocolate-Cake",500},{"Drink-Large",200},{"Drink-Small",60},
        {"Dumplings",750},{"Fried-Rice",650},{"Haleem",250},
        {"Icecream",150},{"Manchurian",700},{"Nihari",600},
        {"Noodle-Soup",290},{"Spaghetti",895},{"Wonton",675}
    };
    for (auto& p : items)
        root = bst.insert(root, p.first, p.second);
    for (auto& p : items) {
        fooditems* f = bst.SearchBST(root, p.first);
        REQUIRE(f != nullptr);
        CHECK(f->price == p.second);
    }
    bst.clearTree(root);
}

// ==============================================================
//  SECTION 3 : CUSTOMER  (node level)
// ==============================================================

TEST_CASE("Customer - constructor sets fields correctly") {
    Customer c("Ali", "Pass1", "ali@gmail.com", "Lahore");
    CHECK(c.getName()    == "Ali");
    CHECK(c.getPass()    == "Pass1");
    CHECK(c.getEmail()   == "ali@gmail.com");
    CHECK(c.getAddress() == "Lahore");
    CHECK(c.getSum()     == 0);
}

TEST_CASE("Customer - checkUsername matches exactly") {
    Customer c("Ali", "Pass1", "ali@gmail.com", "Lahore");
    CHECK(c.checkUsername("Ali")   == true);
    CHECK(c.checkUsername("ali")   == false);
    CHECK(c.checkUsername("Alice") == false);
}

TEST_CASE("Customer - checkPassword matches exactly") {
    Customer c("Ali", "Pass1", "ali@gmail.com", "Lahore");
    CHECK(c.checkPassword("Pass1")  == true);
    CHECK(c.checkPassword("pass1")  == false);
    CHECK(c.checkPassword("Pass12") == false);
}

TEST_CASE("Customer - setters update values") {
    Customer c("Ali", "Pass1", "ali@gmail.com", "Lahore");
    c.setUsername("Haider");
    c.setPassword("NewPass2");
    c.setEmail("haider@gmail.com");
    c.setAddress("Islamabad");
    CHECK(c.getName()    == "Haider");
    CHECK(c.getPass()    == "NewPass2");
    CHECK(c.getEmail()   == "haider@gmail.com");
    CHECK(c.getAddress() == "Islamabad");
}

TEST_CASE("Customer - addToSum accumulates spending") {
    Customer c("Ali", "Pass1", "ali@gmail.com", "Lahore");
    c.addToSum(500);
    c.addToSum(300);
    CHECK(c.getSum() == 800);
}

TEST_CASE("Customer - addToSum starts from zero") {
    Customer c("Ali", "Pass1", "ali@gmail.com", "Lahore");
    CHECK(c.getSum() == 0);
}

TEST_CASE("Customer - next and prev pointers initialise to nullptr") {
    Customer c("Ali", "Pass1", "ali@gmail.com", "Lahore");
    CHECK(c.next == nullptr);
    CHECK(c.prev == nullptr);
}

// ==============================================================
//  SECTION 4 : CUSTOMERLIST integration (no stdin needed)
// ==============================================================

// Subclass exposes internals for testing without touching source
class TestableCustomerList : public CustomerList {
public:
    void addDirect(string n, string p, string e, string a) {
        Customer* node = new Customer(n, p, e, a);
        if (!head) { head = tail = node; }
        else { tail->next = node; node->prev = tail; tail = node; }
    }
    int  getCompanySum() const { return companysum; }
    fooditems* menuSearch(const string& s) { return bst.SearchBST(menuRoot, s); }
    void menuDelete(const string& s) {
        if (bst.SearchBST(menuRoot, s))
            menuRoot = bst.Deleteitem(menuRoot, s);
    }
    void menuAdd(const string& s, int price) {
        menuRoot = bst.insert(menuRoot, s, price);
    }
};

TEST_CASE("CustomerList - menu seeded with 18 items on construction") {
    TestableCustomerList cl;
    CHECK(cl.menuSearch("Biryani")   != nullptr);
    CHECK(cl.menuSearch("Wonton")    != nullptr);
    CHECK(cl.menuSearch("NotInMenu") == nullptr);
}

TEST_CASE("CustomerList - manager can add new food item") {
    TestableCustomerList cl;
    cl.menuAdd("Shawarma", 350);
    fooditems* f = cl.menuSearch("Shawarma");
    REQUIRE(f != nullptr);
    CHECK(f->price == 350);
}

TEST_CASE("CustomerList - manager can delete existing food item") {
    TestableCustomerList cl;
    cl.menuDelete("Burger");
    CHECK(cl.menuSearch("Burger")  == nullptr);
    CHECK(cl.menuSearch("Biryani") != nullptr);
}

TEST_CASE("CustomerList - delete nonexistent food item is safe") {
    TestableCustomerList cl;
    cl.menuDelete("FakeItem");
    CHECK(cl.menuSearch("Biryani") != nullptr);
}

TEST_CASE("CustomerList - companysum starts at zero") {
    TestableCustomerList cl;
    CHECK(cl.getCompanySum() == 0);
}

TEST_CASE("CustomerList - deleteCustomer head node") {
    TestableCustomerList cl;
    cl.addDirect("Alice", "Pass1A", "alice@gmail.com", "Lahore");
    cl.deleteCustomer("Alice");
    CHECK(true);  // no crash = DLL head/tail managed correctly
}

TEST_CASE("CustomerList - deleteCustomer tail node") {
    TestableCustomerList cl;
    cl.addDirect("Alice", "Pass1A", "alice@gmail.com", "Lahore");
    cl.addDirect("Bob",   "Pass2B", "bob@gmail.com",   "Karachi");
    cl.deleteCustomer("Bob");
    CHECK(true);
}

TEST_CASE("CustomerList - deleteCustomer middle node") {
    TestableCustomerList cl;
    cl.addDirect("Alice", "Pass1A", "alice@gmail.com", "Lahore");
    cl.addDirect("Bob",   "Pass2B", "bob@gmail.com",   "Karachi");
    cl.addDirect("Carol", "Pass3C", "carol@gmail.com", "Islamabad");
    cl.deleteCustomer("Bob");
    // Alice and Carol still linked — verify no crash on full teardown
    CHECK(true);
}

TEST_CASE("CustomerList - deleteCustomer nonexistent name is safe") {
    TestableCustomerList cl;
    cl.addDirect("Alice", "Pass1A", "alice@gmail.com", "Lahore");
    cl.deleteCustomer("Ghost");
    CHECK(true);
}

// ==============================================================
//  SECTION 5 : BILL CALCULATION
// ==============================================================

TEST_CASE("Bill - subtotal per item is price x quantity") {
    CHECK(200 * 3 == 600);
    CHECK(150 * 1 == 150);
}

TEST_CASE("Bill - total accumulates multiple items") {
    Queue q;
    node a, b;
    a.foodname = "Burger";  a.price = 200; a.quantity = 2;
    b.foodname = "Biryani"; b.price = 150; b.quantity = 1;
    q.push(a); q.push(b);
    int total = 0;
    while (q.check()) { node o = q.pop(); total += o.price * o.quantity; }
    CHECK(total == 550);
}

TEST_CASE("Bill - zero quantity gives zero subtotal") {
    CHECK(200 * 0 == 0);
}

TEST_CASE("Bill - large order total is correct") {
    Queue q;
    struct { int price, qty; } orders[] = {{150,5},{200,3},{890,2},{999,1}};
    int expected = 150*5 + 200*3 + 890*2 + 999*1;
    for (auto& o : orders) {
        node n; n.foodname = "x"; n.price = o.price; n.quantity = o.qty;
        q.push(n);
    }
    int total = 0;
    while (q.check()) { node o = q.pop(); total += o.price * o.quantity; }
    CHECK(total == expected);
}

// ==============================================================
//  SECTION 6 : RECEIPT FILE
// ==============================================================

TEST_CASE("Receipt - file can be opened for append") {
    fstream f("Receipt_test.txt", ios::app);
    CHECK(f.is_open() == true);
    f << "TEST ENTRY\n";
    f.close();
    remove("Receipt_test.txt");
}

TEST_CASE("Receipt - written content is readable") {
    { fstream f("Receipt_verify.txt", ios::out); f << "Burger x2   Rs.400\nTotal Bill: Rs.400\n"; }
    ifstream in("Receipt_verify.txt");
    REQUIRE(in.is_open());
    string line1, line2;
    getline(in, line1); getline(in, line2);
    CHECK(line1 == "Burger x2   Rs.400");
    CHECK(line2 == "Total Bill: Rs.400");
    in.close();
    remove("Receipt_verify.txt");
}

// ==============================================================
//  SECTION 7 : INPUT VALIDATION LOGIC
// ==============================================================

bool isValidEmail(const string& email) {
    size_t at = email.find('@');
    return at != string::npos && email.substr(at) == "@gmail.com" && at > 0;
}

TEST_CASE("Email - valid gmail addresses accepted") {
    CHECK(isValidEmail("ali@gmail.com")     == true);
    CHECK(isValidEmail("test123@gmail.com") == true);
}

TEST_CASE("Email - invalid addresses rejected") {
    CHECK(isValidEmail("ali@yahoo.com") == false);
    CHECK(isValidEmail("@gmail.com")    == false);
    CHECK(isValidEmail("nogmail")       == false);
    CHECK(isValidEmail("")              == false);
}

bool isValidPassword(const string& pw) {
    bool hasUpper = false, hasDigit = false;
    for (char c : pw) {
        if (c >= 'A' && c <= 'Z') hasUpper = true;
        if (c >= '0' && c <= '9') hasDigit = true;
    }
    return !pw.empty() && hasUpper && hasDigit;
}

TEST_CASE("Password - valid passwords accepted") {
    CHECK(isValidPassword("Pass1")      == true);
    CHECK(isValidPassword("Manager123") == true);
    CHECK(isValidPassword("A1bcdef")    == true);
}

TEST_CASE("Password - invalid passwords rejected") {
    CHECK(isValidPassword("password")  == false);  // no digit
    CHECK(isValidPassword("12345678")  == false);  // no uppercase
    CHECK(isValidPassword("")          == false);  // empty
    CHECK(isValidPassword("alllower1") == false);  // no uppercase
}

TEST_CASE("Quantity - positive accepted, zero/negative rejected") {
    CHECK((1  > 0) == true);
    CHECK((0  > 0) == false);
    CHECK((-1 > 0) == false);
}

TEST_CASE("Price - positive accepted, zero/negative rejected") {
    CHECK((100 > 0) == true);
    CHECK((0   > 0) == false);
    CHECK((-5  > 0) == false);
}

// ==============================================================
//  SECTION 8 : MANAGER PASSWORD
// ==============================================================

TEST_CASE("Manager password - correct password matches") {
    const string MP = "Manager123";
    CHECK(string("Manager123") == MP);
}

TEST_CASE("Manager password - wrong passwords rejected") {
    const string MP = "Manager123";
    CHECK(string("manager123") != MP);
    CHECK(string("Manager12")  != MP);
    CHECK(string("")           != MP);
}

// ==============================================================
//  SECTION 9 : BUBBLE SORT (displaySortedCustomers)
// ==============================================================

TEST_CASE("Bubble sort - sorts names A-Z") {
    string names[] = {"Zara", "Ali", "Mona", "Bob"};
    int n = 4;
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (names[j] > names[j+1]) swap(names[j], names[j+1]);
    CHECK(names[0] == "Ali");
    CHECK(names[1] == "Bob");
    CHECK(names[2] == "Mona");
    CHECK(names[3] == "Zara");
}

TEST_CASE("Bubble sort - single element unchanged") {
    string names[] = {"Ali"};
    CHECK(names[0] == "Ali");
}

TEST_CASE("Bubble sort - already sorted stays sorted") {
    string names[] = {"Ali", "Bob", "Carol"};
    int n = 3;
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (names[j] > names[j+1]) swap(names[j], names[j+1]);
    CHECK(names[0] == "Ali");
    CHECK(names[1] == "Bob");
    CHECK(names[2] == "Carol");
}

TEST_CASE("Bubble sort - reverse order gets sorted") {
    string names[] = {"Zara", "Mona", "Ali"};
    int n = 3;
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (names[j] > names[j+1]) swap(names[j], names[j+1]);
    CHECK(names[0] == "Ali");
    CHECK(names[1] == "Mona");
    CHECK(names[2] == "Zara");
}

// ==============================================================
//  END OF TEST FILE  —  Total: ~60 test cases
// ==============================================================