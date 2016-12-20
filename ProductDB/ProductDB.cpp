#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <conio.h>
#include <limits>
#include <fstream>
#define EPSILON 0.001

using namespace std;

typedef unsigned long long loong;

loong CurrentId; // Id for new products
loong RepoSize;

#pragma region Structures
	struct Product {
		loong id;
		string name;
		float price;
	};

	struct ProductRepositoryNode {
		Product product;
		ProductRepositoryNode* nextElement;
	};
#pragma endregion

#pragma region Functions
	template <class T> T RequestValueFromUser(string, string);
	template <class T> T RequestValueFromUser(string, string, T, T);
	void SaveToFile(ProductRepositoryNode* &r);
	bool LoadFromFile(ProductRepositoryNode* &r);
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
	RepoSize = 0;
	if (!LoadFromFile(repo)) std::cout << "Unable to load database file. Creating new" << endl;
	int option = Menu();
	while (option != 8) {
		switch (option) {
		case 1:
			DisplayRepository(repo);
			break;
		case 2: {
			int optionFind = FindMenu();
			switch (optionFind) {
			case 1:
				int ID;
				std::cout << "ID : ";
				std::cin >> ID;
				FindProduct(repo, ID);
				break;
			case 2:
				double price;
				std::cout << "Price : ";
				std::cin >> price;
				FindProduct(repo, price);
				break;
			case 3:
				string name;
				std::cout << "Name : ";
				std::cin >> name;
				FindProduct(repo, name);
				break;
			}
		}
				break;
		case 3:
			AddToRepository(repo, CreateNewProduct());
			break;
		case 4: {
			int editId;
			std::cout << "Enter product's ID : ";
			std::cin >> editId;
			EditProduct(repo, editId);
		}
				break;
		case 5: {
			int ID;
			std::cout << "Enter product's ID : ";
			std::cin >> ID;
			DeleteProduct(repo, ID);
		}
				break;
		case 6: {
			int optionSort = SortMenu();
			SortRepository(repo, optionSort);
		}
				break;
		case 7:
			SaveToFile(repo);
			break;
		}
		_getch();
		option = Menu();
	}
	while (repo != NULL) {
		ProductRepositoryNode* tmp = repo->nextElement;
		delete repo;
		repo = tmp;
	}
	return 0;
}

template <class T> T RequestValueFromUser(string message, string errorMessage) {
	T value;
	while (true) {
		std::cout << message << endl;
		std::cin >> value;
		if (std::cin.fail()) {
			std::cout << errorMessage<< endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		break;
	}
	return value;
}

template <class T> T RequestValueFromUser(string message, string errorMessage, T min, T max) {
	T value;
	while (true) {
		std::cout << message << endl;
		std::cin >> value;
		if (std::cin.fail() || value<min || value>max) {
			std::cout << errorMessage << " in range : " << min << " to " << max << endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		break;
	}
	return value;
}

void SaveToFile(ProductRepositoryNode* &repository) {
	ifstream f1("ProductDB.dbf");
	if (f1.good()) {
		rename("ProductDB.dbf", "ProductDB.transact.dbf");
		f1.close();
	}
	try {
		ofstream output("ProductDB.dbf", ios::trunc);
		output << CurrentId << endl << RepoSize << endl;
		ProductRepositoryNode* tmp = repository;
		if (tmp != NULL) {
			while (tmp != NULL) {
				output << tmp->product.id <<" "<< tmp->product.name << " " << tmp->product.price << endl;
				tmp = tmp->nextElement;
			}
		}
		output.close();
	}
	catch (const char* msg) {
		std::cout << msg << endl;
		remove("ProductDB.dbf");
		rename("ProductDB.transact.dbf", "ProductDB");
	}
	remove("ProductDB.transact.dbf");
}

bool LoadFromFile(ProductRepositoryNode* &repository) {
	ifstream f1("ProductDB.dbf");
	if (!f1.good()) {
		
		CurrentId = 0;
		RepoSize = 0;
		return false;
	}
	else {
		loong tmpSize = 0;
		try {
			f1 >> CurrentId;
			f1 >> tmpSize; 
			Product p;

			while (f1 >> p.id >> p.name >> p.price) {
				AddToRepository(repository, p);
			}
			f1.close();
			RepoSize = tmpSize;
			return true;
		}
		catch (const char* msg) {
			std::cout << msg << endl;
			return false;
		}
	}
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
			std::cout << ">> Element number " << i << endl;
			DisplayProductInfo(tmp->product);
			i++;
			tmp = tmp->nextElement;
		}
	}
	else {
		std::cout << "No products found in database\n\n" << endl;
	}
}

void DisplayProductInfo(Product product) {
	//std::cout << "=========================================================" << endl;
	std::cout << "> Product id : " << product.id << endl;
	std::cout << "> Product name : " << product.name << endl;
	std::cout << "> Product price : " << product.price << endl;
	std::cout << "=========================================================" << endl;
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
		std::cout << "No products found in database" << endl;
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
		std::cout << "No products found in database" << endl;
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
		std::cout << "No products found in database" << endl;
	}
}

#pragma endregion

void SortRepository(ProductRepositoryNode* &repository, int mode) { // 0 - id, 1 - name, 2 - price
	system("cls");
	if (repository == NULL) {
		std::cout << "Unable to sort empty database" << endl;
		return;
	}
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
			for (loong i = RepoSize - 1; i > 0; i--) {
				for (loong j = 0; j < i; j++) {
					if (table[j].id > table[j + 1].id) swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
		}
		break;

	case 2: {
			bool p = true;
			for (loong i = RepoSize - 1; i > 0; i--) {
				for (loong j = 0; j < i; j++) {
					if (table[j].name > table[j + 1].name) swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
		}
		break;

	case 3: {
			bool p = true;
			for (loong i = RepoSize - 1; i > 0; i--) {
				for (loong j = 0; j < i; j++) {
					if (table[j].price > table[j + 1].price) swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
		}
		break;
	}
	repository = NULL;
	loong temp = RepoSize;
	RepoSize = 0;
	for (int i = 0; i < temp; i++) AddToRepository(repository, table[i]);
	std::cout << "Done!";
	return;
}

Product CreateNewProduct() {
	Product p;
	system("cls");
	std::cout << "Enter product information : " << endl;
	std::cout << "> Product name : " << endl;
	std::cin >> p.name;
	p.price = RequestValueFromUser<float>("> Product price : ", "Enter numeric value!");
	std::cout << "Assigned ID : " << CurrentId << endl;
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
		tmp = tmp->nextElement;
	}
	std::cout << "Selected product does not exist" << endl;
}

void DeleteProduct(ProductRepositoryNode* &repository, int id) {
	ProductRepositoryNode* tmp = repository;
	if (repository == NULL) return;
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
	std::cout << "------------------- Main menu -----" << endl;
	std::cout << "\n\n"<<RepoSize<<" items in database\n\n" << endl;
	std::cout << "====== View =======================" << endl;
	std::cout << "1. Display all products in database" << endl;
	std::cout << "2. Find product in database" << endl;
	std::cout << "====== Manage =====================" << endl;
	std::cout << "3. Add new product to database" << endl;
	std::cout << "4. Edit existing product" << endl;
	std::cout << "5. Delete product from database" << endl;
	std::cout << "6. Sort database" << endl;
	std::cout << "7. Save changes" << endl;
	std::cout << "\n" << endl;
	std::cout << "8. Exit" << endl;
	return RequestValueFromUser("\nSelect option : ", "Wrong option! Select value ", 1, 8);
}

int FindMenu() {
	system(" cls");
	std::cout << "1. Find by ID" << endl;
	std::cout << "2. Find by price" << endl;
	std::cout << "3. Find by name" << endl;
	return RequestValueFromUser<int>("Option : ", "Enter numeric value ", 1, 3);
}

int SortMenu() {
	system(" cls");
	std::cout << "1. Sort by ID" << endl;
	std::cout << "2. Sort by name" << endl;
	std::cout << "3. Sort by price" << endl;

	return RequestValueFromUser<int>("Option : ", "Enter numeric value ", 1, 3);
}