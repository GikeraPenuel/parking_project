let adminToken = "";

async function adminLogin() {
    const pass = document.getElementById('adminPassInput').value;
    const res = await fetch('/api/admin/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ password: pass })
    });

    const data = await res.json();
    if (data.success) {
        adminToken = pass;
        document.getElementById('loginArea').style.display = 'none';
        document.getElementById('logoutArea').style.display = 'flex';
        document.getElementById('adminPanel').style.display = 'block';
        document.getElementById('slotsContainer').style.display = 'flex';
        fetchRates();
        fetchStatus();
        alert("Authenticated as Administrator");
    } else {
        alert("ERROR: Invalid admin password.");
    }
}

function adminLogout() {
    adminToken = "";
    document.getElementById('loginArea').style.display = 'flex';
    document.getElementById('logoutArea').style.display = 'none';
    document.getElementById('adminPanel').style.display = 'none';
    document.getElementById('slotsContainer').style.display = 'none';
    document.getElementById('adminPassInput').value = '';
    fetchStatus();
}

async function fetchStatus() {
    const res = await fetch('/api/status', {
        headers: { 'x-admin-pass': adminToken }
    });
    const data = await res.json();

    document.getElementById('slotSummary').innerText = 
        `AVAILABLE SLOTS: ${data.empty_slots} / ${data.total_slots}`;

    if (adminToken) {
        if (document.activeElement !== document.getElementById('totalSlotsInput')) {
            document.getElementById('totalSlotsInput').value = data.total_slots;
        }

        const container = document.getElementById('slotsContainer');
        container.innerHTML = '';

        data.slots.forEach(slot => {
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

async function checkOut() {
    const slot_no = parseInt(document.getElementById('slotno').value);

    const res = await fetch('/api/checkout', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ slot_no })
    });

    const data = await res.json();
    if (data.success) {
        alert(`Time Parked: ${data.time_parked} min\nTHUS\nYou are to pay KSH ${data.fee}\n\nCAR: ${data.plate} has checked out.\nSlot #${data.slot} is now empty.`);
        document.getElementById('slotno').value = '';
        fetchStatus();
    } else {
        alert(`ERROR: ${data.message}`);
    }
}

fetchStatus();
setInterval(fetchStatus, 3000);