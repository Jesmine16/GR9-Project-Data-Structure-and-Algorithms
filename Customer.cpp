#include<iostream>
#include<fstream>
#include<iomanip>
#include <ctime>

using namespace std;

const int RUN = 32; //use for sorting

struct Apple {
	string id, category, name;
	int stock;
	double price, cost;
	bool deleted = false;
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
		
		Apple queue[50]; 
   		int head, tail;	
   		int count = 0;
		
	public:
		
		Apple_Store() 
		{  
	        tail = -1;  
	        head = 0;  
	    }  
		
		// Check the queue is empty or not
		int empty() {  
		    if(head == tail+1) 
			{
			    return 1;  
			} 
			else 
			{
				return 0;
			}
		}  
		
		// Check the queue is full or not
		int full() {
			if(tail == 49) 
			{
				return 1;
			} 
			else 
			{
				return 0;
			}
		}
		
		//add new product
		// Add a new product to the queue
		void addProduct(string id, string category, string name, double price, double cost, int stock) 
		{
			if(!full()) 
			{
				tail++;
				queue[tail].id = id;
				queue[tail].category = category;				
				queue[tail].name = name;
				queue[tail].price = price;
				queue[tail].cost = cost;
				queue[tail].stock = stock;
			} 
			else 
			{
				cout << "Queue is full" << endl;
				
				saveToFile("Apple_Store.txt");
			}
		}
		
		//save all data to the txt file
		void saveToFile(const string& filename)
		{
			ofstream outFile(filename);
			
			    if(!outFile) 
				{
			        cout << "Error opening file! " << endl;
			        return;
			    }
			    
			    if(empty()) 
				{
			        cout << "Queue is empty, nothing to save." << endl;
			        outFile.close();
			        return;
			    }
			    
			    for(int i = head; i <= tail; i++) 
				{
			        outFile << queue[i].id << ","
			                << queue[i].category << ","
			                << queue[i].name << ","
			                << queue[i].price << ","
			                << queue[i].cost << ","
			                << queue[i].stock << endl;
			    }
			
			outFile.close();
		}
		
		//display our product list, with table
		void displayData() 
		{
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    | Cost     | Stock    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		
		    if (empty()) 
			{
		        cout << "|" << setw(76) << right << "No products available." << " |" << endl;
		    } else 
			{
		        for (int i = head; i <= tail; i++) 
				{
		            cout << "| ";
		            cout.width(7); cout << left << queue[i].id << " | ";
		            cout.width(12); cout << left << queue[i].category << " | ";
		            cout.width(28); cout << left << queue[i].name << " | ";
		            cout.width(8); cout << right << fixed << setprecision(2) << queue[i].price << " | ";
		            cout.width(8); cout << right << fixed << setprecision(2) << queue[i].cost << " | ";
		            cout.width(8); cout << right << queue[i].stock << " |" << endl;
		        }
		    }
		    
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		}
		
		void mergeByCategory(int left, int mid, int right) ///
		{
		    int n1 = mid - left + 1;
		    int n2 = right - mid;
		    
		    Apple* leftArr = new Apple[n1];
		    Apple* rightArr = new Apple[n2];
		    
		    for (int i = 0; i < n1; i++)
		        leftArr[i] = queue[left + i];
		    for (int j = 0; j < n2; j++)
		        rightArr[j] = queue[mid + 1 + j];
		    
		    int i = 0, j = 0, k = left;
		    while (i < n1 && j < n2) 
			{
		        if (leftArr[i].category <= rightArr[j].category) 
				{
		            queue[k] = leftArr[i];
		            i++;
		        } 
				else 
				{
		            queue[k] = rightArr[j];
		            j++;
		        }
		        k++;
		    }
		    
		    while (i < n1) 
			{
		        queue[k] = leftArr[i];
		        i++;
		        k++;
		    }
		    while (j < n2) 
			{
		        queue[k] = rightArr[j];
		        j++;
		        k++;
		    }
		    
		    delete[] leftArr;
		    delete[] rightArr;
		}

		void mergeSortByCategory(int left, int right) 
		{
		    if (left < right) 
			{
		        int mid = left + (right - left) / 2;
		        mergeSortByCategory(left, mid);
		        mergeSortByCategory(mid + 1, right);
		        mergeByCategory(left, mid, right);
		    }
		}

		//using the category to search product, also show will table
		void searchByCategory(const string& target) 
		{
		    if (!empty()) 
			{
		        mergeSortByCategory(head, tail);
		    } 
			else 
			{
		        cout << "Queue is empty!" << endl;
		        return;
		    }
		    
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    | Cost     | Stock    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		    
		    bool found = false;
		    int left = head;
		    int right = tail;

		    while (left <= right) 
			{
		        int mid = left + (right - left) / 2;
		        
		        if (queue[mid].category == target) 
				{

		            found = true;

		            int temp = mid;
		            while (temp >= head && queue[temp].category == target) 
					{
		                printProduct(queue[temp]);
		                temp--;
		            }

		            temp = mid + 1;
		            while (temp <= tail && queue[temp].category == target) 
					{
		                printProduct(queue[temp]);
		                temp++;
		            }
		            break;
		        }
		        else if (queue[mid].category < target) 
				{
		            left = mid + 1;
		        }
		        else 
				{
		            right = mid - 1;
		        }
		    }
		    
		    if (!found) 
			{
		        cout << "|" << setw(84) << right << "No product found in this category." << " |" << endl;
		    }
		    
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		}

		void printProduct(const Apple& product) 
		{
		    cout << "| ";
		    cout.width(7);  cout << left  << product.id << " | ";
		    cout.width(12); cout << left  << product.category << " | ";
		    cout.width(28); cout << left  << product.name << " | ";
		    cout.width(8);  cout << right << fixed << setprecision(2) << product.price << " | ";
		    cout.width(8);  cout << right << fixed << setprecision(2) << product.cost << " | ";
		    cout.width(8);  cout << right << product.stock << " |" << endl;
		}

		//load data from file
		void loadFile()
		{
			ifstream file("Apple_Store.txt");
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
		
		void compactQueue() 
		{
		    int newTail = head - 1;
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{
		            queue[++newTail] = queue[i];
		        }
		    }
		    tail = newTail;
		    cout << "Queue compacted. New size: " << (tail-head+1) << endl;
		}
		
		void updateStock(const string& id, double addedStock) 
		{
		    if (addedStock <= 0) {
		        cout << "Invalid input! Stock addition must be positive." << endl;
		        return;
		    }
		
		    if (empty()) {
		        cout << "Error: Queue is empty, cannot update." << endl;
		        return;
		    }
		
		    bool found = false;

		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted && queue[i].id == id) 
				{
		            queue[i].stock += addedStock;
		            found = true;
		            
		            saveToFile("Apple_Store.txt");
		            
		            cout << "Stock updated! Product " << id 
		                 << " now has " << queue[i].stock
		                 << " units." << endl;
		            break;
		        }
		    }
		
		    if (!found) 
			{
		        cout << "Product " << id << " not found!" << endl;
		    }
		}
		
		//release memory
		void releaseMemory()
		{
		    head = 0;
        	tail = -1;
        	count = 0;
		}
		
		~Apple_Store() 
		{
        	releaseMemory();
    	}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void displayCategories() 
		{
		    string categories[10]; 
		    int catCount = 0;
		
		    cout << "=== Available Categories ===" << endl;
		    
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{
		            bool exists = false;

		            for (int j = 0; j < catCount; j++) 
					{
		                if (categories[j] == queue[i].category) 
						{
		                    exists = true;
		                    break;
		                }
		            }

		            if (!exists && catCount < 10) 
					{
		                categories[catCount] = queue[i].category;
		                cout << (catCount + 1) << ". " << queue[i].category << endl;
		                catCount++;
		            }
		        }
		    }
		
		    if (catCount == 0) 
			{
		        cout << "No categories available." << endl;
		    }
		}
		
		void displayData(const string& category) 
		{
		    bool found = false;
		
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+" << endl;

		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted && queue[i].category == category) 
				{
		            found = true;
		            cout << "| ";
		            cout.width(7);  cout << left << queue[i].id << " | ";
		            cout.width(12); cout << left << queue[i].category << " | ";
		            cout.width(28); cout << left << queue[i].name << " | ";
		            cout.width(8);  cout << right << fixed << setprecision(2) << queue[i].price << " | " << endl;
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
		
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{ 
		            bool exists = false;
		
		            for (int j = 0; j < catCount; j++) 
					{
		                if (categories[j] == queue[i].category) 
						{
		                    exists = true;
		                    break;
		                }
		            }
		
		            if (!exists && catCount < 10)
					{
		                categories[catCount++] = queue[i].category;
		            }
		        }
		    }
		
		    return catCount;
		}
		
		string getCategoryByIndex(int select) 
		{
		    string categories[10];
		    int catCount = 0;
		
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{
		            bool exists = false;
		
		            for (int j = 0; j < catCount; j++) {
		                if (categories[j] == queue[i].category) 
						{
		                    exists = true;
		                    break;
		                }
		            }
		            if (!exists && catCount < 10) {
		                categories[catCount++] = queue[i].category;
		            }
		        }
		    }
		
		    return (select > 0 && select <= catCount) ? categories[select - 1] : "";
		}
		
		void addToCart(const string& category) 
		{
		    displayData(category);
		    
		    cout << "\nEnter Product ID to purchase: ";
		    string id;
		    cin.ignore();
		    getline(cin, id);
		
		    Apple* selectedProduct = nullptr;
		    for (int i = head; i <= tail && !selectedProduct; i++)
			{
		        if (!queue[i].deleted && queue[i].id == id && queue[i].category == category) 
				{
		            selectedProduct = &queue[i];
		        }
		    }
		
		    if (!selectedProduct) 
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
		    if (selectedProduct->stock < qty) 
			{
		        cout << "Not enough stock available. Current stock: " 
		             << selectedProduct->stock << endl;
		        return;
		    }
		
		    double total = qty * selectedProduct->price;
		
		    cout << "\nPurchase successful!" << endl;
		    cout << "Product: " << selectedProduct->name << endl;
		    cout << "Quantity: " << qty << endl;
		    cout << "Total price: RM " << fixed << setprecision(2) << total << endl;
		
		    ofstream cartFile("Cart.txt", ios::app);
		    if (cartFile) 
			{
		        cartFile << selectedProduct->id << ","
		                 << selectedProduct->name << ","
		                 << selectedProduct->price << ","
		                 << qty << ","
		                 << total << endl;
		        cartFile.close();
		    
		    } 
			else 
			{
		        cout << "Failed to write to cart file." << endl;
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
					}while(choice != 1);
					break;
					
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
					}while(choice != 1);
					break;
			
			case 4: do
					{
					    system("cls");
					    A.viewCart();
					
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					}while(choice != 1);
					break;
					
			case 5: do
					{
						system("cls");
						cout << now << endl;
						A.checkoutCart();
						
						cout << "\nEnter 1 to exit" << endl;
				    	cin >> choice;
					}while(choice != 1);
					break;
					
			case 6: do
					{
						system("cls");
    					A.viewOrderHistory();
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
					}while(choice != 1);
					break;
					
			case 10: A.releaseMemory();
					return 0;
		}
			
	}while(choice != 10);
	
	return 0;
}