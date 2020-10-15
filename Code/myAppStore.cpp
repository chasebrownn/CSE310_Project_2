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
				cout << "Found Application: " << application << endl;

				cout << "\t" << "Category: " << temp->app_node->record.category << endl;
				cout << "\t" << "Application Name: " << temp->app_node->record.app_name << endl;
				cout << "\t" << "Version: " << temp->app_node->record.version << endl;
				cout << "\t" << "Size: " << temp->app_node->record.size << endl;
				cout << "\t" << "Units: " << temp->app_node->record.units << endl;
				cout << "\t" << "Price: " << temp->app_node->record.price << endl << endl;

				return;
			}
			temp = temp->next;
		}
	}

	cout << endl << "Application " << application << " not found." << endl << endl;	
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

void insert_hash(int position, hash_table** hash, char* app_name, tree* app_node)
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

		int prime_this = 0;
		
		prime_this = 2 * m;
		
		while (!TestForPrime(prime_this)) // Finding k
		{
			prime_this++;
		}

		hash_table** hash = new hash_table*[prime_this];

		for (int i = 0; i < prime_this; i++)
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
						position = hash_function(prime_this, temp_struct.app_name); // 3. Add each application to the hash table at position x mod k
						insert_hash(position, hash, temp_struct.app_name, fetch_node(temp_struct.app_name, cat[i].root));

						break;

					}
				}

			}

			//print_hash(hash, prime_this);

		} // End of m

		// 4. <Find> ... Function:

		getline(cin, get_number_of_finds); //Get number of finds -> f
		f = stoi(get_number_of_finds);
		//cout << f << endl;

		string line;
		string find_this;
		string app;
			
		for (int i = 0; i < f; i++) //Iterate f times and grab the find function line
		{
			//getline(cin, line);
			//find_this = line.substr(14); //find category

			cin >> line;
			cin >> find_this;
			getline(cin, app);
			app = app.substr(1);

			find_app(app, hash, prime_this);

		}

	}

}
