            # PARKING MANAGEMENT SYSTEM
A web based parking management system

# Languages
- C++ - main language as backend
- Crow - bridges C++ and frontend
- Java
- Css
- Html

# Modules
- Register Vehicle
- Assign Vehicle
- Track all parked vehicle
- Web based interface
- change number of total slot
- Change prices of rates

# Running
  ## Linux
  - clone the repository:
        ```bash 
            git clone  https://github.com/GikeraPenuel/parking_project.git
        ```
  - go to the root of the project:  
        ```bash 
            cd ~/"your_folder"/parking_project 
        ```
  - Run the program: 
                    ``` bash 
                            ./parking 
                    ```
  - go to your browser and type ` http://localhost:8080 ` and expect a result
  - default admin password is ` admin123 `

  ## Windows
  - clone the repository:
        ```powershell 
            git clone  https://github.com/GikeraPenuel/parking_project.git
        ```
  - go to project directory:
        ```powershell 
            cd .\parking_project 
        ```
  - Run the program: 
        ```powershell 
            .\parking.exe 
        ```
  - go to your browser and type ` http://localhost:8080 ` and expect a result
  - default admin password is ` admin123 `

  - NOTE! Please do not run from folder in file explorer as executable as it may cause some issues with dependencies and file paths... i learnt the hard way


# ALGORITHM 
================================================================================
                    SMART PARKING MANAGEMENT SYSTEM
                      CORE SYSTEM ALGORITHMS & SPECIFICATION
================================================================================

SYSTEM OVERVIEW
--------------------------------------------------------------------------------
The Smart Parking Management System operates as an event-driven RESTful application
with client-side polling capabilities. It governs dynamic slot allocation, admin
authentication, time-delta billing calculation, and role-based data sanitization.


## 1. VEHICLE CHECK-IN ALGORITHM
--------------------------------------------------------------------------------
Trigger Endpoint: POST /api/checkin
Payload: { "f_name": string, "s_name": string, "number_plate": string }

Steps:
1. Receive request payload containing customer details.
2. Query capacity status:
   - Call getEmptySlots().
   - IF getEmptySlots() == 0:
       - Return HTTP 400 Bad Request ("NO AVAILABLE SLOTS ... RETURN LATER").
3. Linear search allocation:
   - Iterate i from 0 to (total_slots - 1):
       - IF slot_status[i] == true (slot is free):
           a. Set slot_status[i] = false (mark slot as occupied).
           b. Instantiate Customer object with f_name, s_name, and number_plate.
           c. Capture entry timestamp:
              T_entry = std::chrono::steady_clock::now()
           d. Store Customer at array/vector index i.
           e. Return HTTP 200 OK with { "success": true, "assigned_slot": i }.
4. IF loop completes without finding an open slot, return HTTP 400 Bad Request.


## 2. VEHICLE CHECK-OUT & FEE CALCULATION ALGORITHM
--------------------------------------------------------------------------------
Trigger Endpoint: POST /api/checkout
Payload: { "slot_no": integer }

Steps:
1. Receive request payload containing targeted slot_no.
2. Parameter validation:
   - IF slot_no < 0 OR slot_no >= total_slots OR slot_status[slot_no] == true:
       - Return HTTP 400 Bad Request ("invalid slot number!" or "Slot is already empty!").
3. Time delta computation:
   - Capture checkout timestamp:
     T_exit = std::chrono::steady_clock::now()
   - Calculate elapsed duration in minutes:
     Delta_t = duration_cast<minutes>(T_exit - T_entry)
4. Billing rate mapping:
   - Evaluate Delta_t against rate thresholds (using current ParkingRate struct):
       - IF Delta_t <= 30 minutes      => Fee = ParkingRate.halfHours
       - ELSE IF Delta_t <= 120 minutes => Fee = ParkingRate.twoHours
       - ELSE IF Delta_t <= 240 minutes => Fee = ParkingRate.fourHours
       - ELSE IF Delta_t <= 360 minutes => Fee = ParkingRate.sixHours
       - ELSE (Delta_t > 360 minutes)   => Fee = ParkingRate.overHours
5. Reset slot state:
   - Set slot_status[slot_no] = true (mark slot as available).
   - Clear Customer record at index slot_no.
6. Return HTTP 200 OK response with payload:
   {
     "success": true,
     "slot": slot_no,
     "plate": number_plate,
     "time_parked": Delta_t,
     "fees": Fee
   }


## 3. STATUS QUERY & ROLE-BASED DATA SANITIZATION ALGORITHM
--------------------------------------------------------------------------------
Trigger Endpoint: GET /api/status
Headers: { "x-admin-pass": string (optional) }

Steps:
1. Extract "x-admin-pass" header from incoming request.
2. Authenticate session:
   - Execute verifyAdmin(header_pass).
   - Set boolean flag: isAdmin = (header_pass == admin_password).
3. Build slot state collection:
   - Initialize JSON array list.
   - For i from 0 to (total_slots - 1):
       a. Create slot JSON item: { "slot_no": i, "is_empty": slot_status[i] }.
       b. SANITIZATION CHECK:
          - IF isAdmin == true AND slot_status[i] == false:
              * Append "occupant": f_name + " " + s_name
              * Append "plate": number_plate
          - ELSE (Non-admin request or slot is empty):
              * Omit PII fields (occupant and plate) from JSON object.
       c. Add item to array list.
4. Construct final response object:
   {
     "total_slots": getTotalSlot(),
     "empty_slots": getEmptySlots(),
     "slots": [ ... ]
   }
5. Return HTTP 200 OK with constructed status object.


## 4. FRONTEND DYNAMIC POLLING & STATE SYNCHRONIZATION ALGORITHM
--------------------------------------------------------------------------------
Client-Side Module: script.js

Steps:
1. Application Initialization:
   - Immediately execute fetchStatus() on initial DOM load.
   - Register recurring interval timer:
     setInterval(fetchStatus, 3000) -> Polls backend every 3 seconds.
2. Admin Session Management:
   - Login: Post password to /api/admin/login. On success, cache password in
     adminToken variable and reveal administrative UI panels.
   - Logout: Reset adminToken to empty string and collapse admin UI panels.
3. DOM Rendering (fetchStatus execution):
   - Send HTTP GET request to /api/status including "x-admin-pass": adminToken.
   - Update summary text: "AVAILABLE SLOTS: empty_slots / total_slots".
   - IF adminToken IS SET:
       a. Update slot capacity input field (if not currently focused).
       b. Clear existing visual container DOM nodes.
       c. Iterate over slots array:
          - Generate slot card element with class "empty" or "occupied".
          - Display slot number, plate number, and occupant details (if present).
          - Append slot card to dashboard container.

