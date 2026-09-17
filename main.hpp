#ifndef PARKING_HPP
#define PARKING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

const int Max_Customer{200}; //using a const int as the number of customers is not more than 200

struct Customer{                                        //using struct to store variable of different types
    std::string f_name{};
    std::string s_name{};
    std::string number_plate{};                         //number plate is a string to store even letters
    std::chrono::steady_clock::time_point entryTime;    //this stores the entry time of the car
    //std::chrono::steady_clock::time_point exitTime;
    
};

struct Parking{
    private:                                            //private members to be accessed only in the struct
        int total_slots{};
        std::vector<bool> slot_status;                  //this assigns boolean values to vectors as slot status
        std::vector<Customer> customers;                //this stores each customer information as a value in an array

    public:
        Parking(int totalSlots)
            :total_slots(totalSlots), slot_status(totalSlots, true), customers(totalSlots){} //this is a constructor that sets totalslots
                                                                                            //sets the value of the array equal to total
                                                                                            // slots and all to be empty
                                                                                            //number of customers allowed is equal to
                                                                                            //number of totalslots


        int getTotalSlot(){                                                                 //a function to see number of slots
            return total_slots;                                 
        }

        int getEmptySlots() const {                                                         //a function to get empty slots
            int count = 0;
            for(bool empty : slot_status){                                                  // a loop that sweeps all slot status 
                if(empty){                                                                  //every slot that is true increases the count
                    ++count;                                                                //the end result count is returned as no of e.slots
                }
            }
            return count;
        }

        int addCustomer(const Customer& newCustomer){                   //a function to add new customer to first available slot
            for(int i = 0; i < total_slots; i++){                       //a loop for scrolling through all slots
                if(slot_status[i]){                                     //if the slotstatus is true which means empty
                    slot_status[i] = false;                             //the slotstatus is set to false as it is now taken
                    customers[i] = newCustomer;                         //the particular slot is assigned to new customer
                    return i;                                           //slot number is returned to user
                }
            }
            return -1;
        }

        int charges(int time){                                          //function to calculate the charges to be paid
            int rate{};
            if(time <= 30){
                rate = 0;

                return rate;
            }

            else if(time <= 120){
                rate = 50;

                return rate;
            }

            else if(time <=240){
                rate = 100;

                return rate;
            }

            else if(time <= 360){
                rate = 300;

                return rate;
            }
            else{
                rate = 500;

                return rate;
            }

        }

        bool checkOut(int slotIndex){                           //function to checkout, set as bool so can return a true,
            if(slotIndex < 0 || slotIndex >= total_slots){      //which means slot is now empty
                std::cout<< "ERR!! INVALID SLOT NUMBER \n";         //this is displayed if slot entered does not exist
                return false;
            }
            if(slot_status[slotIndex]){
                std::cout<< "Slot #"<< slotIndex << " is already empty \n";  //this is displayed if user try to checkout empty slot
                return false;
            }

            auto currentTime = std::chrono::steady_clock::now(); //time during checkout

            //auto checkoutTime = parkinglot.getTime(slotno);
                                                                                    //the elapsed time is calculated
            auto elapsed = std::chrono::duration_cast<std::chrono::minutes> (currentTime - customers[slotIndex].entryTime);


            int fees{};
            fees = charges(elapsed.count());                    //fees is assigned the charges to be paid

            std::cout << "\n time parked: " << elapsed.count() << "min\n";
            std::cout<< "THUS \n";
            std::cout<< "you are to pay KSH"<< fees<< "\n\n";

            std::cout<< "CAR: "<< customers[slotIndex].number_plate << " has checked out \n";
            std::cout<< "Slot no #"<< slotIndex << " is now empty";

            slot_status[slotIndex] = true;                  //slotstatus is set to true which means empty
            customers[slotIndex] = Customer{};              //resets customer information in that slot

            return true;
            
        }

        /*
        auto getTime(int slotIndex){
            return customers[slotIndex].entryTime;
        }
        */

        void printInfo(int slotIndex) const{                            //prints all info of customer in slot x
            if(slotIndex >= 0 && slotIndex < total_slots){
                if(!slot_status[slotIndex]){
                    std::cout<< "\n \t \t Slot no #" << slotIndex<< "\n";
                    std::cout<< "\t \t occupant: ";
                    std::cout << customers[slotIndex].f_name << " ";
                    std::cout << customers[slotIndex].s_name << "\n";
                    std::cout << "\t \t PLATE: " << customers[slotIndex].number_plate << "\n";
                }
                else{                                                   //prints if no customer is assigned there
                    std::cout<< "Slot no #"<< slotIndex << " is empty. \n";
                }
            }
        }
};

#endif