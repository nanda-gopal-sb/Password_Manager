#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <stdlib.h>
using namespace std;
const char MasterKey[] = "<Your Master Key here>";
struct Password
{
	char textP[256] = {0};
	int lenP = 0;
};
struct User
{
	char Name[256] = {0};
	int lenN = 0;
};
struct Application
{
	char Name[256] = {0};
	int lenW = 0;
};
struct JanusUser
{
	Password password;
	User username;
};
JanusUser user;
struct Key
{
	Application App;
	Password password;
	User username;
};
struct Node
{
	Key *Data;
	Node *next;
};
void encrypt(char *toEncrypt, const char *key, int len)
{
	for (int i = 0; i < len; i++)
	{
		toEncrypt[i] = toEncrypt[i] ^ key[i % (strlen(key) / sizeof(char))];
	}
}
void decrpt(char *toDecrpt, const char *key, int len)
{
	for (int i = 0; i < len; i++)
	{
		toDecrpt[i] = toDecrpt[i] ^ key[i % strlen(key)];
	}
}
void Encrpt_s(Key *toEncrpt)
{
	encrypt(toEncrpt->App.Name, MasterKey, toEncrpt->App.lenW);
	encrypt(toEncrpt->password.textP, MasterKey, toEncrpt->password.lenP);
	encrypt(toEncrpt->username.Name, MasterKey, toEncrpt->username.lenN);
}
void Decrpt_s(Key *toDecrpt)
{
	encrypt(toDecrpt->App.Name, MasterKey, toDecrpt->App.lenW);
	encrypt(toDecrpt->password.textP, MasterKey, toDecrpt->password.lenP);
	encrypt(toDecrpt->username.Name, MasterKey, toDecrpt->username.lenN);
}
void AddUser()
{
	cout << "Enter the name of the User" << endl;
	cin >> user.username.Name;
	user.username.lenN = strlen(user.username.Name);
	cout << "Enter the password" << endl;
	cin >> user.password.textP;
	user.password.lenP = strlen(user.password.textP);
	ofstream outf("userInfo.dat", ios::binary);
	// temporary user for storing it to file
	JanusUser _tmp = user;
	encrypt(_tmp.username.Name, MasterKey, _tmp.username.lenN);
	encrypt(_tmp.password.textP, MasterKey, _tmp.password.lenP);
	outf.write((const char *)(&_tmp), sizeof(JanusUser));
	outf.close();
}
void ReadUser()
{
	ifstream file("userInfo.dat", ios::binary);
	if (file.is_open())
	{
		file.read(reinterpret_cast<char *>(&user), sizeof(JanusUser));
		decrpt(user.username.Name, MasterKey, user.username.lenN);
		decrpt(user.password.textP, MasterKey, user.password.lenP);
		file.close();
		return;
	}
	else
	{
		cout << "No user file found" << endl;
		AddUser();
	}
}
bool auth(JanusUser temp)
{
	if (strcmp(temp.password.textP, user.password.textP) == 0 &&
		strcmp(temp.username.Name, user.username.Name) == 0)
	{
		return true;
	}
	return false;
}
Key *GetNewPassword()
{
	Key *temp = new Key;
	cout << "Enter name of website" << endl;
	cin >> temp->App.Name;
	temp->App.lenW = strlen(temp->App.Name);
	cout << "Enter username" << endl;
	cin >> temp->username.Name;
	temp->username.lenN = strlen(temp->username.Name);
	cout << "Enter Password" << endl;
	cin >> temp->password.textP;
	temp->password.lenP = strlen(temp->password.textP);
	return temp;
}
Key *DeletePassword()
{
	Key *temp = new Key;
	cout << "Enter name of website you want to delete" << endl;
	cin >> temp->App.Name;
	temp->App.lenW = strlen(temp->App.Name);
	cout << "Enter username of Name you want to delete" << endl;
	cin >> temp->username.Name;
	temp->username.lenN = strlen(temp->username.Name);
	return temp;
}
Key *EditPassword()
{
	Key *temp = new Key;
	cout << "Enter name of website you want to edit" << endl;
	cin >> temp->App.Name;
	temp->App.lenW = strlen(temp->App.Name);
	cout << "Enter username of Name you want to edit" << endl;
	cin >> temp->username.Name;
	temp->username.lenN = strlen(temp->username.Name);
	return temp;
}
JanusUser loginForm()
{
	JanusUser temp;
	cout << "Welcome to Janus" << endl;
	cout << "Enter your UserName" << endl;
	cin >> temp.username.Name;
	cout << "Enter your Password " << temp.username.Name << endl;
	cin >> temp.password.textP;
	return temp;
}
void WelcomeMessage()
{
	cout << "Welcome to Janus\nCreating config files";
}
void ShowOptions()
{
	cout << "1 - Show all passwords" << endl;
	cout << "2 - Add new Passwords" << endl;
	cout << "3 - Delete Password" << endl;
	cout << "4 - Exit" << endl;
}
class LinkedListOps
{
private:
	Node *head = nullptr;

public:
	Node *getLinkedList()
	{
		return head;
	}
	LinkedListOps()
	{
		head = nullptr;
	}
	void AddNode(Key *data)
	{
		Node *newNode = new Node();
		newNode->Data = data;
		newNode->next = nullptr;
		if (head == nullptr)
		{
			head = newNode;
		}
		else
		{
			Node *temp = head;
			for (; temp->next != nullptr; temp = temp->next)
				;
			temp->next = newNode;
		}
	}
	void Populate()
	{
		ifstream file("Passwords.dat", ios::binary);
		if (file.is_open())
		{
			while (!file.eof())
			{
				Key *temp = new Key;
				file.read(reinterpret_cast<char *>(temp), sizeof(Key));
				Decrpt_s(temp);
				if (temp->App.Name[0] != '\0' && temp->password.textP[0] != '\0' && temp->username.Name[0] != '\0')
				{
					AddNode(temp);
				}
			}
			file.close();
		}
		else
		{
			cout << "No file found, no stored password found" << endl;
		}
	}
	void deleteNode(Key *toDel)
	{
		Node *temp = head;
		Node *prev = nullptr;
		for (; temp != nullptr; temp = temp->next)
		{
			if (
				strcmp(temp->Data->App.Name, toDel->App.Name) == 0 && strcmp(temp->Data->username.Name, toDel->username.Name) == 0)
			{
				if (temp == head)
				{
					head = temp->next;
					free(temp->Data);
					free(temp);
					cout << "User Sucessfully removed" << endl;
					return;
				}
				else
				{
					if (temp->next == 0)
					{
						prev->next = 0;
						free(temp->Data);
						free(temp);
						cout << "User Sucessfully removed" << endl;
						return;
					}
					else
					{
						prev->next = temp->next;
						free(temp->Data);
						free(temp);
						cout << "User Sucessfully removed" << endl;
						return;
					}
				}
			}
			prev = temp;
		}
		cout << "Specified Password not found try again" << endl;
	}
	void printList()
	{
		Node *temp = head;
		if (temp == nullptr)
		{
			cout << "No passwords found " << endl;
			return;
		}
		while (temp != nullptr)
		{
			Key *data = temp->Data;
			cout << setw(12) << "Website - " << setw(30) << data->App.Name;
			cout << setw(12) << "UserName - " << setw(30) << data->username.Name;
			cout << setw(12) << "Password - " << setw(30) << data->password.textP;
			temp = temp->next;
			cout << endl;
		}
	}
	void WriteIntoFile()
	{
		ofstream outf("Passwords.dat", ios::binary | ios::out);
		Node *temp = head;
		while (temp != nullptr)
		{
			Encrpt_s(temp->Data);
			outf.write((const char *)temp->Data, sizeof(Key));
			temp = temp->next;
		}
		outf.close();
	}
	~LinkedListOps()
	{
		while (head != nullptr)
		{
			Node *temp = head->next;
			free(head->Data);
			free(head);
			head = temp;
		}
	}
};

int main()
{
	LinkedListOps obj1;
	ReadUser();
	if (auth(loginForm()))
	{
		obj1.Populate();
		ShowOptions();
		char ch = '\0';
		while (ch != '5')
		{
			cin >> ch;
			switch (ch)
			{
			case '1':
				system("clear");
				obj1.printList();
				ShowOptions();
				break;
			case '2':
				system("clear");
				obj1.AddNode(GetNewPassword());
				ShowOptions();
				break;
			case '3':
				system("clear");
				if (obj1.getLinkedList() == nullptr)
				{
					cout << "No Node found to delete" << endl;
				}
				else
				{
					obj1.printList();
					obj1.deleteNode(DeletePassword());
				}
				ShowOptions();
				break;
			case '4':
				cout << "Saving Passwords.............Saved" << endl;
				obj1.WriteIntoFile();
				break;
			default:
				system("clear");
				cout << "Invalid Option" << endl;
				ShowOptions();
				break;
			}
		}
	}
	else
	{
		cout << "Username or Password Incorrect" << endl;
	}
}
