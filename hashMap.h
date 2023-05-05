#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

template <class t1, class t2>
class hashMap
{
public:
	hashMap(std::size_t = 5);
	t2& operator[](t1);
private:
	struct keyVal
	{
		keyVal()
		{
			key = t1();
			value = t2();
		}

		t1 key;
		t2 value;
	};

	void resize(std::size_t);

	std::size_t hash1(std::string);
	std::size_t hash2(std::string);

	std::vector<keyVal> table1;
	std::vector<keyVal> table2;

	std::size_t items1;
	std::size_t items2;
};

template <class t1, class t2>
hashMap<t1, t2>::hashMap(std::size_t size)
{
	//set items to 0 and resize table to size 
	items1 = items2 = 0; 
	table1.resize(size);
	table2.resize(size); 
}

template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
	//collison counters 
	int i1 = 0, i2 = 0;
	//if load factor is 20% or higher resize 
	double loadFactor1 = items1 / (double)(table1.size()); 
	double loadFactor2 = items2 / (double)(table2.size()); 
	if(loadFactor1 >= 0.20 || loadFactor2 >=0.20 || items1 <3)
	{
		//call oobject *this resize so works for table one and two 
		this->resize(table1.size());
	}
	//reset index for the hash maps 
	std::size_t index1 = hash1(key)%table1.size(); 
	std::size_t index2 = hash2(key)%table2.size(); 
	int counter = 0;
	//run hashMap search, using key values to compare
	while(true)
	{
		//if index1 is found and matches the key in table1 
		if(table1[index1].key == key)
		{
			return table1[index1].value; 
		}
		//if index1 is in table1 and empty, populate said index and inc index1
		else if(table1[index1].key==t1())
		{
			//making an entry for a blank 
			table1[index1].key = key; 
			items1+= 1; 
			return table1[index1].value; 
		}
		// else condition will account fo this {if(table1[index1].key != key && table1[index1].key!=t1())}
		else{
			//collison occurd in table one, inc by one and iterat table 2
			i1+=1; 
				//if index is found in table2 and key value matches
				if(table2[index2].key == key)
				{
					return table2[index2].value; 
				}
				//if index is found in table is and is empty, populate and inc index2 
				else if(table2[index2].key==t1())
				{
					//making an entry for a blank 
					table2[index2].key = key; 
					items2+= 1; 
					return table2[index2].value; 
				}
				//collision occurd in table 1 and now 2, so resize index
				else{
					//collision found in table2, inc and resize both indexs
					i2+=1; 
					//for even iteration 
					if(counter%2 == 0)
					{
						index1 = (index1 + i1 * hash2(key)) % table1.size();
						index2 = (index2 + i2 * hash1(key)) % table2.size(); 
					}
					//for odd iteration 
					else
					{
						index1 = (index1 + i1 * hash1(key)) % table1.size(); 
						index2 = (index2 + i2 * hash2(key)) % table2.size(); 
					}
				}
		

		}
		counter +=1; 
	}

}

template <class t1, class t2>
void hashMap<t1, t2>::resize(std::size_t amount)
{
	//temp vector obj 
	std::vector<keyVal> obj; 
	std::vector<keyVal> obj2; 
	//preallocate space for vector
	obj.reserve(table1.size()); 
	obj2.reserve(table2.size()); 

	//go through and make a copy and erase elements from the table 
	for(auto it = table1.begin(); it!= table1.end(); ++it)
	{
		obj.push_back(*it);
	}
	//erase the elements
	table1.clear(); 
	//repeat for table2 
	for(auto it2 = table2.begin(); it2!= table2.end(); ++it2)
	{
		obj2.push_back(*it2);
	}
	table2.clear();

	//resize the table
	table1.resize(amount*2);  //idk if this is right but i tried -\-_-/-
	table2.resize(amount*2); 

	//now refill the newly resized table with elements from the object vectors no empty spots 
	for(auto it =obj.begin(); it!=obj.end(); ++it)
	{
										
		std::size_t hValue = hash1(it->key); 		//calc the hash value using hash1
			table1[hValue % table1.size()] = *it;       //at least thats what im trying to do below
	}													  
	//repeat for table2 
	//to run the empty, might need to use an int i and not it
	for(auto it2= obj2.begin(); it2!=obj2.end(); ++it2)
	{

		std::size_t hValue = hash2(it2->key); 
			table2[hValue % table2.size()] = *it2;
		
	}
}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash1(std::string key)
{	
	//k denotes the length of the string key
	//each index of k, will be multiplied to power of 10, per iteration of i 
	size_t k = key.length(); 
	for(int i =0; i<= key.length(); ++i)
	{
		k+= (key[i] * pow(10, i)); 
	}
	return k; 
}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash2(std::string key)
{
	//k denotes the length of the string key
	//sum is the second last index to the power of 10, per each iteration 
	size_t k =key.length();
	size_t sum = 0;  
	for(int i = 0; i <= key.length(); ++i)
	{
		sum+= (key[k-1-i] * pow(10, i));
	}
	return sum; 
}
