#include <iostream>
#include "HashM.h"
#include <regex>
#include"Windows.h"

Hashmap::Hashmap() {
	size = 10;
	items = (Item**)calloc(size, sizeof(Item*));
}

Hashmap::~Hashmap() {
	for (int i = 0; i < size; ++i) {
		Item* current = items[i];
		while (current != nullptr) {
			Item* next = current->next;
			delete current;
			current = next;
		}
	}
	free(items);
	items = nullptr;
}


long Hashmap::hashFunc(std::string data) {
	long sum = 0;
	for (int i = 0; i < data.length(); i++) {
		sum += data[i];
	}
	return (sum % size);
}


void Hashmap::insertItem(std::string key, std::string data, std::string key2) {

	int index = hashFunc(data);
	Item* current = items[index];

	if (current == nullptr) { 
		items[index] = new Item(key, data, key2);
	}
	else if (current->data == data) { 
		current->key = key;
		return;
	}
	else {
		long i = 1;
		long newIndex = (index + i * i) % size; 
		while (items[newIndex] != nullptr) {
			i++;
			newIndex = (index + i * i) % size;
		}
		items[newIndex] = new Item(key, data, key2);
		return;
	}
}


std::string Hashmap::getItem(std::string data) {
	int index = hashFunc(data);
	Item* current = items[index];
	while (current != nullptr) {
		if (current->data == data) {
			return current->key;
		}
		current = current->next;
	}
	return " "; 
}


void Hashmap::deleteItem(std::string data) {
	long index = hashFunc(data);

	Item* prev = nullptr;
	Item* current = items[index];

	while (current != nullptr) {
		if (current->data == data) {
			if (prev == nullptr) { 
				items[index] = current->next;
			}
			else {
				prev->next = current->next;
			}
			delete current; 
			return;
		}
		prev = current;
		current = current->next;
	}
}


void Hashmap::displayHashmap() {
	for (int i = 0; i < size; i++) {
		Item* current = items[i];

		while (current != nullptr) {

			std::cout << current->key << " - " << current->data << " - " << current->key2 << std::endl;
			current = current->next;
		}
	}
}

bool Hashmap::isValidData(const std::string& data) {
	if (data.length() != 5 || !std::all_of(data.begin(), data.end(), ::isdigit)) {
		return false;
	}

	return true;
}

bool Hashmap::isValidKey(const std::string& key) {

	for (char c : key) {
		if (std::isdigit(c)) {
			return false;
		}
	}
	return true;
}


bool Hashmap::isValidKey2(const std::string& key2) {
	std::regex pattern("^\\d{2}\\.\\d{2}\\.\\d{4}$");
	return std::regex_match(key2, pattern);
}


void Hashmap::menu() {

	int choice;
	int Level = 0;
	std::string key, data, key2;

	do {
		std::cout << "•1 Äîáàâëåíèå çàïèñè \n";
		std::cout << "•2 Ïîèñê\n";
		std::cout << "•3 Óäàëåíèå çàïèñè\n";
		std::cout << "•4 Ïðîñìîòð âñåõ èìåþùèõñÿ çàïèñåé\n";
		std::cout << "•5 Ïîèñê ïî ôðàãìåíòó\n";
		std::cout << "•0 Âûõîä\n";
		std::cout << "Ââåäèòå íîèåð ïóíêòà(0-5): ";
		std::cin >> choice;

		switch (choice) {
		case 1:
			if (Level >= size) {
				std::cout << "Õýø-òàáëèöà ïåðåïîëíåíà.\n";
				system("pause");
			}
			else {

				while (true) {
					std::cout << "Enter ÔÈÎ: ";
					std::cin.ignore();
					std::getline(std::cin, key);

					do {
						std::cout << "Enter ФИО: ";
						std::cin.ignore();
						std::getline(std::cin, key);

					} while (!isValidKey(key));

					do {
						std::cout << "Enter номер договора (из 5 цифр): ";
						std::cin >> data;

					} while (!isValidData(data));

					do {
						std::cout << "Enter др (дд.мм.гггг): ";
						std::cin >> key2;

					} while (!isValidKey2(key2));

					insertItem(key, data, key2);
					Level++;
					break; 
				}

			}
			system("cls");

			break;
		case 2:
			std::cout << "Ââåäèòå äàííûå äëÿ ïîèñêà: ";
			std::cin >> data;
			std::cout << "Ðåçóëüòàò : " << getItem(data) << std::endl;
			system("pause");
			system("cls");
			break;
		case 3:
			std::cout << "Ââåäèòå äàííûå äëÿ óäàëåíèÿ ( íîìåð äîãîâîðà ): ";
			std::cin >> data;
			deleteItem(data);
			Level--;
			system("cls");
			break;
		case 4:
			displayHashmap();
			system("pause");
			system("cls");
			break;
		case 5: {
			std::string key2;
			std::cin >> key2;
			std::vector<std::string> results = search(key2);
			if (!results.empty()) {
				std::cout << "Íàéäåííûå çàïèñè:\n";
				for (const std::string& result : results) {
					std::cout << result << std::endl;
				}
			}
			else {
				std::cout << "Íè÷åãî íå íàéäåíî.\n";
			}
			system("pause");
			system("cls");
			break;
		}

		case 0:
			break;
		default:
			std::cout << "Íåâåðíûé âûáîð. \n";
			system("pause");
			system("cls");
			break;
		}
	} while (choice != 0);
}



std::vector<int> Hashmap::prefix_function(const std::string& pattern) {
	int n = pattern.length();
	std::vector<int> pi(n, 0);
	int k = 0;
	for (int i = 1; i < n; ++i) {
		while (k > 0 && pattern[k] != pattern[i]) {
			k = pi[k - 1];
		}
		if (pattern[k] == pattern[i]) {
			++k;
		}
		pi[i] = k;
	}
	return pi;
}


std::vector<int> Hashmap::KMPSearch(const std::string& text, const std::string& pattern) {
	std::vector<int> positions;
	std::vector<int> pi = prefix_function(pattern);
	int n = text.length();
	int m = pattern.length();
	int q = 0;
	for (int i = 0; i < n; ++i) {
		while (q > 0 && pattern[q] != text[i]) {
			q = pi[q - 1]; 
		}
		if (pattern[q] == text[i]) { 
			++q;
		}
		if (q == m) {
			positions.push_back(i - m + 1); 
			q = pi[q - 1];
		}
	}
	return positions;
}


std::vector<std::string> Hashmap::search(const std::string& fragment) {
	std::vector<std::string> results; 
	for (int i = 0; i < size; ++i) {
		if (items[i] != nullptr) {
			std::vector<int> positions = KMPSearch(items[i]->data, fragment);
			if (!positions.empty()) { 
				results.push_back(items[i]->key);
			}
		}
	}
	return results;

}
