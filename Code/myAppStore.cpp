#include <iostream>
#include <string>
#include <math.h>
#include <cstring>

#define CAT_NAME_LEN 25
#define APP_NAME_LEN 50
#define VERSION_LEN 10
#define UNIT_SIZE 3

using namespace std;

struct app_info get_app_info();
//int height(binary_search_tree* root);

/*
* Chase Brown
* 1212869071
*	Project 2
* 
* Milestone due Thur 10/15/2020
*	- For the milestone deadline, the hash table and the array of categories with a binary search tree for each
*		category must be implemented for myAppStore. You need only be able to support the query: find category
*		<category name>.
* 
* Scope:
*	1. Take n number of categories and store the categories into an array of type struct category
*	2. Take m number of applications and store them into it's category bst
*	3. Find k and add each application to the hash table at position x mod k
*	4. Take f number of <find> functions and find ...
*/

typedef struct categories
{
	string category_name;
	//char category[CAT_NAME_LEN]; // Name of category
	struct tree* root; // Pointer to root of search tree for this category
	categories() : category_name(""), root(NULL) {};

} application_categories;

typedef struct app_info
{
	char category[CAT_NAME_LEN]; // Name of category
	char app_name[APP_NAME_LEN]; // Name of the application
	char version[VERSION_LEN]; // Version number
	float size; // Size of the application
	char units[UNIT_SIZE]; // GB or MB
	float price; // Price in $ of the application

} appInfo;

typedef struct tree
{
	struct app_info record; // Information about the application
	struct tree* left; // Pointer to the left subtree
	struct tree* right; // Pointer to the right subtree

} binary_search_tree;

binary_search_tree* create_tree(appInfo app)
{
	binary_search_tree* root = new binary_search_tree();
	root->record = app;
	root->right = NULL;
	root->left = NULL;

	return root;
}

void insert_node(binary_search_tree* &root, appInfo app)
{
	if (root == NULL)
	{
		root = create_tree(app);
		return;
	}
	if (strcmp(root->record.app_name, app.app_name) < 0) //strcmp: If first is less than, it returns a value <0, otherwise return >0, if == returns 0.
	{
		insert_node(root->right, app);
	}
	else
	{
		insert_node(root->left, app);
	}
	
}

/*
char category[CAT_NAME_LEN]; // Name of category
char app_name[APP_NAME_LEN]; // Name of the application
char version[VERSION_LEN]; // Version number
float size; // Size of the application
char units[UNIT_SIZE]; // GB or MB
float price; // Price in $ of the application
*/

void print_app(appInfo app)
{
	cout << app.category << endl;
	cout << app.app_name << endl;
	cout << app.version << endl;
	cout << app.size << endl;
	cout << app.units << endl;
	cout << app.price << endl;
}

struct app_info get_app_info() // Fetches each line of information and stores into appInfo at respective app-type (i.e category, name, version, etc. )
{
	string temp;
	appInfo application;

	getline(cin, temp);
	strncpy(application.category, temp.c_str(), CAT_NAME_LEN);

	getline(cin, temp);
	strncpy(application.app_name, temp.c_str(), APP_NAME_LEN);

	getline(cin, temp);
	strncpy(application.version, temp.c_str(), VERSION_LEN);

	cin >> application.size;
	cin.ignore(100, '\n'); // or getline(cin, temp);

	getline(cin, temp);
	strncpy(application.units, temp.c_str(), UNIT_SIZE);

	cin >> application.price;
	cin.ignore(100, '\n');
	
	return application;

}

typedef struct hash_table_entry
{
	char app_name[APP_NAME_LEN]; // Name of the application
	struct tree* app_node; // Pointer to node in tree containing the application information
	struct hash_table_entry* next; // Pointer to next entry in the chain

} hash_table ;

void find_app(string application, hash_table** hash, int hash_size) // find app functin: Iterates through the hash table and finds matching app_name with app string
{
	for (int i = 0; i < hash_size; i++)
	{
		hash_table* temp = (hash[i]);

		while(temp != NULL)
		{
			if (temp->app_node != NULL && strcmp(temp->app_node->record.app_name, application.c_str()) == 0)
			{
				cout << endl << "Found Application: " << application << endl;

				cout << "\t" << "Category: " << temp->app_node->record.category << endl;
				cout << "\t" << "Application Name: " << temp->app_node->record.app_name << endl;
				cout << "\t" << "Version: " << temp->app_node->record.version << endl;
				cout << "\t" << "Size: " << temp->app_node->record.size << endl;
				cout << "\t" << "Units: " << temp->app_node->record.units << endl;
				cout << "\t" << "Price: " << temp->app_node->record.price << endl;

				return;
			}
			temp = temp->next;
		}
	}

	cout << endl << "Application " << application << " not found." << endl;	
}

void find_category(string category, hash_table** hash, int hash_size) // find category functin: Iterates through the hash table and finds matching category
{
	bool found = false;

	for (int i = 0; i < hash_size; i++)
	{
		hash_table* temp = (hash[i]);

		while (temp != NULL)
		{
			if (temp->app_node != NULL && strcmp(temp->app_node->record.category, category.c_str()) == 0)
			{
				if (found == false)
				{
					cout << endl << "Apps found in Category: " << category << endl;
					found = true;
				}

				cout << "\t" << temp->app_node->record.app_name << endl;
			}
			temp = temp->next;
		}
	}
	if (found == false)
	{
		cout << endl << "Category " << category << " no apps found." << endl;
	}
}

void price_free(hash_table** hash, int hash_size, int categories) // Iterates through the hash and prints any free applications
{
	bool found = false;

	// I use an array to store any category that has previously been printed to contain a free application so we don't print a category more than once
	string* category_used = new string[categories];

	for (int i = 0; i < hash_size; i++)
	{
		hash_table* temp = (hash[i]);

		while (temp != NULL)
		{
			if (temp->app_node != NULL && temp->app_node->record.price == 0.00) //If price is free ($0.00) then...
			{
				//cout << "\t" << temp->app_node->record.app_name << endl;

				if (found == false)
				{
					for (int i = 0; i < categories; i++)
					{
						if (category_used[i] == temp->app_node->record.category)
						{
							// If there exists a category in the array that has already been printed to contain a free app, found = true

							found = true;
						}						
					}
				}
				
				if (found == true) // When there is a free app in a category that's already been printed to contain another free app we just print this app
				{
					cout << "\t" << temp->app_node->record.app_name << endl;
				}
				else // Otherwise we print the category along with the app, and add the category to the "category_used" array
				{
					cout << endl << "Free Applications in Category: " << temp->app_node->record.category << endl;
					cout << "\t" << temp->app_node->record.app_name << endl;

					for (int i = 0; i < categories; i++)
					{
						if (category_used[i] == "")
						{
							category_used[i] += temp->app_node->record.category;
							break;
						}
					}
					
				}
				found = false;
			}
			temp = temp->next;
		} 
	}

	/*for (int x = 0; x < categories; x++)
	{
		cout << category_used[x] << endl;
	}*/

	delete[] category_used;
}

void price_range_recursive(binary_search_tree* bst, float low, float high) // Does an in-order recursive traversal to print any apps in the price range
{
	if (bst != NULL)
	{
		price_range_recursive(bst->left, low, high);
		if (bst->record.price >= low && bst->record.price <= high)
		{
			cout << "\t" << bst->record.app_name << endl;
		}
		price_range_recursive(bst->right, low, high);		
	}
}

void app_range_recursive(binary_search_tree* bst, const char* low, const char* high) // Does an in-order recursive traversal to print any apps in the app alphabetic/ASCII range
{
	if (bst != NULL)
	{
		app_range_recursive(bst->left, low, high);
		if (strcmp(bst->record.app_name, low) >= 0 && strcmp(high, bst->record.app_name) >= 0)
		{
			cout << "\t" << bst->record.app_name << endl;
		}
		app_range_recursive(bst->right, low, high);
	}
}

void range_price(application_categories* cat, int cat_size, string category, float low, float high)
{
	binary_search_tree* ptr = NULL;
	bool found = false;

	for (int i = 0; i < cat_size; i++)
	{
		if (cat[i].category_name == category)
		{
			ptr = cat[i].root;
			found = true;
		}
	}
	if (found)
	{
		cout << endl << "Applications in Price Range (" << low << ", " << high << ") in Category: " << category << endl;
		price_range_recursive(ptr, low, high);
	}
	else
	{
		cout << endl << "No applications found in " << category << " for the given price range (" << low << ", " << high << ")" << endl;
	}
}

void range_app(application_categories* cat, int cat_size, string category, const char* low, const char* high)
{
	binary_search_tree* ptr = NULL;
	bool found = false;

	for (int i = 0; i < cat_size; i++)
	{
		if (cat[i].category_name == category)
		{
			ptr = cat[i].root;
			found = true;
		}
	}
	if (found)
	{
		cout << endl << "Applications in Range (" << low << ", " << high << ") in Category: " << category << "." << endl;
		app_range_recursive(ptr, low, high);
	}
	else
	{
		cout << endl << "No applications found in " << category << " for the given price range (" << low << ", " << high << ")" << endl;
	}
}

/* Found: Application <app name> from Category <category name> successfully deleted.
*  Not Found: Application <app name> not found in category <category name>; unable to delete. 
*/

void delete_node(tree* root, string name)
{
	if (root == nullptr)
	{
		return;
	}

	tree* parent = new tree();
	char child = 'l'; //will help us in remembering if root is the right or left son of his parent

	while (root->record.app_name != name) //first, finding root and remembering who's root parent
	{ 
		if (root->record.app_name < name)
		{
			parent = root;
			root = root->left;
			child = 'l';
		}
		else
		{
			parent = root;
			root = root->right;
			child = 'r';
		}
	}
	// now you have both the root node, and its parent
	if ((root->right == nullptr) && (root->left == nullptr)) // Case 1: Leaf
	{
		delete(root);
		return;
	}
	else if (root->left == nullptr)
	{
		if (child == 'l')
		{
			parent->left = root->right;
		}
		else
		{
			parent->right = root->right;
		}

	}
	else
	{
		tree* replace = root->left;

		while (replace->right != nullptr)
		{
			replace = replace->right;
		} //now replacement is a leaf, and can replace root
		if (child == 'l')
		{
			parent->left = replace;
			replace->left = root->left;
			replace->right = root->right;
		}
		else
		{
			parent->right = replace;
			replace->left = root->left;
			replace->right = root->right;
		}
	}
	delete (root);
}

/*
struct tree delete_from_bst(binary_search_tree* root, string name)
{
	binary_search_tree* node_pointer;

	if (node_pointer == NULL)
		return root;
	else if (strcmp(node_pointer->record.app_name, name.c_str()) > 0)
		root->left = delete_from_bst(root->left, name);
	else if (strcmp(node_pointer->record.app_name, name.c_str()) < 0)
		root->right = delete_from_bst(root->right, name);

}*/

void delete_app(hash_table** hash, application_categories* cat, int hash_size, string category, string name)
{
	/* First search the hash table for the application with name <app name>.  Then it first deletes the entry from the search tree of the given <category name>, and then
	*  also deletes the entry from the hash table.  Finally, it prints Application <app name> from Category <category name> successfully deleted.If the application is
	*  not found it prints Application <app name> not found in category <category name>; unable to delete.substituting the parameters given in the command.
	* 
	*  Found: Application <app name> from Category <category name> successfully deleted.
	*  Not Found: Application <app name> not found in category <category name>; unable to delete. 
	*/

	//cout << hash_size << endl;
	
	for (int i = 0; i < hash_size; i++)
	{
		hash_table* temp = (hash[i]);

		while (temp != NULL)
		{
			if (strcmp(temp->app_name, name.c_str()) == 0) // App found, delete from bst then delete from hash table
			{
				cout << "Found application: " << name << endl;

				//Delete from bst

				delete_node(temp->app_node, name);

				//free(temp);
			}
			temp = temp->next;
		}
	}

}

void print_hash(hash_table** hash, int hash_size) // Iterates through each hash table position and if not NULL, print contents
{

	for (int i = 0; i < hash_size; i++)
	{
		hash_table* temp = (hash[i]);

		while (temp != NULL)
		{
			if (temp->app_node != NULL)
			{
				print_app(temp->app_node->record);
				//temp = temp->next;
			}
			temp = temp->next;
		}
	}
}

int height(binary_search_tree* root) // Calculates the height of given bst
{
	int h = 0;
	if (root != NULL)
	{
		int lHeight = height(root->left);
		int rHeight = height(root->right);
		int maxHeight = max(lHeight, rHeight);
		h = maxHeight + 1;
	}
	return h;
}

int getCount(binary_search_tree* root) // Calculates the number of nodes in bst
{
	if (root == NULL)
	{
		return 0;
	}		
	return 1 + getCount(root->left) + getCount(root->right);
}

void report_bst(hash_table** hash, int hash_size, application_categories* cat, int n)
{
	/*  For the BST: Print the category name, a count of the total
		number of nodes in the tree, the height of the tree, the height of the root node’s left subtree, and the height
		of the root node’s right subtree.
	*/

	int height_of_bst;
	int num_of_nodes;
	int height_left;
	int height_right;

	cout << endl << "[Binary Search Tree]:" << endl;

	for (int i = 0; i < n; i++)
	{
		//tree* pointer = (cat[i].root);

		num_of_nodes = getCount(cat[i].root); // Gets the number of nodes in category[i]
		height_of_bst = height(cat[i].root); // Gets the height of category[i]'s bst
		height_left = height(cat[i].root->left); // Calling the height of category[i]'s left subtree
		height_right = height(cat[i].root->right); // Calling the height of category[i]'s right subtree

		cout << endl << "Category: " << cat[i].category_name << endl;
		cout << "Number of nodes: " << num_of_nodes << endl;
		cout << "Height of bst: " << height_of_bst << endl;
		cout << "Height of left subtree: " << height_left << endl;
		cout << "Height of right subtree: " << height_right << endl;
	}
}

void report_hash(hash_table** hash, int hash_size)
{
	/*  For the hash table: Print a table that lists for each chain length `, 0 ≤ ` ≤ `max, the number of chains
		of length `, up to the maximum chain length `max that your hash table contains. In addition, compute and
		print the load factor α for the hash table, giving n and m.
	*/

	int chain_count = 0;
	int counter = 0;
	int max_counter = 0;

	cout << endl << "[Hash Table]:" << endl;
	cout << "----" << endl; // Top of hash table diagram

	for (int i = 0; i < hash_size; i++) // For loop iterates through the hash table from top to bottom
	{
		hash_table* temp = (hash[i]); // Pointer for this hash position to check through chaining

		counter = 0;

		if (temp->app_node != NULL)
		{
			cout << " " << i << " |";
		}
		while (temp != NULL) //  While loop to iterate through the seperate chains
		{
			if (temp->app_node != NULL) // If node is not null...
			{
				cout << " --> " << temp->app_name;
				if (temp->next == NULL)
				{
					cout << endl << "----" << endl; // Bottom of hash table diagram
				}
				counter++;
				if (counter > chain_count)
				{
					chain_count = counter;
					max_counter = 1;
				}
				else if (counter == chain_count)
				{
					max_counter++;
				}
			
			}
			temp = temp->next;
		}
		
	}
	cout << "Max number of chains: " << chain_count << endl;
	cout << "Number of max chains: " << max_counter << endl;
}

void insert_hash(int position, hash_table** hash, char* app_name, tree* app_node) // Inserts app into the hash table
{
	hash_table* temp = (hash[position]);

	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	hash_table* newEntry = new hash_table();

	strcpy(newEntry->app_name, app_name);
	newEntry->app_node = app_node;
	newEntry->next = NULL;
	if (temp->app_node != NULL)
	{
		hash[position]->next = newEntry;
	}
	else
	{
		hash[position] = newEntry;
	}

	//delete[] newEntry;
}

tree* fetch_node(char* app_name, tree* root)
{
	if (root == NULL)
	{
		return NULL;
	}
	while (root != NULL && strcmp(root->record.app_name, app_name) != 0)
	{
		if (strcmp(root->record.app_name, app_name) < 0)
		{
			root = root->right;
		}
		else
		{
			root = root->left;
		}
	}

	return root;
}

bool TestForPrime(int val)
{ 
	int limit, factor = 2;
	limit = (long)(sqrtf((float)val) + 0.5f);
	while ((factor <= limit) && (val % factor)) factor++;
	return(factor > limit);
}

int hash_function(int k, string app_name)
{
	int x = app_name.length();
	int hash_value = 0;
	int sum = 0;

	for (int i = 0; i < x; i++)
	{
		sum += app_name[i];
	}

	hash_value = sum % k;

	//cout << sum << endl;
	//cout << hash_value << endl;

	return hash_value;
}

int main(int argc, char** args) //int argc, char** args
{
	string get_number_of_categories, tmp;
	string get_number_of_apps;
	string get_number_of_finds;
	int n;
	int m;
	int f;

	//cout << "argc = " << argc << endl;

	while (getline(cin, get_number_of_categories)) // 1. Take n number of categories
	{
		n = stoi(get_number_of_categories);
		//cout << n << endl;

		application_categories* cat = new application_categories[n];
		appInfo temp_struct;


		for (int i = 0; i < n; i++) //Stores the categories into the array of type cat (category)
		{
			getline(cin, cat[i].category_name);
			cat[i].root = NULL;			
		}

		getline(cin, get_number_of_apps); // 2. Take m number of applications and add them to it's bst
		m = stoi(get_number_of_apps);
		//cout << m << endl;

		int hash_size = 0;		
		hash_size = 2 * m;
		
		while (!TestForPrime(hash_size)) // Finding k
		{
			hash_size++;
		}

		hash_table** hash = new hash_table*[hash_size]; // New hash table of size hash_size

		for (int i = 0; i < hash_size; i++)
		{
			hash[i] = new hash_table();
			hash[i]->app_node = NULL;
			hash[i]->next = NULL;
		}

		int position;

		if (m != 0)
		{
			for (int i = 0; i < m; i++)
			{
				temp_struct = get_app_info(); //fetch app_info and store it in <struct app_info> then place it into a temp_struct for the remaining of this loop

				for (int i = 0; i < n; i++)
				{
					if (strcmp (temp_struct.category, cat[i].category_name.c_str()) == 0) // If category of temp_struct (of app_info) matches one of the given categories...
					{

						insert_node(cat[i].root, temp_struct); //insert node with app_info and root
						position = hash_function(hash_size, temp_struct.app_name); // 3. Add each application to the hash table at position x mod k
						insert_hash(position, hash, temp_struct.app_name, fetch_node(temp_struct.app_name, cat[i].root));

						break;

					}
				}

			}

			//print_hash(hash, hash_size);

		} // End of m

		// 4. <Find> ... Function:

		getline(cin, get_number_of_finds); //Get number of finds -> f
		f = stoi(get_number_of_finds);
		//cout << f << endl;

		string line;
		string find_this;
		string search_this;
		string category;

		string no = "no";
		string report = "report";

		string app_low, app_high;
		float price_low, price_high;

		/*
		*  find app <app_name>
		*  find category <category_name>
		*  find price free
		*  range <category_name> price <low> <high>
		*  range <category_name> app <low> <high>
		*  delete <category_name> <app_name>
		*/
			
		for (int i = 0; i < f; i++) //Iterate f times and grab the find function line
		{
			cin >> line;

			if (line == no || line == "" || line == report)
			{
				if (line == report) // If report is requested...
				{
					/*  For the BST: Print the category name, a count of the total
						number of nodes in the tree, the height of the tree, the height of the root node’s left subtree, and the height
						of the root node’s right subtree.

						For the hash table: Print a table that lists for each chain length `, 0 ≤ ` ≤ `max, the number of chains
						of length `, up to the maximum chain length `max that your hash table contains. In addition, compute and
						print the load factor α for the hash table, giving n and m.
					*/

					report_bst(hash, hash_size, cat, n);
					report_hash(hash, hash_size);
					cout << "Number of Categories (n): " << n << endl;
					cout << "Number of Apps (m): " << m << endl;
				}
				else
				{
					break;
				}
			}
			else if (line == "find")
			{
				cin >> find_this;

				if (find_this == "app") // find app <app_name>
				{
					getline(cin, search_this);
					search_this = search_this.substr(2,(search_this.size() - 3)); // Take the substring to get rid of unnecessary spaces of quotations
					find_app(search_this, hash, hash_size);
				}
				else if (find_this == "category") // find category <category_name>
				{
					getline(cin, search_this);
					search_this = search_this.substr(2,(search_this.size() - 3));
					find_category(search_this, hash, hash_size);
				}
				else if (find_this == "price") // find price free
				{
					getline(cin, search_this);
					search_this = search_this.substr(1);
					price_free(hash, hash_size, n);
				}

			}
			else if (line == "range") // range <category name> price <low> <high> ---or---  range <category name> app <low> <high>
			{
				// get <category name>
				string tmp;
				category = "";
				cin >> tmp;

				while (tmp != "app" && tmp != "price") // [Note]: Test with category social networking or another multi-word string of category 
				{
					category += tmp + " ";
					cin >> tmp;
				}
				category = category.substr(1, category.length() - 3);
				if (tmp == "price")
				{
					cin >> price_low;
					cin >> price_high;
					range_price(cat, n, category, price_low, price_high);
				}
				else
				{
					cin >> app_low;
					app_low = app_low.substr(1, app_low.length() - 2);
					cin >> app_high;
					app_high = app_high.substr(1, app_high.length() - 2);
					range_app(cat, n, category, app_low.c_str(), app_high.c_str());
				}

			}
			else if (line == "delete") // delete <category> <application>
			{
				bool exists = false;
				string temporary;
				string delete_from_category = "";
				string delete_application;
				cin >> temporary;

				while (!exists) // Running through the line of test type delete. If the category is found as a match in a previous category, store and move onto application.
				{
					delete_from_category += temporary + " ";

					for (int i = 0; i < n; i++)
					{
						if (cat[i].category_name == delete_from_category.substr(1, delete_from_category.length() - 3))
						{
							exists = true;
							break;
						}
					}
					if (!exists)
					{
						cin >> temporary;
					}
				}

				delete_from_category = delete_from_category.substr(1, delete_from_category.length() - 3); // Removing unnecessary spaces and quotations
				getline (cin, delete_application);
				delete_application = delete_application.substr(2, delete_application.length() - 3); // Removing unnecessary spaces and/or quotations

				//cout << "Category: " << delete_from_category << endl;
				//cout << "App: " << delete_application << endl;

				//delete_app(hash, cat, hash_size, delete_from_category, delete_application);
			}
			
		}

		delete[] cat;
		delete[] hash;
		break;
	}

}
