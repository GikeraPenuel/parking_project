#include "../include/main.hpp"

#include <limits>

int main(){
    Parking parkinglot(5);
    ParkingRate rate{};

    //int remain{0};

    //int entry{0};

    while(true){
        
        std::cout<< "\t WELCOME \t \t \n";          //main menu
        std::cout<< "AVAILABLE SLOTS: "<< parkinglot.getEmptySlots()<<"\n"; 
        std::cout<< "\t 1. CHECK IN \n";
        std::cout<< "\t 2. CHECK OUT \n";
        std::cout<< "\t 3. SLOTS INFO\n";
        std::cout<< "\t 0. ADMIN\n";


        std::cout<< "Enter a # choice: ";
        int choice{};                               //accepts user info
        //std::cin>> choice;

        if (!(std::cin >> choice)) {                //this checks if valid input has been added
            std::cout << "\nError: Invalid input format. Please enter a number.\n";
            std::cin.clear();                       // Resets the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discards bad input and input buffer
            continue;
        }

        if(choice == 0 ||choice == 1 || choice == 2 || choice == 3){          //ensures only valid choices are in
            switch(choice){
                case 0: {
                    parkinglot.setRate(rate);
                    break;
                }
                case 1: {
                    if(parkinglot.getEmptySlots() <= 0){
                        std::cout<< "NO AVAILABLE SLOTS... RETURN LATER \n";        //does not continue if no slots available to check in
                    }
                    else{
                       

                        Customer c{};
                        std::cout<< "enter first name: ";
                        std::cin>> c.f_name;

                        std::cout<< "enter second name: ";
                        std::cin>> c.s_name;

                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignores input buffer

                        std::cout<< "enter vehicle number plate: ";     
                        std::getline(std::cin, c.number_plate);                 //can accept a white space

                        c.entryTime = std::chrono::steady_clock::now();         //records entry time to elemt in struct of customer

                        int slot = parkinglot.addCustomer(c);                   //slot is assigned the index of customer slot in array

                        std::cout<< "\n you have been assigned SLOT #"<< slot<< "\n\n";

                        
                        // parkinglot.printInfo(slot);
                    }
                    break;
                }

                case 2: {                                               //for checking out
                    int slotno{};
                    std::cout<< "Enter your parking slot number #: ";
                    std::cin>> slotno;

                    /*
                    auto currentTime = std::chrono::steady_clock::now();

                    auto checkoutTime = parkinglot.getTime(slotno);

                    auto elapsed = std::chrono::duration_cast<std::chrono::minutes> (currentTime - checkoutTime);


                    int fees{};
                    fees = parkinglot.charges(elapsed.count());

                    

                    
                    if(parkinglot.checkOut(slotno)){
                        std::cout << "\n time parked: " << elapsed.count() << "min\n";
                        std::cout<< "THUS \n";
                        std::cout<< "you are to pay KSH"<< fees;
                    }
                    */

                    parkinglot.checkOut(slotno, rate);                            //calls the check out function

                    break;
                }

                case 3: {
                    for(int i = 0; i < parkinglot.getTotalSlot(); i++){             //prints all info about taken slots
                        parkinglot.printInfo(i);
                    }

                    std::cout<< "\nthere are -"<< parkinglot.getEmptySlots() <<"- empty slots\n";
                    break;
                }

                default:
                    return 1;
            }
        }
        else{ 
            std::cout<< " \n error !!!!! you have entered invalid choice \n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; 
        }
    }

    return 0;
}