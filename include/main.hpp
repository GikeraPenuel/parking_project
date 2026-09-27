#ifndef PARKING_HPP
#define PARKING_HPP

#include "crow_all.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

//const int Max_Customer{200}; //using a const int as the number of customers is not more than 200

struct Customer{                                        //using struct to store variable of different types
    std::string f_name{};
    std::string s_name{};
    std::string number_plate{};                         //number plate is a string to store even letters
    std::chrono::steady_clock::time_point entryTime;    //this stores the entry time of the car
    //std::chrono::steady_clock::time_point exitTime;
    
};


struct ParkingRate{  //struct for storing rates for parking
    double halfHours{50.0};
    double twoHours{100.0};
    double fourHours{200.0};
    double sixHours{300.0}; 
    double overHours{400.0};
};

struct Parking{
    private:                                            //private members to be accessed only in the struct
        int total_slots{};
        std::vector<bool> slot_status;                  //this assigns boolean values to vectors as slot status true = empty, false = occupied
        std::vector<Customer> customers;                //this stores each customer information as a value in an array
        std::string admin_password{"admin123"};         //default hardcoded admin password
    public:
        Parking(int totalSlots)
            :total_slots(totalSlots), slot_status(totalSlots, true), customers(totalSlots){} //this is a constructor that sets totalslots
                                                                                            //sets the value of the array equal to total
                                                                                            // slots and all to be empty
                                                                                            //number of customers allowed is equal to
                                                                                            //number of totalslots

        bool verifyAdmin(const std::string& pass) const{       //authenticates if password entered is same the hardcoded one
            return pass == admin_password; 
        }

        int getTotalSlot(){                                                                 //a function to see total number  of slots
            return total_slots;                                 
        }

        bool setTotalSlots(int newTotal){       //// Dynamically adjusts total capacity and resizes backing vectors while preserving valid states
            if(newTotal <= 0){
                return false;
            }
            total_slots = newTotal;
            slot_status.resize(newTotal, true);
            customers.resize(newTotal);
            return true;
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

        //ParkingRate rate{}

        /* void setRate(ParkingRate rate){
            bool running = true;
            do{
                std::cout<< "\n SET THE RATE FOR \n";
                std::cout<< "\t 1. Half hour\n";
                std::cout<< "\t 2. Up to two hour\n";
                std::cout<< "\t 3. Up to four hours\n";
                std::cout<< "\t 4. Up to six hours\n";
                std::cout<< "\t 5. Over six hours\n";
                std::cout<< "\t 0. EXIT\n";
                
                int choice{};
                std::cout<<"Enter your choice #: ";

                if(!(std::cin>> choice)){
                    std::cout<<"\n \a Error: Invalid input format. Please enter a number.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    continue;
                }
                if(choice >= 0 && choice <= 5){
                    switch(choice){
                        case 0:{
                            running = false;
                            break;
                        }
                        case 1:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.halfHours <<" for half an hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.halfHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.halfHours <<" for half an hour";
                                
                            }
                            else{
                                std::cout<< "\n\a ERROR\n";
                                std::cout<< "Invalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 2:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.twoHours <<" up to two hours";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.twoHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.twoHours <<" up to two hours";
                                
                            }
                            else{
                                std::cout<< "\n\a ERROR\n";
                                std::cout<< "Invalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 3:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.fourHours <<" up to four hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.fourHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.fourHours <<" up to four hour";
                                
                            }
                            else{
                                std::cout<< "\n\a ERROR\n";
                                std::cout<< "Invalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 4:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.sixHours <<" up to six hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.sixHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.sixHours <<" up to six hour";
                                
                            }
                            else{
                                std::cout<< "\n\a ERROR\n";
                                std::cout<< "Invalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 5:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.overHours <<" for over six hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.overHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.overHours <<" for over six hour";
                                
                            }
                            else{
                                std::cout<< "\n\a ERROR\n";
                                std::cout<< "Invalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                    }
                }
                else{
                    std::cout<<"\n ERROR!! Invalid choice\n";
                }
            }while(running);
        };
        */

        int charges(int time, ParkingRate const useRate){                                          //function to calculate the charges to be paid
            
            if(time <= 30){

                return useRate.halfHours;
            }

            else if(time <= 120){

                return useRate.twoHours;
            }

            else if(time <=240){

                return useRate.fourHours;
            }

            else if(time <= 360){

                return useRate.sixHours;
            }
            else{

                return useRate.overHours;
            }

        }

        crow::json::wvalue checkOut(int slotIndex, ParkingRate const nowRate){         // Processes checkout for a given slot:
            crow::json::wvalue res;
            if(slotIndex < 0 || slotIndex >= total_slots){      //check if slot exists
                res["success"] = false;
                res["message"] = "invalid slot number!";
                return res;
            }
            if(slot_status[slotIndex]){        //checks if slot is occupied or empty
                res["success"] = false;
                res["message"] = "Slot is already empty!";
                return res;
            }

            auto currentTime = std::chrono::steady_clock::now(); //time during checkout

            //auto checkoutTime = parkinglot.getTime(slotno);
                                                                                   
            auto elapsed = std::chrono::duration_cast<std::chrono::minutes> (currentTime - customers[slotIndex].entryTime);   //the elapsed time is calculated

            
            int fees{};
            fees = charges(elapsed.count(), nowRate);                    //fees is assigned the charges to be paid

            
            // Populate successful response JSON payload
            res["success"] = true;
            res["slot"] = slotIndex;
            res["plate"] = customers[slotIndex].number_plate;
            res["time_parked"] = elapsed.count();
            res["fees"] = fees;

            slot_status[slotIndex] = true;                  //slotstatus is set to true which means empty
            customers[slotIndex] = Customer{};              //resets customer information in that slot

            return res;
            
        }

        
        crow::json::wvalue getSlotInfo(bool isAdmin) const {      // Serializes all slots into JSON format; sanitizes sensitive occupant data if requester is not an admin
            crow::json::wvalue list = crow::json::wvalue::list();

            for(int i = 0; i < total_slots; i++){
                crow::json::wvalue item;
                item["slot_no"] = i;
                item["is_empty"] = slot_status[i];


                if(isAdmin && !slot_status[i]){ //prints all info if it is an admin
                    item["occupant"] = customers[i].f_name + " " + customers[i].s_name;
                    item["plate"] = customers[i].number_plate;
                }
                list[i] = std::move(item);
            }
            return list;
       }
};

/* ==========================================
 * LEGACY CLI SETTERS & RENDERERS
 * ==========================================
 * The following methods were used prior to migrating the application to Crow REST APIs.

        // Interactive CLI menu for adjusting rates on console input
        void setRate(ParkingRate rate){
            bool running = true;
            do{
                std::cout<< "\n SET THE RATE FOR \n";
                std::cout<< "\t 1. Half hour\n";
                std::cout<< "\t 2. Up to two hour\n";
                std::cout<< "\t 3. Up to four hours\n";
                std::cout<< "\t 4. Up to six hours\n";
                std::cout<< "\t 5. Over six hours\n";
                std::cout<< "\t 0. EXIT\n";
                
                int choice{};
                std::cout<<"Enter your choice #: ";

                if(!(std::cin>> choice)){
                    std::cout<<"\n \a Error: Invalid input format. Please enter a number.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    continue;
                }
                if(choice >= 0 && choice <= 5){
                    switch(choice){
                        case 0:{
                            running = false;
                            break;
                        }
                        case 1:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.halfHours <<" for half an hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.halfHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.halfHours <<" for half an hour";
                            }
                            else{
                                std::cout<< "\n\a ERROR\nInvalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 2:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.twoHours <<" up to two hours";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.twoHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.twoHours <<" up to two hours";
                            }
                            else{
                                std::cout<< "\n\a ERROR\nInvalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 3:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.fourHours <<" up to four hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.fourHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.fourHours <<" up to four hour";
                            }
                            else{
                                std::cout<< "\n\a ERROR\nInvalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 4:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.sixHours <<" up to six hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.sixHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.sixHours <<" up to six hour";
                            }
                            else{
                                std::cout<< "\n\a ERROR\nInvalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                        case 5:{
                            std::cout<< "\ncurrent parking rate is:KSH "<< rate.overHours <<" for over six hour";
                            std::cout<< "\n Enter new rate:";
                            if(std::cin>> rate.overHours){
                                std::cout<< "\n CHANGE SUCCESSFUL \n";
                                std::cout<< "\n New parking rate is:KSH "<< rate.overHours <<" for over six hour";
                            }
                            else{
                                std::cout<< "\n\a ERROR\nInvalid Input\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            break;
                        }
                    }
                }
                else{
                    std::cout<<"\n ERROR!! Invalid choice\n";
                }
            }while(running);
        }

        // Returns entry time point for a slot
        auto getTime(int slotIndex){
            return customers[slotIndex].entryTime;
        }

        // Terminal printer for displaying slot occupant information
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
*/

#endif