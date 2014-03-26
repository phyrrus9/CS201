//
//  Customer.h
//  Project4
//
//  Created by Ethan Laur on 2/11/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef Project4_Customer_h
#define Project4_Customer_h

typedef int DWORD;
typedef short WORD; //make it more windows friendly
typedef std::string String;

class Customer
{
private:
	DWORD arrivaltime, servicetime, departuretime;
	String customer_name;
public:
	int num;
	Customer() {
		setArrivalTime(0);
		setServiceTime(0);
		setDepartureTime(getDepartureTime());
	}
	Customer(DWORD arrivaltime_in, DWORD servicetime_in, String customer_name_in) {
		setArrivalTime(arrivaltime_in);
		setServiceTime(servicetime_in);
		setDepartureTime(getDepartureTime());
		setCustomerName(customer_name_in);
	}
	Customer(Customer &old) {
		setArrivalTime(old.getArrivalTime());
		setServiceTime(old.getServiceTime());
		setDepartureTime(old.getDepartureTime());
		setCustomerName(old.getCustomerName());
	}
	DWORD getDepartureTime() {
		return departuretime;
	}
	DWORD getArrivalTime() {
		return arrivaltime;
	}
	DWORD getServiceTime() {
		return servicetime;
	} 
	String getCustomerName() {
		return customer_name;
	}
	void setDepartureTime(DWORD departuretime_in) {
		departuretime = departuretime_in;
	}
	void setArrivalTime(DWORD arrivaltime_in) {
		arrivaltime = arrivaltime_in;
	}
	void setServiceTime(DWORD servicetime_in) {
		servicetime = servicetime_in;
	}
	void setCustomerName(String customer_name_in) {
		customer_name = customer_name_in;
	}
};

#endif
