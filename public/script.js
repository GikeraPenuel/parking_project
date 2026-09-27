let adminToken = "";  // Stores admin pass for authentication header across endpoints

async function adminLogin() {
    const pass = document.getElementById('adminPassInput').value;  //Authenticates admin by posting password to backend.
    const res = await fetch('/api/admin/login', {                 //Corresponding Backend Route: POST /api/admin/login in main.cpp
        method: 'POST',                                            //Calls method: Parking::verifyAdmin() in parking.hpp
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ password: pass })
    });

    const data = await res.json();
    if (data.success) {    // Save administrative credential locally to pass in header 'x-admin-pass'
        adminToken = pass;

        // Toggle view elements for admin dashboard view
        document.getElementById('loginArea').style.display = 'none';
        document.getElementById('logoutArea').style.display = 'flex';
        document.getElementById('adminPanel').style.display = 'block';
        document.getElementById('slotsContainer').style.display = 'flex';

        // Load initial values for rate fields and slot statuses
        fetchRates();
        fetchStatus();
        alert("Authenticated as Administrator");
    } else {
        alert("ERROR: Invalid admin password.");
    }
}
/*
 Clears local admin session and hides privileged dashboard components.
 */
function adminLogout() {
    adminToken = "";
    document.getElementById('loginArea').style.display = 'flex';
    document.getElementById('logoutArea').style.display = 'none';
    document.getElementById('adminPanel').style.display = 'none';
    document.getElementById('slotsContainer').style.display = 'none';
    document.getElementById('adminPassInput').value = '';
    fetchStatus();// Refresh slot statuses as non-admin (clears occupant PII from public view)
}

async function fetchStatus() {
    const res = await fetch('/api/status', {  // Passes 'x-admin-pass' header; backend uses it to decide whether to attach occupant/plate PII
        headers: { 'x-admin-pass': adminToken }
    });
    const data = await res.json();

    document.getElementById('slotSummary').innerText =   // Render count derived from Parking::getEmptySlots() and Parking::getTotalSlot()
        `AVAILABLE SLOTS: ${data.empty_slots} / ${data.total_slots}`;

    if (adminToken) {
        if (document.activeElement !== document.getElementById('totalSlotsInput')) {  // Prevent overwriting active user typing when polling updates total capacity input
            document.getElementById('totalSlotsInput').value = data.total_slots;
        }

        const container = document.getElementById('slotsContainer');
        container.innerHTML = '';

        data.slots.forEach(slot => {                                // Dynamically build and render visual slot cards array from JSON
            const card = document.createElement('div');
            card.className = `slot-card ${slot.is_empty ? 'empty' : 'occupied'}`;
            card.innerHTML = `
                <h3>Slot #${slot.slot_no}</h3>
                <p><strong>${slot.is_empty ? 'EMPTY' : slot.plate}</strong></p>
                <small>${slot.is_empty ? '' : slot.occupant}</small>
            `;
            container.appendChild(card);
        });
    }
}
/*
 Retrieves dynamic fee schedule values set on server.
 Corresponding Backend Route: GET /api/admin/rates in main.cpp
 Reads from struct: ParkingRate in parking.hpp
 */

async function fetchRates() {
    if (!adminToken) return;
    const res = await fetch('/api/admin/rates', {
        headers: { 'x-admin-pass': adminToken }
    });
    if (res.ok) {
        const data = await res.json();
        document.getElementById('rateHalf').value = data.halfHours;
        document.getElementById('rateTwo').value = data.twoHours;
        document.getElementById('rateFour').value = data.fourHours;
        document.getElementById('rateSix').value = data.sixHours;
        document.getElementById('rateOver').value = data.overHours;
    }
}

/*
Updates total parking slots capacity.
Corresponding Backend Route: POST /api/admin/slots in main.cpp
Calls method: Parking::setTotalSlots() in parking.hpp
 */

async function updateTotalSlots() {
    const total_slots = parseInt(document.getElementById('totalSlotsInput').value);

    const res = await fetch('/api/admin/slots', {
        method: 'POST',
        headers: { 
            'Content-Type': 'application/json',
            'x-admin-pass': adminToken
        },
        body: JSON.stringify({ total_slots })
    });

    const data = await res.json();
    if (data.success) {
        alert("SUCCESS: Total slots updated!");
        fetchStatus();
    } else {
        alert(`ERROR: ${data.message}`);
    }
}
/*
Sends updated rate values to backend server.
Corresponding Backend Route: POST /api/admin/rates in main.cpp
Updates instance of: struct ParkingRate in main.cpp
 */
async function updateRates() {
    const payload = {
        halfHours: parseFloat(document.getElementById('rateHalf').value),
        twoHours: parseFloat(document.getElementById('rateTwo').value),
        fourHours: parseFloat(document.getElementById('rateFour').value),
        sixHours: parseFloat(document.getElementById('rateSix').value),
        overHours: parseFloat(document.getElementById('rateOver').value)
    };

    const res = await fetch('/api/admin/rates', {
        method: 'POST',
        headers: { 
            'Content-Type': 'application/json',
            'x-admin-pass': adminToken
        },
        body: JSON.stringify(payload)
    });

    const data = await res.json();
    if (data.success) {
        alert("CHANGE SUCCESSFUL: Rates updated!");
        fetchRates();
    } else {
        alert("ERROR: Unable to update rates.");
    }
}
/*
Registers new incoming vehicle and customer entry.
Corresponding Backend Route: POST /api/checkin in main.cpp
Populates struct Customer & calls Parking::addCustomer() in parking.hpp
 */
async function checkIn() {
    const f_name = document.getElementById('fname').value;
    const s_name = document.getElementById('sname').value;
    const number_plate = document.getElementById('plate').value;

    const res = await fetch('/api/checkin', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ f_name, s_name, number_plate })
    });

    const data = await res.json();
    if (data.success) {
        alert(`SUCCESS: You have been assigned SLOT #${data.assigned_slot}`);
        document.getElementById('fname').value = '';
        document.getElementById('sname').value = '';
        document.getElementById('plate').value = '';
        fetchStatus();
    } else {
        alert(`ERROR: ${data.message}`);
    }
}
/*
Processes vehicle exit, computes elapsed time and billing total.
Corresponding Backend Route: POST /api/checkout in main.cpp
Calls method: Parking::checkOut() in parking.hpp
 */

async function checkOut() {
    const slot_no = parseInt(document.getElementById('slotno').value);

    const res = await fetch('/api/checkout', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ slot_no })
    });

    const data = await res.json();
    if (data.success) {  // Display summary modal (data.fees matches key returned in parking.hpp checkOut)
        alert(`Time Parked: ${data.time_parked} min\nTHUS\nYou are to pay KSH ${data.fee}\n\nCAR: ${data.plate} has checked out.\nSlot #${data.slot} is now empty.`);
        document.getElementById('slotno').value = '';
        fetchStatus();
    } else {
        alert(`ERROR: ${data.message}`);
    }
}

fetchStatus();  // Initial fetch on page load
setInterval(fetchStatus, 3000);  // Polling: Auto-refresh slot statuses every 3 seconds to keep UI synchronized