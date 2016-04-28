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
#include <fstream>
#include <stdlib.h>
#include "main.h"

using namespace std;


class Bank
{
  public:
  	//initilize all variables used in this class
  	int goodTellers, badTellers, managers;
  	double time = 60;
  	bool busy = true;
  	bool open;
  	
  	//boolean function for telling if the bank is open or closed
  	bool bankState()
  	{
  	  //checks to see if the bank is open
  	  if (time < 0)
  	  {
  	    cout << "\nThe bank is closed\n";
        //sets the bank status to closed which is false
        open == false;
        //returns status
        return open;
  	  }
  	  else if( time > 28800)
  	  {
  	    cout << "\nThe bank is closed\n";
  	    //sets the bank status to closed which is false
  	    open == false;
  	    //returns status
  	    return open;
  	  }
  	  else
  	  {
  	    cout << "\nThe bank is now open for service.\n";
  	    cout << "\n";
  	    //sets the bank status to open which is true
  	    open == true;
  	    //returns status
  	    return open;
  	  }
  	}	
   
    //function for telling whether the bank is busy or not
  	bool isBusy()
    { 
      return busy;
    } 
};

class Customer
{
  //variable to keep track of amount of time customers is being served
  int serviceTime;
    public:
      //constructor for customer
      Customer() : serviceTime(0) {}
      Customer(int tm) : serviceTime(tm) {}
      
      //function to get the service time 
      int getTime() {return serviceTime;}
      
      //function to set the service time to a new time
      void setTime(int newtime) {serviceTime = newtime;}
    
    //overloaded operator for formating customer queue
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
    //constructor for the teller class
    Teller(queue<Customer>& customerQueue) : customers(customerQueue), remainingTime(0), busy(false) {}
       
    //overloaded operator for teller
    Teller& operator=(const Teller& temp)
    {
      customers = temp.customers;
      current = temp.current;
      remainingTime = temp.remainingTime;
      busy = temp.busy;
      return *this;
    }

    //function for the good tellers
    void goodTeller()
    {
      
    }

    //function for the bad tellers
    void badTeller()
    {

    }

    //function to see if the teller is busy or not
    bool isBusy()
    { 
      return busy;
    }

    //funtion to simulate tellers helping customers
    void helpCustomers(bool customerCheck = false)
    {
      //checks if time is left after being served
      if(!customerCheck)
      {
      	//sets the new time 
        remainingTime = SEGMENT;
      }  
        
      //  
      int timeServed = current.getTime();

      if(timeServed > remainingTime)
      {
        timeServed -= remainingTime;
        current.setTime(timeServed);
        //Continue working on current
        busy = true;
        return;
      }

      //
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
   //overloaded operator for the output
   friend ostream& operator<<(ostream& outStream, const customerQueue& custQueue)
   {
   	 //formats the customer queue and returns the output
     copy(custQueue.c.begin(), custQueue.c.end(), ostream_iterator<Customer>(outStream, " "));
     return outStream;
   }
};

class manager
{
  public:
  	//two states for the manager
  	bool visit = false;
  	bool office = false;

    //creates new bank object
    Bank startDay;

    //run function to set the manager's state at the start of the business day
  	bool run()
  	{
  	  //checks to see if the bank is open 	
      if(startDay.bankState() == true)
      {
      	//sets the status of the manager to be in the office
        office == true;
        return office;
      }	
      else
      {
      	//sets the status of the manager to be in the office
      	office == false;
      	return office;
      }
  	}
};
 
int main()
{
  //Output file for the bank report
  ofstream outFile("bankReport.txt");

  //variable to keep track of score
  long bankScore = 0.0;
 
  //creates new bank object
  Bank newDay;

  //create new customers object	
  customerQueue customers;
  
  //create tellers list
  list<Teller> tellers;

  //Creating iterator for tellers
  typedef list<Teller>::iterator tellersIter;
  
  //adds the customers to the teller queue
  tellers.push_back(Teller(customers));
  
  //random number generator for keeping track of clock time
  srand(time(0));
  
  //keeping track of run time
  clock_t ticks = clock();

  outFile << "Bank Report\n" << endl;

  cout << "Starting Bank Simulation..." << endl;
  outFile << "Starting Bank Simulation..." << endl;

  //checks to see if the bank is open or closed for the day
  newDay.bankState();
  
  //Run simulation for at least 5 seconds:
  while(clock() < ticks + 5 * CLOCKS_PER_SEC)
  {
    //for loop to add customers to the queue
    for(int i = 0; i < rand() % 5; i++)
    {	
      //adding customers to the queue	
      customers.push(Customer(rand() % 15 + 1));
      cout << "\nNumber of tellers: " << tellers.size() << " \nServing customers: " << customers << endl;
      outFile << "\nNumber of tellers: " << tellers.size() << " \nServing customers: " << customers << endl;
    }

    //Tellers start to help customers in the queue
    for(tellersIter i = tellers.begin(); i != tellers.end(); i++)
    {
      //going through customer queue and running the FIFO simulation
      (*i).helpCustomers();
      //adjust bank score each time customer is served
      bankScore += 10;
    }

    cout << "\nNumber of tellers: " << tellers.size() << " \nServing customers: " << customers << endl;
    outFile << "\nNumber of tellers: " << tellers.size() << " \nServing customers: " << customers << endl;

    //Checks to see if the customer line is too long and adds a teller, if necessary
    if(customers.size() / tellers.size() > 2)
    {
       cout << "\nLine is too long, adding another teller...\n";
       outFile << "\nLine is too long, adding another teller...\n";

       //adding a teller due to a long customer line
       tellers.push_back(Teller(customers));
       //adjusting bank score
       bankScore -= 10;
    }  

    //checks to see if line is short enough to remove a teller
    if(tellers.size() > 1 && customers.size() / tellers.size() < 2)
    {
      //iterating throught the tellers to find one that is not busy to be removed
      for(tellersIter i = tellers.begin(); i != tellers.end(); i++)
      {
        if(!(*i).isBusy())
        {
          cout << "\nThe lines are short, so a teller is removed\n";
          outFile << "\nThe lines are short, so a teller is removed\n";
          //remove the teller that is not currently busy
          tellers.erase(i);
          //adjust bank score
          bankScore -= 10;
          //Break out of the for loop
          break;
        }
      }  
    }
  }

  //output bank score and other information 
  cout << "\nBank Score: " << bankScore;
  outFile << "\nBank Score: " << bankScore;
  cout << "\nEnd of simulation.\n Check the BankReport.txt file for the bank report simulation output.\n";
  cout << "\n";
  outFile << "\nEnd of simulation...Goodbye\n";
}