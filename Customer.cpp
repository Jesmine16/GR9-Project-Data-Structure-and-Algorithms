#include<iostream>
#include<fstream>
#include<iomanip>

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
		    cout << "+----+------------------------------+---------+--------+----------+" << endl;
		    cout << "| No | Product Name                 | Price   | Qty    | Subtotal |" << endl;
		    cout << "+----+------------------------------+---------+--------+----------+" << endl;
		
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
		
		        cout << "| " << setw(2) << index++ << " | ";
		        cout << setw(28) << left << name << " | ";
		        cout << setw(6) << fixed << setprecision(2) << right << price << " | ";
		        cout << setw(6) << left << qty << " | ";
		        cout << setw(8) << subtotal << " |" << endl;
		
		        total += subtotal;
		    }
		
		    cout << "+----+------------------------------+---------+--------+----------+" << endl;
		    cout << "Total Amount: RM " << fixed << setprecision(2) << total << endl;
		
		    cartFile.close();
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
	
	do
	{
		system("cls");
		cout << "Welcome to Apple Store" << endl;
		cout << "1. Display Data" << endl;
		cout << "2. Display Category" << endl;
		cout << "3. Add to Cart" << endl;
		cout << "4. View Cart" << endl;
	
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
					
			case 2: {
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
					}
					
			case 3:{
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
					}
			
			case 4: {
					    system("cls");
					    A.viewCart();
					
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					    break;
					}
			
			case 10: A.releaseMemory();
					return 0;
		}
			
	}while(choice != 10);
	
	return 0;
}
