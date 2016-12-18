// ProductDB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <conio.h>
#define EPSILON 0.001

using namespace std;

unsigned long long CurrentId; // Id for new products
unsigned long long RepoSize;

#pragma region Structures
	struct Product {
		unsigned long long id;
		string name;
		float price;
	};

	struct ProductRepositoryNode {
		Product product;
		ProductRepositoryNode* nextElement;
	};
#pragma endregion

#pragma region RepoControl
	void AddToRepository(ProductRepositoryNode* &r , Product);
	void DisplayRepository(ProductRepositoryNode*);
	void DisplayProductInfo(Product);
	void FindProduct(ProductRepositoryNode*, string);
	void FindProduct(ProductRepositoryNode*, int);
	void FindProduct(ProductRepositoryNode*, double);
	void SortRepository(ProductRepositoryNode* &r, int); //int - mode
#pragma endregion

#pragma region ItemControl
	Product CreateNewProduct();
	void EditProduct(ProductRepositoryNode*, int);
	void DeleteProduct(ProductRepositoryNode* &repository, int);
#pragma endregion

#pragma region ProgramControl
	int Menu();
	int FindMenu();
	int SortMenu();
#pragma endregion

int main()
{
	//init
	ProductRepositoryNode* repo = NULL;
	int option = Menu();
	while (option != 7) {
		switch (option) {
			case 1:
				DisplayRepository(repo);
				break;
			case 2: {
				int optionFind = FindMenu();
				switch (optionFind) {
					case 1:
						int ID;
						cout << "ID : ";
						cin >> ID;
						FindProduct(repo, ID);
						break;
					case 2:
						double price;
						cout << "Price : ";
						cin >> price;
						FindProduct(repo, price);
						break;
					case 3:
						string name;
						cout << "Name : ";
						cin >> name;
						FindProduct(repo, name);
						break;
				}
			}
			break;
			case 3:
				AddToRepository(repo, CreateNewProduct());
				break;
			case 4: {
				int ID;
				cout << "Enter product's ID : ";
				cin >> ID;
				EditProduct(repo, ID); }
				break;
			case 5:
				int ID;
				cout << "Enter product's ID : ";
				cin >> ID;
				DeleteProduct(repo, ID);
				break;
			case 6:
				int optionSort = SortMenu();
				SortRepository(repo, optionSort);
				break;
		}
		_getch();
		option = Menu();
	}
	return 0;
}

void AddToRepository(ProductRepositoryNode* &repository, Product product) {
	ProductRepositoryNode* tmp = repository;	
	ProductRepositoryNode* newProduct = new ProductRepositoryNode;
	newProduct->product = product;
	newProduct->nextElement = NULL;
	if (tmp == NULL) repository = newProduct;
	else {
		while (tmp->nextElement != NULL) tmp = tmp->nextElement;
		tmp->nextElement = newProduct;
	}	
	RepoSize++;
}

void DisplayRepository(ProductRepositoryNode* repository) {
	system("cls");
	int i = 0;
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			cout << ">> Element number " << i << endl;
			DisplayProductInfo(tmp->product);
			i++;
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database\n\n" << endl;
	}
}

void DisplayProductInfo(Product product) {
	//cout << "=========================================================" << endl;
	cout << "> Product id : " << product.id << endl;
	cout << "> Product name : " << product.name << endl;
	cout << "> Product price : " << product.price << endl;
	cout << "=========================================================" << endl;
}

#pragma region FindProductOverloads

void FindProduct(ProductRepositoryNode* repository, string name) {
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			if (tmp->product.name == name) {
				DisplayProductInfo(tmp->product);
			}
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database" << endl;
	}
}

void FindProduct(ProductRepositoryNode* repository, int id) {
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			if (tmp->product.id == id) {
				DisplayProductInfo(tmp->product);
				return;
			}
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database" << endl;
	}
}

void FindProduct(ProductRepositoryNode* repository, double price) {
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			if (fabs(tmp->product.price - price) < EPSILON) {
				DisplayProductInfo(tmp->product);
			}
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database" << endl;
	}
}

#pragma endregion

void SortRepository(ProductRepositoryNode* &repository, int mode) { // 0 - id, 1 - name, 2 - price
	Product* table = new Product[RepoSize];
	ProductRepositoryNode* tmp = repository;
	ProductRepositoryNode* tmp2 = repository;
	int iterator = 0;
	while (tmp != NULL) {
		table[iterator] = tmp->product;
		tmp2 = tmp->nextElement;
		delete tmp;
		tmp = tmp2;
		iterator++;
	}
	switch (mode) {
	case 1: {
			bool p = true;
			for (int i = RepoSize - 1; i > 0; i--) {
				for (int j = 0; j < i; j++) {
					if (table[j].id > table[j + 1].id) swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
		}
		break;

	case 2: {
			bool p = true;
			for (int i = RepoSize - 1; i > 0; i--) {
				for (int j = 0; j < i; j++) {
					if (table[j].name > table[j + 1].name) swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
		}
		break;

	case 3: {
			bool p = true;
			for (int i = RepoSize - 1; i > 0; i--) {
				for (int j = 0; j < i; j++) {
					if (table[j].price > table[j + 1].price) swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
		}
		break;
	}
	repository = NULL;
	int temp = RepoSize;
	RepoSize = 0;
	for (int i = 0; i < temp; i++) AddToRepository(repository, table[i]);
	return;
}

Product CreateNewProduct() {
	Product p;
	system("cls");
	cout << "Enter product information : " << endl;
	cout << "> Product name : " << endl;
	cin >> p.name;
	cout << "> Product price : " << endl;
	cin >> p.price;
	cout << "Assigned ID : " << CurrentId << endl;
	p.id = CurrentId;
	CurrentId++;
	return p;
}

void EditProduct(ProductRepositoryNode* repository, int id) {
	ProductRepositoryNode* tmp = repository;
	while (tmp != NULL) {
		if (tmp->product.id == id) {
			int tmpID = id;
			tmp->product = CreateNewProduct();
			tmp->product.id = tmpID;
			return;
		}
	}
	cout << "Selected product does not exist" << endl;
}

void DeleteProduct(ProductRepositoryNode* &repository, int id) {
	ProductRepositoryNode* tmp = repository;
	if (repository->product.id == id) {
		tmp = NULL;
		if (repository->nextElement != NULL) {
			tmp = repository->nextElement;
		}
		delete repository;
		repository = tmp;
		RepoSize--;
		return;
	}
	if (repository == NULL) return;
	if (repository != NULL) {
		while (tmp->nextElement->product.id != id) {
			if (tmp->nextElement == NULL) return;
			tmp = tmp->nextElement;
		}
		ProductRepositoryNode* temp = tmp->nextElement;
		tmp->nextElement = temp->nextElement;
		delete temp;
		RepoSize--;
	}
}

int Menu() {
	system("cls");
	cout << "------------------- Main menu -----" << endl;
	cout << "====== View =======================" << endl;
	cout << "1. Display all products in database" << endl;
	cout << "2. Find product in database" << endl;
	cout << "====== Manage =====================" << endl;
	cout << "3. Add new product to database" << endl;
	cout << "4. Edit existing product" << endl;
	cout << "5. Delete product from database" << endl;
	cout << "6. Sort database" << endl;
	cout << "\n" << endl;
	cout << "7. Exit" << endl;
	cout << "\nSelect option : ";
	int option;
	cin >> option;
	if (option < 1 || option >7) return Menu();
	return option;
}

int FindMenu() {
	system(" cls");
	cout << "1. Find by ID" << endl;
	cout << "2. Find by price" << endl;
	cout << "3. Find by name" << endl;

	int option;
	cout << "Option : ";
	cin >> option;
	return option;
}

int SortMenu() {
	system(" cls");
	cout << "1. Sort by ID" << endl;
	cout << "2. Sort by price" << endl;
	cout << "3. Sort by name" << endl;

	int option;
	cout << "Option : ";
	cin >> option;
	return option;
}