#ifndef PARKING_HPP
#define PARKING_HPP

#include <iostream>
#include <string>
#include <vector>

const int Max_Customer{200};

struct Customer{
    std::string f_name{};
    std::string s_name{};
    std::string number_plate{};
    
};

struct Parking{
    private:
        int total_slots{};
        std::vector<bool> slot_status;
        std::vector<Customer> customers;

    public:
        Parking(int totalSlots)
            :total_slots(totalSlots), slot_status(totalSlots, true), customers(totalSlots){}

        int getTotalSlot(){
            return total_slots;
        }

        int getEmptySlots() const {
            int count = 0;
            for(bool empty : slot_status){
                if(empty){
                    ++count;
                }
            }
            return count;
        }

        int addCustomer(const Customer& newCustomer){
            for(int i = 0; i < total_slots; i++){
                if(slot_status[i]){
                    slot_status[i] = false;
                    customers[i] = newCustomer;
                    return i; 
                }
            }
            return -1;
        }

        bool checkOut(int slotIndex){
            if(slotIndex < 0 || slotIndex >= total_slots){
                std::cout<< "ERR!! INVALID SLOT NUMBER \n";
                return false;
            }
            if(slot_status[slotIndex]){
                std::cout<< "Slot #"<< slotIndex << " is already empty \n";
                return false;
            }

            std::cout<< "CAR: "<< customers[slotIndex].number_plate << " has checked out \n";
            std::cout<< "Slot no #"<< slotIndex << " is now empty";

            slot_status[slotIndex] = true;
            customers[slotIndex] = Customer{};

            return true;
            
        }

        void printInfo(int slotIndex) const{
            if(slotIndex >= 0 && slotIndex < total_slots){
                if(!slot_status[slotIndex]){
                    std::cout<< "\n \t \t Slot no #" << slotIndex<< "\n";
                    std::cout<< "\t \t occupant: ";
                    std::cout << customers[slotIndex].f_name << " ";
                    std::cout << customers[slotIndex].s_name << "\n";
                    std::cout << "\t \t PLATE: " << customers[slotIndex].number_plate << "\n";
                }
                else{
                    std::cout<< "Slot no #"<< slotIndex << " is empty. \n";
                }
            }
        }
};

#endif