/*
Name: Natnael Gebremariam
NSHE: 5002150850
Assignment: 6
Description: Create double bracket hash function & use it to store bank information
Input: Account Login, Deposit Amount, Withdrawal Amount, View, Exit
Output:  Bank Account Information
*/
#include "hashMap.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <string.h>
#include <cstdlib>
#include <algorithm>
using namespace std; 

struct bankType
{
	struct transactionType
	{
		//each bank record will have a litf of transactions
		transactionType(bool t, double a)
		{
			type = t;
			amount = a;
		}
		// transaction type true/false deposit/withdrawal
		bool type;
		// transaction amount
		double amount;
	};
	//Some constructors to help make this struct easier to handle
	bankType(std::string first, std::string last, std::string acctNo,
		double balance,unsigned int pin)
	{
		name = last + ", " + first;
		this->balance = balance;
		this->pin = pin;
		this->acctNo = acctNo;
		locked = false;
	}

	bankType()
	{
		name = "";
		balance = 0;
		pin = 0;
		locked = false;
	}

	std::string name; //name on account
	std::string acctNo; //bank account number
	double balance; // current balance on acount
	unsigned int pin; //pin number
	bool locked; //true/false for locked/unlocked
	//a list of deposit/withdrawal and amounts of each
	std::vector<transactionType> transactions;
};
std::string allLower(string str)
{
	//transform all string inputs to lowerCase to eas of comparison
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}
void readData(std::fstream& inFile,  hashMap<std::string, hashMap<std::string, bankType > >& bank) 
{
	//first_name,last_name,location,AcctNo,Balance,PIN  
	//deleted first line 
	inFile.open("data.csv"); 
	std::string line = ""; 
	std::string line1 = ""; 
	getline(inFile, line1); 
	while(!inFile.eof())
	{
		//get the line 
		std::getline(inFile, line);
		if(line.empty()) break; 
		
		//get the first string till the comma and convert all inputs to lower case
		std::string firstName = allLower(line.substr(0, line.find(",")));
		//erase the line read in, till nex comma and repeat 
		line.erase(0, line.find(",")+1); 
		//last name 
		std::string lastName = allLower(line.substr(0, line.find(",")));
		//erase line 
		line.erase(0, line.find(",")+1); 
		//location and save into hashMap with error key 0000 to find later on 
		std::string location = allLower(line.substr(0, line.find(",")));
		//erase line 
		line.erase(0, line.find(",")+1); 
		//find acctNo 
		std::string acct = line.substr(0, line.find(","));
		//erase later on  
		line.erase(0, line.find(",")+1); 
		
		//use lib to convert to a double 
		double balance = std::stod(line.substr(0, line.find(",")));
		//erase
		line.erase(0, line.find(",")+1); 
		
		//use lib to convert to int and read till end of line char
		int pin = stoi(line);
		
		//last 4 digs of the acctNo string // all acctNo are 10 dig # 
		std::string acct4Digits = acct.substr(6);
		//hashMap the location to search and use
		bank[location]["0000"].name = "exists";
		//hashMap all the name, balance, acctNo and pin to the hashMap to access
		bank[location][acct4Digits] = bankType(firstName, lastName, acct, balance, pin); 
	}
	//close file 
	inFile.close(); 

}
string userInput(string loc)
{
	//user input asking for branch location, use peek to account for spacing 
	string loc2  = ""; 
	cout<< "\nPlease enter bank branch location: ";
	cin>>loc;
	if(cin.peek() == ' ' && cin.peek()!=',')
	{
		cin>>loc2;
		cout<<endl; 
		return allLower(loc + " " + loc2);  
	}
	cout<<endl; 
	return allLower(loc);  
	
		
}
bool checkBankLocation(std::string place, hashMap<std::string, hashMap<std::string, bankType > >& bank)
{
	//searches for existing locations, as denoted in the hashMap, if location is presnet
	if(bank[place]["0000"].name == "exists")
	{
		return true; 
	}
	return false; 
} 
bool getAcctNo(std::string branch, string last4acctNo, hashMap< std::string, hashMap< std::string, bankType > >& bank)
{
	
	string tempAcctNo= bank[branch][last4acctNo].acctNo;
	//checks if user input are found as last 4 of acctNumber in the hashMap 
	if(tempAcctNo =="")
	{
		//if empty or not same, return this
		return false; 
	}
	//else, its found and matches so return true
	return true; 
}
bool getPin(std::string branch, std::string last4acctNo, hashMap<std::string, hashMap<std::string, bankType > >& bank)
{
	//prompt user for pin and checks if pin is found in hashMap
	std::string userPin=""; 
	cout<<"Enter a pin: ";
	cin>>userPin; 
	//pin to test 8459
	unsigned int hashPin = bank[branch][last4acctNo].pin; 
	if(bank[branch][last4acctNo].pin == std::stoi(userPin))
	{
		return true; 
	}
	return false; 
}
void viewAccount(std::string branch, std::string last4acctNo, hashMap<std::string, hashMap<std::string, bankType > >& bank)
{
	//accesses name and blance of selected person 
	cout<< "\nName: "<<bank[branch][last4acctNo].name<<endl; 
	cout<< "Balance: $"<<bank[branch][last4acctNo].balance<<endl;
	cout<<endl; 
	//then prints out transaction list if there are any present
	if(bank[branch][last4acctNo].transactions.size() > 0)
	{
		cout<<"List of transactions\n"; 
		for(int i = 0; i < bank[branch][last4acctNo].transactions.size(); ++i)
		{
			if(bank[branch][last4acctNo].transactions[i].type==true)
			{
				cout << "Deposit amount $" <<  bank[branch][last4acctNo].transactions[i].amount << endl;
			}
			else
			{
				cout << "Withdrawal amount $" <<  bank[branch][last4acctNo].transactions[i].amount<<endl;
			}
			
		}
	}
	else	
		cout<<"No transactions\n";
	cout<<endl; 
}
void withDrawal(std::string branch, std::string last$acctNo, hashMap<std::string, hashMap<std::string, bankType > >& bank)
{	
	//using users withdrawl amount checks for avaliable funds and if present does the math, if not erorr prompt 
	double money= 0; 
	cout<< "\nEnter withdrawal amount: ";
	cin>>money; 

	if(bank[branch][last$acctNo].balance < money)
	{
		cout<< "Not enough funds to make transaction\n";
		cout<<endl; 
		return; 
	}
	double currMoney = bank[branch][last$acctNo].balance - money;  
	bank[branch][last$acctNo].balance = bank[branch][last$acctNo].balance - money;
	bank[branch][last$acctNo].transactions.push_back(bankType::transactionType(false, money));
	cout<<endl; 
	
}
void deposite(std::string branch, std::string last$acctNo, hashMap<std::string, hashMap<std::string, bankType > >& bank)
{	
	//asks user for deposit amount and using transactions, pushes back the change in balance
	double money= 0; 
	cout<< "\nEnter deposit amount: ";
	cin>>money; 
	
	double currMoney = bank[branch][last$acctNo].balance + money;  
	bank[branch][last$acctNo].balance = bank[branch][last$acctNo].balance + money;
	bank[branch][last$acctNo].transactions.push_back(bankType::transactionType(true, money));

	cout<<endl; 
	return; 
}
bool startOver(string done)
{
	//asks user to startover and using allLower, converts input to lower
	cout<<"Continue? (Y/N): ";
	cin>>done; 
	string check = allLower(done); 
	if(check != "y")
	{
		//cout << done;
		return false; 
	}
	return true; 
}
int main()
{
	//declaration of variables
	std::cout << std::fixed << std::setprecision(2);
	hashMap< std::string, hashMap< std::string, bankType > > bank;
	std::fstream file;
	string loc; 
	string acct; 
	char choice; 
	bool found = true;
	bool checkAcctNo = true; 
	bool checkPin = true;
	bool exitChoice = true; 
	string done; 
	bool end= true;   
	//read in data.csv
	readData(file,bank);  
	while(end)
	{
		//user input 
		std::string currLoc = userInput(loc); 
		//user input for bank location and check if its there
		found =checkBankLocation(currLoc, bank); 
        //count for invalid pin input 
		int count = 0;
		//handle for choice loop, for v,w, and d 
        exitChoice = true;
		if(found)
		{
			//if location is found 
			std::string last4acctNo; 
			cout<<"Enter last 4 digits of account : "; 
			cin>>last4acctNo; 
			cout<<endl; 
			//if the account is locaked, skip to asking branch name 
			if(bank[currLoc][last4acctNo].locked == true)
            {
                std::cout << "Account has a lock\n";
                
				exitChoice = false; 
            }
			//if not locked, get the account number 
			checkAcctNo = getAcctNo(currLoc,last4acctNo,bank);
			//if account number found and the exitchoice is true continue to prompt 
			if(checkAcctNo && exitChoice) 
			{
				//while acoount is not loced get the pin and check if pin is correct 
				while(bank[currLoc][last4acctNo].locked == false)
				{
					checkPin= getPin(currLoc, last4acctNo, bank);
					cout<<endl; 
					if(checkPin)
					{
						//if correct, break out and offer options 
						break;  
					}
					//repromt 3 times to get correct pin and if not correct, lock the pin
					else
					{
						cout<<"Invalid Pin\n";
						cout<<endl; 			
						count++;
						if(count == 3)
						{
							bank[currLoc][last4acctNo].locked=true;
							cout<<"3 failed login attempts , account is now locked\n"; 
							//prompt user to start over and dont undo lock untill program is done and reset exit choice 
							if(!startOver(done)) 
							{
								end=false; 
								break; 
							}else{
								//condition to skip back to brancha and not account prompt 
                                exitChoice = false;
                            }
						
						}
					}
				}
				//condition to skip back to main prompt for bank branch
                if(exitChoice == false) continue;
				//while true to having a choice to exit, run throughh prompt to d,w,v,e 
				while(exitChoice)
				{
					cout<<"(D)eposit\n";
					cout<<"(W)ithdrawal\n";
					cout<<"(V)iew account\n";
					cout<<"(E)xit account\n";
					cout<<"Enter choice: "; 
					cin>>choice; 
					//user input using switch statments 
					switch (choice)
					{
						case 'D': case 'd':
							//code to add money, a function		
							deposite(currLoc, last4acctNo, bank); 
							break;
						case 'W': case 'w':	
							withDrawal(currLoc, last4acctNo, bank); 
							//code to take money, a function
							break;
						case 'V': case 'v':
							viewAccount(currLoc, last4acctNo, bank); 
							//code to add money, a function
							break;
						default: 
							//all other choices are a break, exit 
							cout<<endl; 
                            exitChoice = false; 
							break; 
					}

				}

			}
			//for when when wrong acctNo is used, print not found and prompt user to start over 
			else
			{	//condition that checks AcctNo is not found 
				if(!checkAcctNo)
				{
					cout<<"Account not found"<<endl; 
					cout<<endl; 
					if(!startOver(done)) 
						{	
							break; 
						}
					else   
						continue;
				}
	
			}
		}
		//for when wrong branch is input, print prompt and ask user if they wish to continue
		else
		{
			cout<<"Bank branch not found\n";
			cout<<endl; 
		}
        if(!startOver(done)) 
				break;  
			else continue;
	}
	return 0;
}