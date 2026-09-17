#include "main.hpp"
#include <limits>

int main(){
    Parking parkinglot(5);

    //int remain{0};

    //int entry{0};

    while(true){
        
        std::cout<< "\t WELCOME \t \t \n";
        std::cout<< "\t 1. CHECK IN \n";
        std::cout<< "\t 2. CHECK OUT \n";
        std::cout<< "\t 3. SLOT INFO\n";

        std::cout<< "Enter a # choice: ";
        int choice{};
        //std::cin>> choice;

        if (!(std::cin >> choice)) {
            std::cout << "\nError: Invalid input format. Please enter a number.\n";
            std::cin.clear(); // Resets the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discards bad input
            continue;
        }

        if(choice == 1 || choice == 2 || choice == 3){
            switch(choice){
                case 1: {
                    if(parkinglot.getEmptySlots() <= 0){
                        std::cout<< "NO AVAILABLE SLOTS... RETURN LATER \n";
                    }
                    else{
                       

                        Customer c{};
                        std::cout<< "enter first name: ";
                        std::cin>> c.f_name;

                        std::cout<< "enter second name: ";
                        std::cin>> c.s_name;

                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        std::cout<< "enter vehicle number plate: ";
                        std::getline(std::cin, c.number_plate);

                        int slot = parkinglot.addCustomer(c);

                        std::cout<< "\n you have been assigned SLOT #"<< slot<< "\n\n";

                        
                        // parkinglot.printInfo(slot);
                    }
                    break;
                }

                case 2: {
                    int slotno{};
                    std::cout<< "Enter your parking slot number #: ";
                    std::cin>> slotno;

                    parkinglot.checkOut(slotno);
                    break;
                }

                case 3: {
                    for(int i = 0; i < parkinglot.getTotalSlot(); i++){
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