#include<iostream>
#include<fstream>
#include<iomanip>
#include <ctime>

using namespace std;

const int TABLE_SIZE = 23;//hashing table size

struct Apple //define variable
{
	string ID;
	string Category;
	string Product_Name;
	double Price;
	double Cost;
	double Stock;
	Apple* next;// point to the next node in the chain
};

struct CartItem 
{
    string ID;
    string Product_Name;
    double Price;
    int Quantity;
    double Subtotal;
};
 

class Apple_Store
{
	private:
		
		Apple* table[TABLE_SIZE];//our hashing table, using saperate chaining
		
		// we are using ACSII converting a string ID to a table index
		//this hashing method we get the idea from AI, and make a reference to it
		int hashFunction(const string& key)
		{
			int sum = 0;
			for (int i=0; i<key.length(); i++)
			{
				sum += (i+1) * key[i];
			}
			return sum % TABLE_SIZE;
		}
		
		CartItem cart[20];
		int cartCount = 0;	
		
	public:
		
		Apple_Store()//initialize the hash table
		{
			for(int i=0; i<TABLE_SIZE; i++)
			{
				table[i] = NULL;
			}
		}
		
		//add new product
		void addProduct(const string& id, const string& category, const string& product_name, 
		double price, double cost, double stock)
		{
			if(checkID(id))
			{
				cout << "This Product ID is already exists! " << endl;
				return;
			}
			 
			int index = hashFunction(id);
			
			Apple* newNode = new Apple;
			newNode -> ID = id;
			newNode -> Category = category;
			newNode -> Product_Name = product_name;
			newNode -> Price = price;
			newNode -> Cost = cost;
			newNode -> Stock = stock;
			newNode -> next = NULL;
			
			//insertion into the head of a linked table
			if(table[index] == NULL)
			{
				table[index] = newNode;
			}
			else
			{
				newNode -> next = table[index];
				table[index] = newNode;
			}
			 
			saveToFile("Apple_Store.txt");
		} 
		
		//save all data to the txt file
		void saveToFile(const string& filename)
		{
			ofstream outFile(filename);
			
			if(!outFile)
			{
				cout << "Error opening file! " << endl;
			}
			
			for(int i=0; i<TABLE_SIZE; i++)
			{
				Apple* current = table[i];
				while(current != NULL)
				{
					outFile << current -> ID << ","
					<< current -> Category << ","
					<< current -> Product_Name << ","
					<< current -> Price << ","
					<< current -> Cost <<","
					<< current -> Stock << endl;
					
					current = current -> next;
				}
			}
			
			outFile.close();
		}
		
		//display our product list, with table
		void displayData()
		{
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    | Cost     | Stock    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		
		    bool hasData = false;
		
		    for (int i = 0; i < TABLE_SIZE; i++)
		    {
		        Apple* current = table[i];
		
		        while (current != NULL)
		        {
		            hasData = true;
		
		            cout << "| ";
		            cout.width(7); cout << left << current->ID << " | ";
		            cout.width(12); cout << left << current->Category << " | ";
		            cout.width(28); cout << left << current->Product_Name << " | ";
		            cout.width(8); cout << right << fixed << setprecision(2) << current->Price << " | ";
		            cout.width(8); cout << right << fixed << setprecision(2) << current->Cost << " | ";
		            cout.width(8); cout << right << current->Stock << " |" << endl;
		
		            current = current->next;
		        }
		    }
		
		    if (!hasData)
		    {
		        cout << "|" << setw(76) << right << "No product found." << " |" << endl;
		    }
		
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		}

		//using the category to search product, also show will table
		void searchByCategory(const string& target)
		{
		    bool found = false;
		
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    | Cost     | Stock    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		
		    for (int i = 0; i < TABLE_SIZE; i++)
		    {
		        Apple* current = table[i];
		
		        while (current != NULL)
		        {
		            if (current->Category == target)
		            {
		                found = true;
		
		                cout << "| ";
		                cout.width(7);  cout << left  << current->ID << " | ";
		                cout.width(12); cout << left  << current->Category << " | ";
		                cout.width(28); cout << left  << current->Product_Name << " | ";
		                cout.width(8);  cout << right << fixed << setprecision(2) << current->Price << " | ";
		                cout.width(8);  cout << right << fixed << setprecision(2) << current->Cost << " | ";
		                cout.width(8);  cout << right << fixed << setprecision(2) << current->Stock << " |" << endl;
		            }
		
		            current = current->next;
		        }
		    }
		
		    if (!found)
		    {
		        cout << "|" << setw(84) << right << "No product found in this category." << " |" << endl;
		    }
		
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		}

		//load data from file
		void loadFile()
		{
			ifstream file("sorted_information.txt");
			string line;
			
			if(!file.is_open())
			{
				cout << "Error opening file! " << endl;
			}
			
			//this way of loading data from the AI reference
			while (getline(file, line))
			{
				string id, category, product_name;
				double price, cost, stock;
				
				size_t pos1 = line.find(',');
		        size_t pos2 = line.find(',', pos1+1);
		        size_t pos3 = line.find(',', pos2+1);
		        size_t pos4 = line.find(',', pos3+1); 
		        size_t pos5 = line.find(',', pos4+1); 
		        
		        id = line.substr(0, pos1);
		        category = line.substr(pos1+1, pos2-pos1-1); 
		        product_name = line.substr(pos2+1, pos3-pos2-1); 
		        price = stod(line.substr(pos3+1, pos4-pos3-1)); 
		        cost = stod(line.substr(pos4+1, pos5-pos4-1)); 
		        stock = stod(line.substr(pos5+1)); 
		        

		        addProduct(id, category, product_name, price, cost, stock);
			}
			
			file.close();
		}
		
		//using ID to delete the product
		void deleteProduct(const string& id)
		{
			int index = hashFunction(id);
			Apple* current = table[index];
			Apple* prev = NULL;
			
			while(current != NULL)
			{
				if(current -> ID == id)
				{
					if(prev == NULL)
					{
						table[index] = current -> next;
					}
					else
					{
						prev -> next = current -> next;
					}
					delete current;
					
					saveToFile("Apple_Store.txt");
					saveSortedToFile("sorted_information.txt");
					
					cout << "Product " << id << " deleted successfully! " << endl;
					return;
				}
				prev = current;
				current = current -> next;
			}
			cout << "Product " << id << " not found!" << endl;
		}
		
		//increase in stock of commodities (restock) 
		void updateStock(const string& id, double addedStock) 
		{
			if(addedStock <= 0)
			{
				cout << "Invalid input! Stock addition must be position." << endl;
				return;
			}
			
			int index = hashFunction(id);
			Apple* current = table[index];
			
			while(current != NULL)
			{
				if(current -> ID == id)
				{
					current -> Stock += addedStock;
					cout << "Stock updated! Product " << id << " now has " << current -> Stock
					<< " units." << endl;
					return;
				}
				current = current -> next;
			} 
			cout << "Product " << id << " not found!" << endl;
		}
		
		//check if an ID exists, if yes, not allow add with the same ID
		bool checkID(const string& id)
		{
			int index = hashFunction(id);
			Apple* current = table[index];
		
		    while (current != NULL) 
			{
		        if (current -> ID == id) 
				{
		            return true; 
        		}
        		current = current->next;
			}
			return false;
		}
		
		//release memory
		void releaseMemory()
		{
		    for (int i = 0; i < TABLE_SIZE; i++)
		    {
		        Apple* current = table[i];
		        while (current != NULL)
		        {
		            Apple* temp = current;
		            current = current->next;
		            delete temp;
		        }
		        table[i] = NULL;
		    }
		}
		
		void saveSortedToFile(const string& filename)
		{
		    Apple* allProducts[50];
		    int count = 0;
		
		    // collect all pointers into array
		    for (int i = 0; i < TABLE_SIZE; i++) 
			{
		        Apple* current = table[i];
		        
		        while (current != NULL) 
				{
		            allProducts[count++] = current;
		            current = current->next;
		        }
		    }
		    
		    //Selection Sort by ID
		    for (int i = 0; i < count - 1; i++) 
			{
		        int minIndex = i;
		        
		        for (int j = i + 1; j < count; j++)
				{
		            if (allProducts[j]->ID < allProducts[minIndex]->ID) 
					{
		                minIndex = j;
		            }
		        }
		        if (minIndex != i) 
				{
		            Apple* temp = allProducts[i];
		            allProducts[i] = allProducts[minIndex];
		            allProducts[minIndex] = temp;
		        }
		    }
		    
			ofstream outFile(filename);
			if (!outFile) {
		        cout << "Error opening file!" << endl;
		        return;
		    }
		
		    for (int i = 0; i < count; i++) {
		        outFile << allProducts[i]->ID << ","
		                << allProducts[i]->Category << ","
		                << allProducts[i]->Product_Name << ","
		                << allProducts[i]->Price << ","
		                << allProducts[i]->Cost << ","
		                << allProducts[i]->Stock << endl;
		    }
		
		    outFile.close();
		}
		
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void displayCategories()
		{
		    string categories[10];
		    int catCount = 0;
		
		    cout << "=== Available Categories ===" << endl;
		
		    for (int i = 0; i < TABLE_SIZE; i++)
		    {
		        Apple* current = table[i];
		        while (current != NULL)
		        {
		            bool exists = false;

		            for (int j = 0; j < catCount; j++)
		            {
		                if (categories[j] == current -> Category)
		                {
		                    exists = true;
		                    break;
		                }
		            }

		            if (!exists)
		            {
		                categories[catCount++] = current->Category;
		                cout << catCount << ". " << current->Category << endl;
		            }
		
		            current = current->next;
		        }
		    }
		
		    if (catCount == 0)
		        cout << "No categories available." << endl;
		}
		
		void displayData(const string& category)
		{
		    bool found = false;
		
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		
		    for (int i = 0; i < TABLE_SIZE; i++)
		    {
		        Apple* current = table[i];
		        while (current != NULL)
		        {
		            if (current->Category == category)
		            {
		                found = true;
		                cout << "| ";
		                cout.width(7);  cout << left << current->ID << " | ";
		                cout.width(12); cout << left << current->Category << " | ";
		                cout.width(28); cout << left << current->Product_Name << " | ";
		                cout.width(8);  cout << right << fixed << setprecision(2) << current->Price << " | " << endl; 
		            }
		            current = current->next;
		        }
		    }
		
		    if (!found)
		    {
		        cout << "|" << setw(84) << right << "No product found in this category." << " |" << endl;
		    }
		
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		}
		
		int getCategoryCount()
		{
		    string categories[10];
		    int catCount = 0;
		
		    for (int i = 0; i < TABLE_SIZE; i++)
		    {
		        Apple* current = table[i];
		        while (current != NULL)
		        {
		            bool exists = false;
		
		            for (int j = 0; j < catCount; j++)
		            {
		                if (categories[j] == current->Category)
		                {
		                    exists = true;
		                    break;
		                }
		            }
		
		            if (!exists)
		            {
		                categories[catCount++] = current->Category;
		            }
		
		            current = current->next;
		        }
		    }
		    return catCount;
		}
		
		string getCategoryByIndex(int select)
		{
		    string categories[10];
		    int catCount = 0;
		
		    for (int i = 0; i < TABLE_SIZE; i++)
		    {
		        Apple* current = table[i];
		        while (current != NULL)
		        {
		            bool exists = false;
		            for (int j = 0; j < catCount; j++)
		            {
		                if (categories[j] == current->Category)
		                {
		                    exists = true;
		                    break;
		                }
		            }
		            if (!exists)
		            {
		                categories[catCount++] = current->Category;
		            }
		
		            current = current->next;
		        }
		    }
		
		    if (select > 0 && select <= catCount)
		    {
		        return categories[select - 1];
		    }
		    else
		    {
		        return ""; // invalid
		    }
		}
		
		void addToCart(const string& category)
		{
		    Apple* selectedProduct = NULL;

		    displayData(category);
		
		    cout << "\nEnter Product ID to purchase: ";
		    string id;
		    cin.ignore();
		    getline(cin, id);
		
		    int index = hashFunction(id);
		    Apple* current = table[index];
		    while (current != NULL)
		    {
		        if (current->ID == id && current->Category == category)
		        {
		            selectedProduct = current;
		            break;
		        }
		        current = current->next;
		    }
		
		    if (selectedProduct == NULL)
		    {
		        cout << "Product not found in this category." << endl;
		        return;
		    }
		
		    cout << "Enter quantity to purchase: ";
		    int qty;
		    cin >> qty;
		
		    if (qty <= 0)
		    {
		        cout << "Invalid quantity!" << endl;
		        return;
		    }
		
		    if (selectedProduct->Stock < qty)
		    {
		        cout << "Not enough stock available. Current stock: " << selectedProduct->Stock << endl;
		        return;
		    }
		
		    double total = qty * selectedProduct->Price;
		
		    cout << "\nPurchase successful!" << endl;
		    cout << "Product: " << selectedProduct->Product_Name << endl;
		    cout << "Quantity: " << qty << endl;
		    cout << "Total price: RM " << fixed << setprecision(2) << total << endl;

		    ofstream cartFile("Cart.txt", ios::app);
		    if (cartFile)
		    {
		        cartFile << selectedProduct->ID << ","
		                  << selectedProduct->Product_Name << ","
		                  << selectedProduct->Price << ","
		                  << qty << ","
		                  << total << endl;
		        cartFile.close();
		    }
		    else
		    {
		        cout << "Failed to write order log." << endl;
		    }
		}
		
		void viewCart()
		{
		    ifstream cartFile("Cart.txt");
		    if (!cartFile)
		    {
		        cout << "\nCart file not found or cannot be opened." << endl;
		        return;
		    }
		
		    string line;
		    int index = 1;
		    double total = 0.0;
		
		    cout << "\n=== Your Shopping Cart ===" << endl;
		    cout << "+----+------------------------------+----------+--------+----------+" << endl;
		    cout << "| No | Product Name                 | Price    | Qty    | Subtotal |" << endl;
		    cout << "+----+------------------------------+----------+--------+----------+" << endl;
		
		    while (getline(cartFile, line))
		    {
		        string id, name;
		        double price, subtotal;
		        int qty;
		
		        size_t pos1 = line.find(',');
		        size_t pos2 = line.find(',', pos1 + 1);
		        size_t pos3 = line.find(',', pos2 + 1);
		        size_t pos4 = line.find(',', pos3 + 1);
		
		        id = line.substr(0, pos1);
		        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
		        price = stod(line.substr(pos2 + 1, pos3 - pos2 - 1));
		        qty = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
		        subtotal = stod(line.substr(pos4 + 1));
		
		        cout << "| " << setw(2) << left << index++ << " | ";
		        cout << setw(28) << left << name << " | ";
		        cout << setw(8) << fixed << setprecision(2) << left << price << " | ";
		        cout << setw(6) << left << qty << " | ";
		        cout << setw(8) << left << subtotal << " |" << endl;
		
		        total += subtotal;
		    }
		
		    cout << "+----+------------------------------+----------+--------+----------+" << endl;
		    cout << "Total Amount: RM " << fixed << setprecision(2) << total << endl;
		
		    cartFile.close();
		}
	
		string getCurrentDateTime() 
		{
		    time_t now = time(0);                     
		    tm *ltm = localtime(&now);              
		
		    char buffer[20];                         
		    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
		
		    return string(buffer);
		}
		
		string getInv() 
		{
		    time_t now = time(0);                     
		    tm *ltm = localtime(&now);              
		
		    char buffer[20];                         
		    strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", ltm);
		
		    return string(buffer);
		}
		
		void checkoutCart()
		{
		    ifstream inFile("Cart.txt");
		    if (!inFile)
		    {
		        cout << "Cart.txt not found or cannot be opened." << endl;
		        return;
		    }
		
		    string line;
		    double grandTotal = 0.0, subTotal = 0.0;
		    int count = 1;
		    
		    string Inv = getInv();
		    string now = getCurrentDateTime();
			
			cout << "Invoice No: " << Inv << endl;

			cout << "+--------------------------------------------------------------------+" << endl;
			cout << "|                                                                    |" << endl;
			cout << "|                            Apple Store                             |" << endl;
			cout << "|                                                                    |" << endl;			
			cout << "|                                                                    |" << endl;
			cout << "|                                                                    |" << endl;
			cout << "|   Invoice No	:INV" << Inv << "                                   |" << endl;
			cout << "|   Date	:" << now << "                                 |" << endl;
			cout << "|                                                                    |" << endl;			
			cout << "|                                                                    |" << endl;
			cout << "|                                                                    |" << endl;
	
		    ofstream orderFile("Order.txt", ios::app);
		    if (!orderFile)
		    {
		        cout << "Failed to open Order.txt for writing." << endl;
		        return;
		    }
		
		    while (getline(inFile, line))
		    {
		        string id, name, priceCart, qtyCart, subtotalCart;
		        size_t pos1 = line.find(',');
		        size_t pos2 = line.find(',', pos1 + 1);
		        size_t pos3 = line.find(',', pos2 + 1);
		        size_t pos4 = line.find(',', pos3 + 1);
		
		        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos || pos4 == string::npos)
		            continue;
		
		        id = line.substr(0, pos1);
		        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
		        priceCart = line.substr(pos2 + 1, pos3 - pos2 - 1);
		        qtyCart = line.substr(pos3 + 1, pos4 - pos3 - 1);
		        subtotalCart = line.substr(pos4 + 1);
		
		        int qty = stoi(qtyCart);
		        double total = stod(subtotalCart);
		        double price = stod(priceCart);
		        subTotal += total;
		        			
				cout << "|   " << count++ << ". " << setw(28) << left << name << "RM " << setw(10) << fixed << setprecision(2) << left << price << "   " <<
				setw(2) << qty << "   " << "RM " << setw(10) << fixed << setprecision(2) << left << total << "|" << endl;
		
		        orderFile << "\n" << now << "," << Inv << "," << id << "," << name << "," << fixed << setprecision(2) << price << "," << qty << "," << fixed << setprecision(2) << total;
		    }
		    	double SST = subTotal * 0.1;
		    	grandTotal = subTotal + SST; 
		    
		    	cout << "|                                                                    |" << endl;			
				cout << "|                                                                    |" << endl;
				cout << "|                                                                    |" << endl;
				cout << "|   Subtotal " << "                                           RM " << setw(10) << fixed << setprecision(2) << left << subTotal << "|"<< endl; 
				cout << "|   SST (10%) " << "                                          RM "<< setw(10) << fixed << setprecision(2) << left << SST << "|"<< endl; 
				cout << "|                                                      ============  |" << endl;
				cout << "|   Grand Total " << "                                        RM "<< setw(10) << fixed << setprecision(2) << left << grandTotal << "|"<< endl; 
				cout << "|                                                      ============  |" << endl;
				cout << "|                                                                    |" << endl;			
				cout << "|                                                                    |" << endl;
				cout << "|                                                                    |" << endl;
				cout << "+--------------------------------------------------------------------+" << endl;
		    
		    orderFile << "\n" << fixed << setprecision(2) << subTotal << "," << fixed << setprecision(2) << SST << "," << fixed << setprecision(2) << grandTotal << endl;
		
		    inFile.close();
		    orderFile.close();
		
		    cout << "\nOrder saved to Order.txt with timestamp." << endl;
		}
		
		void viewOrderHistory()
		{
		    ifstream inFile("Order.txt");
		    if (!inFile)
		    {
		        cout << "Order.txt not found or cannot be opened." << endl;
		        return;
		    }
		
		    string line;
		    string lastTime = "", lastInv = "";
		    int orderCount = 0;
		
		    cout << "\n=== Order History ===" << endl;
		
		    while (getline(inFile, line))
		    {
		        if (line.empty()) continue;

		        if (isdigit(line[0]) && line.find(':') == string::npos && line.find('-') == string::npos)
		        {
		            double subtotal, sst, grandTotal;
		            size_t p1 = line.find(',');
		            size_t p2 = line.find(',', p1 + 1);
		
		            subtotal = stod(line.substr(0, p1));
		            sst = stod(line.substr(p1 + 1, p2 - p1 - 1));
		            grandTotal = stod(line.substr(p2 + 1));
		
		            cout << "+----------------------------------------------------------------+" << endl;
		            cout << "Subtotal   : RM " << fixed << setprecision(2) << subtotal << endl;
		            cout << "SST (10%)  : RM " << fixed << setprecision(2) << sst << endl;
		            cout << "Grand Total: RM " << fixed << setprecision(2) << grandTotal << endl;
		            cout << "+----------------------------------------------------------------+\n\n" << endl;
		            continue;
		        }
		
		        string date, inv, id, name, priceStr, qtyStr, subtotalStr;
		
		        size_t pos1 = line.find(',');
		        size_t pos2 = line.find(',', pos1 + 1);
		        size_t pos3 = line.find(',', pos2 + 1);
		        size_t pos4 = line.find(',', pos3 + 1);
		        size_t pos5 = line.find(',', pos4 + 1);
		        size_t pos6 = line.find(',', pos5 + 1);
		
		        if (pos6 == string::npos) continue;
		
		        date = line.substr(0, pos1);
		        inv = line.substr(pos1 + 1, pos2 - pos1 - 1);
		        id = line.substr(pos2 + 1, pos3 - pos2 - 1);
		        name = line.substr(pos3 + 1, pos4 - pos3 - 1);
		        priceStr = line.substr(pos4 + 1, pos5 - pos4 - 1);
		        qtyStr = line.substr(pos5 + 1, pos6 - pos5 - 1);
		        subtotalStr = line.substr(pos6 + 1);

		        if (inv != lastInv)
		        {
		            orderCount++;
		            cout << "Order #" << orderCount << " - " << date << " (Invoice: " << inv << ")" << endl;
		            cout << "+----------------------------------------------------------------+" << endl;
		            cout << "| Product Name                 | Price     | Qty | Subtotal      |" << endl;
		            cout << "+----------------------------------------------------------------+" << endl;
		            lastInv = inv;
		        }
		
		        cout << "| " << setw(28) << left << name
		             << " | " << setw(9) << priceStr
		             << " | " << setw(3) << qtyStr
		             << " | " << setw(13) << subtotalStr << " |" << endl;
		    }
		
		    inFile.close();
		}
			
};

int main()
{
	Apple_Store A;
	A.loadFile();
	string id, product_name, category;
	double price, cost, stock, addedStock;
	int choice;
	char option;
	string target;
	
	string now = A.getCurrentDateTime();
	do
	{
		system("cls");
		cout << "Current time: " << now << endl;
		cout << "Welcome to Apple Store" << endl;
		cout << "1. Display Data" << endl;
		cout << "2. Display Category" << endl;
		cout << "3. Add to Cart" << endl;
		cout << "4. View Cart" << endl;
		cout << "5. Check Out" << endl;
		cout << "6. View Order History" << endl;
	
		cout << "Enter choice (1-10) : ";
		cin >> choice;
		
		if(choice < 1 || choice > 10)
		{
			cout << "Invalid Input! Please enter again." << endl;
			
			cout << "Enter choice (1-10) : ";
			cin >> choice;
		}
		
		switch(choice)
		{
			case 1: do
					{
						system("cls");

						cout << "=== Display Product ===" << endl;
						A.displayData();
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
					}while(choice != 1);
					break;
					
			case 2: do
					{
						system("cls");
						A.displayCategories();
						
						int select;
						cout << "\nSelect category number: ";
						cin >> select;
						
						string selectedCategory = A.getCategoryByIndex(select);
						
						if (selectedCategory != "")
						{
						    A.displayData(selectedCategory);
						}
						else
						{
						    cout << "Invalid category selected!" << endl;
						}
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
						break;
					}while(choice != 1);
					
			case 3: do
					{
				    system("cls");
				    A.displayCategories();
				
				    int select;
				    cout << "\nSelect category number to view and purchase from: ";
				    cin >> select;
				
				    string selectedCategory = A.getCategoryByIndex(select);
				    if (selectedCategory != "")
				    {
				        A.addToCart(selectedCategory);
				    }
				    else
				    {
				        cout << "Invalid category selected!" << endl;
				    }
				
				    cout << "\nEnter 1 to exit" << endl;
				    cin >> choice;
				    break;
					}while(choice != 1);
			
			case 4: do
					{
					    system("cls");
					    A.viewCart();
					
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					    break;
					}while(choice != 1);
					
			case 5: do
					{
						system("cls");
						cout << now << endl;
						A.checkoutCart();
						
						cout << "\nEnter 1 to exit" << endl;
				    	cin >> choice;
						break;
					}while(choice != 1);
					
			case 6: do
					{
						system("cls");
    					A.viewOrderHistory();
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
						break;
					}while(choice != 1);
					
			case 10: A.releaseMemory();
					return 0;
		}
			
	}while(choice != 10);
	
	return 0;
}