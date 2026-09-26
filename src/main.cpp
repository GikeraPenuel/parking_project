#include "../include/main.hpp"

#include <limits>

int main(){
    crow::SimpleApp app;

    

    Parking parkinglot(5);
    ParkingRate rate{};

    
    //serve HTML dash
    
    CROW_ROUTE(app, "/")([](){
        crow::response res;
        res.set_static_file_info("public/index.html");
        return res;

    });

    // Serve style.css
    CROW_ROUTE(app, "/style.css")([](){
        crow::response res;
        res.set_static_file_info("public/style.css");
        return res;
    });

    // Serve script.js
    CROW_ROUTE(app, "/script.js")([](){
        crow::response res;
        res.set_static_file_info("public/script.js");
        return res;
    });

    //Admin authnetication
    CROW_ROUTE(app, "/api/admin/login").methods(crow::HTTPMethod::POST)([&parkinglot](const crow::request& req){
        auto body = crow::json::load(req.body);
        crow::json::wvalue res;

        if(!body || !body.has("password")){
            res["success"] = false;
            res["success"] = "Password required.";
            return crow::response(400, res);
        }

        if(parkinglot.verifyAdmin(body["password"].s())){
            res["success"] = true;
            res["message"] = "Login successful!";
            return crow::response(200, res);
        }
        else{
            res["success"] = false;
            res["message"] = "Incorrect admin password";
            return crow::response(401, res);
        }
    });

    //getting status and removing slot occupant info if not admin
    CROW_ROUTE(app, "/api/status").methods(crow::HTTPMethod::GET)([&parkinglot](const crow::request& req){
        std::string authPass = req.get_header_value("x-admin-pass");
        bool isAdmin = parkinglot.verifyAdmin(authPass);

            crow::json::wvalue response;
            response["total_slots"] = parkinglot.getTotalSlot();
            response["empty_slots"] = parkinglot.getEmptySlots();
            response["slots"] = parkinglot.getSlotInfo(isAdmin);
            return response;
    });

    //check in
    CROW_ROUTE(app, "/api/checkin").methods(crow::HTTPMethod::POST)([&parkinglot](const crow::request& req){
        auto body = crow::json::load(req.body);
        crow::json::wvalue res;

        if(!body){
            res["success"] = false;
            res["message"] = "Invalid JSON data.";
            return crow::response(400, res);
        }

        if(parkinglot.getEmptySlots() <= 0){
            res["success"] = false;
            res["message"] = "NO AVAILABLE SLOTS ... RETURN LATER";
            return crow::response(400, res);
        }

        Customer c{};
        c.f_name = body["f_name"].s();
        c.s_name = body["s_name"].s();
        c.number_plate = body["number_plate"].s();
        c.entryTime = std::chrono::steady_clock::now();

        int slot = parkinglot.addCustomer(c);

        res["success"] = true;
        res["assigned_slot"] = slot;
        return crow::response(200, res);
    });

    //checkout
    CROW_ROUTE(app, "/api/checkout").methods(crow::HTTPMethod::POST)([&parkinglot, &rate](const crow::request& req){
        auto body = crow::json::load(req.body);

        if(!body || !body.has("slot_no")){
            crow::json::wvalue err;
            err["success"] = false;
            err["message"] = "invalide payload.";
            return crow::response(400, err);
        }

        int slot = body["slot_no"].i();
        crow::json::wvalue result = parkinglot.checkOut(slot, rate);
        return crow::response(200, result); 
    });

    //admin priviledge- get current rates
    CROW_ROUTE(app, "/api/admin/rates").methods(crow::HTTPMethod::GET)([&parkinglot, &rate](const crow::request& req){
        std::string authPass = req.get_header_value("x-admin-pass");
        if(!parkinglot.verifyAdmin(authPass)){
            return crow::response(401, "Unauthorised");
        }

        crow::json::wvalue res;
        res["halfHours"] = rate.halfHours;
        res["twoHours"] = rate.twoHours;
        res["fourHours"] = rate.fourHours;
        res["sixHours"] = rate.sixHours;
        res["overHours"] = rate.overHours;

        return crow::response(200, res);
    });

    //admin priviledge - update rates
    CROW_ROUTE(app, "/api/admin/rates").methods(crow::HTTPMethod::POST)
    ([&parkinglot, &rate](const crow::request& req) {
        std::string authPass = req.get_header_value("x-admin-pass");
        if (!parkinglot.verifyAdmin(authPass)) {
            return crow::response(401, "Unauthorized");
        }

        auto body = crow::json::load(req.body);
        crow::json::wvalue res;

        if (!body) {
            res["success"] = false;
            res["message"] = "Invalid format.";
            return crow::response(400, res);
        }

        if (body.has("halfHours")) rate.halfHours = body["halfHours"].d();
        if (body.has("twoHours")) rate.twoHours = body["twoHours"].d();
        if (body.has("fourHours")) rate.fourHours = body["fourHours"].d();
        if (body.has("sixHours")) rate.sixHours = body["sixHours"].d();
        if (body.has("overHours")) rate.overHours = body["overHours"].d();

        res["success"] = true;
        res["message"] = "Parking rates updated successfully!";
        return crow::response(200, res);
    });

    //admin priviledge - update number of slots
    CROW_ROUTE(app, "/api/admin/slots").methods(crow::HTTPMethod::POST)
    ([&parkinglot](const crow::request& req) {
        std::string authPass = req.get_header_value("x-admin-pass");
        if (!parkinglot.verifyAdmin(authPass)) {
            return crow::response(401, "Unauthorized");
        }

        auto body = crow::json::load(req.body);
        crow::json::wvalue res;

        if (!body || !body.has("total_slots")) {
            res["success"] = false;
            res["message"] = "Invalid input.";
            return crow::response(400, res);
        }

        int newTotal = body["total_slots"].i();
        if (parkinglot.setTotalSlots(newTotal)) {
            res["success"] = true;
            res["message"] = "Total parking slots updated successfully!";
            return crow::response(200, res);
        } else {
            res["success"] = false;
            res["message"] = "Total slots must be greater than 0.";
            return crow::response(400, res);
        }
    });

    

    std::cout << "Starting Crow Web Server on http://localhost:8080\n";
    app.port(8080).multithreaded().run();

}

/*
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
                    fyi if you uncomment whole main please add a comment here that star followed by slash 

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
*/