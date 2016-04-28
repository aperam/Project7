/**
 Ryan Morvay - morvayr@email.sc.edu
 Joseph Bloxham - bloxhamj@email.sc.edu
 Ian Osea - osea@email.sc.edu
 Project 7
 CSCE 240 
 April 27, 2016
*/

#include <iostream>
#include <ctime>
#include <iterator>
#include <list>
#include <queue>
#include <vector>
#include <stdlib.h>
#include "main.h"

using namespace std;


class Bank
{
  public:
  	int goodTellers, badTellers;
  	int managers;
  	double time;
  
  	bool busy;
  	bool open;
  	
  	
  	bool bankState()
  	{
  	 if (time < 0)
  	{
  	 cout<<"The bank is closed";
    open == FALSE;
    return open;
  	}
  	 else if( time > 288000)
  	 {
  	  cout<<"The bank is closed";
  	  open == FALSE;
  	  return open;
  	 }
  	 else
  	 {
  	  cout<<"The bank is open";
  	  open == TRUE;
  	  return open;
  	 }
  	}	
   
  	bool isBusy()
    { 
      return busy;
    }
    
   

};

class Customer
{
  int serviceTime;
    public:
      Customer() : serviceTime(0) {}
      Customer(int tm) : serviceTime(tm) {}
      int getTime() {return serviceTime;}
      void setTime(int newtime) {serviceTime = newtime;}
    
    friend ostream& operator<<(ostream& os, const Customer& c)
    {
       return os << '[' << c.serviceTime << ']';
    }
};
 
class Teller
{
  //creating the queue to hold the customers
  queue<Customer>& customers;
  //creating new customer object
  Customer current;
  //setting time slots for tellers
  enum {SEGMENT = 5};
  //Remaining time in each segment 
  int remainingTime;
  //boolean representing if a teller is busy with a customer
  bool busy;

  public:
    Teller(queue<Customer>& customerQueue) : customers(customerQueue), remainingTime(0), busy(false) {}
       
    Teller& operator=(const Teller& rv)
    {
      customers = rv.customers;
      current = rv.current;
      remainingTime = rv.remainingTime;
      busy = rv.busy;
      return *this;
    }

    bool isBusy()
    { 
      return busy;
    }

    void helpCustomers(bool recursion = false)
    {
      if(!recursion)
      {
        remainingTime = SEGMENT;
      }  
        
      int timeServed = current.getTime();

      if(timeServed > remainingTime)
      {
        timeServed -= remainingTime;
        current.setTime(timeServed);
        //Continue working on current
        busy = true;
        return;
      }

      if(timeServed < remainingTime)
      {
        remainingTime -= timeServed;
   
        if(!customers.empty())
        {
          current = customers.front();
          cout << "Removing customer\n"; 
          customers.pop(); 
          busy = true;
          helpCustomers(true); // Recurse
        }
        
        return;
      }

      if(timeServed == remainingTime)
      {
        //Done with current customers
        //set to empty
        current = Customer(0);
        busy = false;
        
        //no more time left
        return;
      }
    }
};
 
//Inherit to access protected implementation:
class customerQueue : public queue<Customer>
{
  public:
   friend ostream& operator<<(ostream& outStream, const customerQueue& custQueue)
   {
     copy(custQueue.c.begin(), custQueue.c.end(), ostream_iterator<Customer>(outStream, " "));
     return outStream;
   }
};

 
int main()
{
  //create new customers object	
  customerQueue customers;
  
  //create tellers list
  list<Teller> tellers;

  //Creating iterator for tellers
  typedef list<Teller>::iterator tellersIter;
  
  //adds the customers to the teller queue
  tellers.push_back(Teller(customers));
  
  //random number generator
  srand(time(0));
  
  //keeping track of run time
  clock_t ticks = clock();

  cout << "Starting Bank Simulation..." << endl;
  
  //Run simulation for at least 5 seconds:
  while(clock() < ticks + 5 * CLOCKS_PER_SEC)
  {
    //Add a random number of customers to the
    //queue, with random service times:
    for(int i = 0; i < rand() % 5; i++)
    {	
      customers.push(Customer(rand() % 15 + 1));
      cout << "Teller: " << tellers.size() << " Customers: " << customers << endl;
    }

    //Have the tellers service the queue:
    for(tellersIter i = tellers.begin(); i != tellers.end(); i++)
    {
      (*i).helpCustomers();
    }

    cout << "Teller: " << tellers.size() << " Customers: " << customers << endl;

    //If line is too long, add another teller:
    if(customers.size() / tellers.size() > 2)
    {
       cout << "Line is too long, adding another teller...\n";
       tellers.push_back(Teller(customers));
    }  

    //If line is short enough, remove a teller:
    if(tellers.size() > 1 && customers.size() / tellers.size() < 2)
    {
      for(tellersIter i = tellers.begin(); i != tellers.end(); i++)
      {
        if(!(*i).isBusy())
        {
          cout << "The lines are short, so a teller is removed\n";
          tellers.erase(i);
          //Break the for loop
          break;
        }
      }  
    }
  }
  cout << "\nEnd of simulation...Goodbye\n";
}  


