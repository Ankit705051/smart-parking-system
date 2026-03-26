const API_URL = "http://localhost:8080";

async function checkConnection() {
    try {
        const res = await fetch(`${API_URL}/status`);
        if (res.ok) {
            document.getElementById("statusBadge").textContent = "Connected";
            loadData();
        }
    } catch {
        document.getElementById("statusBadge").textContent = "Disconnected";
    }
}

async function loadData() {
    loadStatus();
    loadSpots();
    loadTickets();
}

async function loadStatus() {
    const res = await fetch(`${API_URL}/status`);
    const data = await res.json();

    document.getElementById("totalSpots").textContent = data.totalSpots;
    document.getElementById("occupiedSpots").textContent = data.occupiedSpots;
    document.getElementById("availableSpots").textContent = data.availableSpots;
    document.getElementById("totalRevenue").textContent = data.totalRevenue;
}

async function loadSpots() {
    const res = await fetch(`${API_URL}/spots`);
    const spots = await res.json();

    const container = document.getElementById("spotsContainer");
    container.innerHTML = "";

    spots.forEach(s => {
        const div = document.createElement("div");
        div.className = `spot ${s.occupied ? "occupied" : "available"}`;
        div.textContent = s.spotNumber;
        container.appendChild(div);
    });
}

async function loadTickets() {
    const res = await fetch(`${API_URL}/tickets`);
    const tickets = await res.json();

    const container = document.getElementById("ticketsContainer");
    container.innerHTML = "";

    tickets.slice(-5).forEach(t => {
        const div = document.createElement("div");
        div.textContent = `#${t.ticketId} - ${t.licensePlate} - $${t.amount}`;
        container.appendChild(div);
    });
}

async function parkVehicle() {
    const plate = document.getElementById("parkPlate").value;
    const type = document.getElementById("parkType").value;
    const color = document.getElementById("parkColor").value;

    const res = await fetch(`${API_URL}/park?plate=${plate}&type=${type}&color=${color}`);
    const data = await res.json();

    alert(data.message);
    loadData();
}

async function exitVehicle() {
    const plate = document.getElementById("exitPlate").value;

    const res = await fetch(`${API_URL}/exit?plate=${plate}`);
    const data = await res.json();

    alert(data.message);
    loadData();
}

window.onload = () => {
    checkConnection();
    setInterval(loadData, 3000);
};