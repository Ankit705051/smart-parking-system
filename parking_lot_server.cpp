// ==================== PARKING LOT SYSTEM WITH C++ OOP ====================
#include<fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

using namespace std;

// ==================== ENUM FOR VEHICLE TYPES ====================
enum class VehicleType {
    CAR,
    BIKE,
    TRUCK,
    VAN
};

string vehicleTypeToString(VehicleType type) {
    switch(type) {
        case VehicleType::CAR: return "Car";
        case VehicleType::BIKE: return "Bike";
        case VehicleType::TRUCK: return "Truck";
        case VehicleType::VAN: return "Van";
        default: return "Unknown";
    }
}

VehicleType stringToVehicleType(const string& str) {
    if (str == "Car" || str == "car") return VehicleType::CAR;
    if (str == "Bike" || str == "bike") return VehicleType::BIKE;
    if (str == "Truck" || str == "truck") return VehicleType::TRUCK;
    if (str == "Van" || str == "van") return VehicleType::VAN;
    return VehicleType::CAR;
}

// ==================== VEHICLE CLASS (Base Class) ====================
class Vehicle {
protected:
    string licensePlate;
    VehicleType type;
    string color;
    time_t entryTime;

public:
    Vehicle(const string& plate, VehicleType t, const string& c)
        : licensePlate(plate), type(t), color(c) {
        entryTime = time(nullptr);
    }

    virtual ~Vehicle() {}

    string getLicensePlate() const { return licensePlate; }
    VehicleType getType() const { return type; }
    string getColor() const { return color; }
    time_t getEntryTime() const { return entryTime; }

    virtual double getParkingRate() const = 0; // Pure virtual function

    string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"licensePlate\":\"" << licensePlate << "\",";
        ss << "\"type\":\"" << vehicleTypeToString(type) << "\",";
        ss << "\"color\":\"" << color << "\",";
        ss << "\"entryTime\":" << entryTime << ",";
        ss << "\"rate\":" << getParkingRate();
        ss << "}";
        return ss.str();
    }
};

// ==================== DERIVED VEHICLE CLASSES ====================
class Car : public Vehicle {
public:
    Car(const string& plate, const string& color)
        : Vehicle(plate, VehicleType::CAR, color) {}

    double getParkingRate() const override { return 5.0; } // $5 per hour
};

class Bike : public Vehicle {
public:
    Bike(const string& plate, const string& color)
        : Vehicle(plate, VehicleType::BIKE, color) {}

    double getParkingRate() const override { return 2.0; } // $2 per hour
};

class Truck : public Vehicle {
public:
    Truck(const string& plate, const string& color)
        : Vehicle(plate, VehicleType::TRUCK, color) {}

    double getParkingRate() const override { return 10.0; } // $10 per hour
};

class Van : public Vehicle {
public:
    Van(const string& plate, const string& color)
        : Vehicle(plate, VehicleType::VAN, color) {}

    double getParkingRate() const override { return 7.0; } // $7 per hour
};

// ==================== PARKING SPOT CLASS ====================
class ParkingSpot {
private:
    int spotNumber;
    VehicleType allowedType;
    bool occupied;
    Vehicle* parkedVehicle;

public:
    ParkingSpot(int num, VehicleType type)
        : spotNumber(num), allowedType(type), occupied(false), parkedVehicle(nullptr) {}

    ~ParkingSpot() {
        if (parkedVehicle) {
            delete parkedVehicle;
        }
    }

    int getSpotNumber() const { return spotNumber; }
    VehicleType getAllowedType() const { return allowedType; }
    bool isOccupied() const { return occupied; }
    Vehicle* getParkedVehicle() const { return parkedVehicle; }

    bool parkVehicle(Vehicle* vehicle) {
        if (occupied || vehicle->getType() != allowedType) {
            return false;
        }
        parkedVehicle = vehicle;
        occupied = true;
        return true;
    }

    Vehicle* removeVehicle() {
        if (!occupied) return nullptr;
        Vehicle* vehicle = parkedVehicle;
        parkedVehicle = nullptr;
        occupied = false;
        return vehicle;
    }

    string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"spotNumber\":" << spotNumber << ",";
        ss << "\"type\":\"" << vehicleTypeToString(allowedType) << "\",";
        ss << "\"occupied\":" << (occupied ? "true" : "false");
        if (occupied && parkedVehicle) {
            ss << ",\"vehicle\":" << parkedVehicle->toJSON();
        }
        ss << "}";
        return ss.str();
    }
};

// ==================== PARKING TICKET CLASS ====================
class ParkingTicket {
private:
    int ticketId;
    string licensePlate;
    time_t entryTime;
    time_t exitTime;
    double amount;
    bool paid;

public:
    ParkingTicket(int id, const string& plate, time_t entry)
        : ticketId(id), licensePlate(plate), entryTime(entry),
          exitTime(0), amount(0.0), paid(false) {}

    int getTicketId() const { return ticketId; }
    string getLicensePlate() const { return licensePlate; }
    
    void calculateAmount(double rate) {
        exitTime = time(nullptr);
        double hours = difftime(exitTime, entryTime) / 3600.0;
        if (hours < 1.0) hours = 1.0; // Minimum 1 hour
        amount = hours * rate;
    }

    double getAmount() const { return amount; }
    void markPaid() { paid = true; }
    bool isPaid() const { return paid; }

    string toJSON() const {
        stringstream ss;
        ss << "{";
        ss << "\"ticketId\":" << ticketId << ",";
        ss << "\"licensePlate\":\"" << licensePlate << "\",";
        ss << "\"entryTime\":" << entryTime << ",";
        ss << "\"exitTime\":" << exitTime << ",";
        ss << "\"amount\":" << fixed << setprecision(2) << amount << ",";
        ss << "\"paid\":" << (paid ? "true" : "false");
        ss << "}";
        return ss.str();
    }
};

// ==================== PARKING LOT CLASS (Main System) ====================
class ParkingLot {
private:
    string name;
    vector<ParkingSpot*> spots;
    map<string, int> vehicleToSpot; // licensePlate -> spotNumber
    vector<ParkingTicket*> tickets;
    int nextTicketId;
    double totalRevenue;

public:
    ParkingLot(const string& lotName) 
        : name(lotName), nextTicketId(1), totalRevenue(0.0) {
        initializeSpots();
    }

    ~ParkingLot() {
        for (auto spot : spots) delete spot;
        for (auto ticket : tickets) delete ticket;
    }

    void initializeSpots() {
        // Create 20 car spots
        for (int i = 1; i <= 20; i++) {
            spots.push_back(new ParkingSpot(i, VehicleType::CAR));
        }
        // Create 10 bike spots
        for (int i = 21; i <= 30; i++) {
            spots.push_back(new ParkingSpot(i, VehicleType::BIKE));
        }
        // Create 5 truck spots
        for (int i = 31; i <= 35; i++) {
            spots.push_back(new ParkingSpot(i, VehicleType::TRUCK));
        }
        // Create 5 van spots
        for (int i = 36; i <= 40; i++) {
            spots.push_back(new ParkingSpot(i, VehicleType::VAN));
        }
    }

    int parkVehicle(const string& plate, VehicleType type, const string& color) {
        // Check if vehicle already parked
        if (vehicleToSpot.find(plate) != vehicleToSpot.end()) {
            return -1; // Already parked
        }

        // Find available spot
        for (auto spot : spots) {
            if (!spot->isOccupied() && spot->getAllowedType() == type) {
                Vehicle* vehicle = createVehicle(plate, type, color);
                if (spot->parkVehicle(vehicle)) {
                    vehicleToSpot[plate] = spot->getSpotNumber();
                    
                    // Create ticket
                    ParkingTicket* ticket = new ParkingTicket(nextTicketId++, plate, vehicle->getEntryTime());
                    tickets.push_back(ticket);
                    
                    return spot->getSpotNumber();
                }
            }
        }
        return -2; // No available spot
    }

    string exitVehicle(const string& plate) {
        if (vehicleToSpot.find(plate) == vehicleToSpot.end()) {
            return "{\"success\":false,\"message\":\"Vehicle not found\"}";
        }

        int spotNum = vehicleToSpot[plate];
        ParkingSpot* spot = nullptr;
        
        for (auto s : spots) {
            if (s->getSpotNumber() == spotNum) {
                spot = s;
                break;
            }
        }

        if (!spot) {
            return "{\"success\":false,\"message\":\"Spot not found\"}";
        }

        Vehicle* vehicle = spot->removeVehicle();
        if (!vehicle) {
            return "{\"success\":false,\"message\":\"Failed to remove vehicle\"}";
        }

        // Find and update ticket
        ParkingTicket* ticket = nullptr;
        for (auto t : tickets) {
            if (t->getLicensePlate() == plate && !t->isPaid()) {
                ticket = t;
                break;
            }
        }

        if (ticket) {
            ticket->calculateAmount(vehicle->getParkingRate());
            double amount = ticket->getAmount();
            totalRevenue += amount;
            ticket->markPaid();
            
            vehicleToSpot.erase(plate);
            delete vehicle;

            stringstream ss;
            ss << "{\"success\":true,\"ticket\":" << ticket->toJSON() << "}";
            return ss.str();
        }

        delete vehicle;
        vehicleToSpot.erase(plate);
        return "{\"success\":false,\"message\":\"Ticket not found\"}";
    }

    string getStatus() const {
        int totalSpots = spots.size();
        int occupiedSpots = 0;
        int carSpots = 0, bikeSpots = 0, truckSpots = 0, vanSpots = 0;
        int carOccupied = 0, bikeOccupied = 0, truckOccupied = 0, vanOccupied = 0;

        for (const auto& spot : spots) {
            if (spot->isOccupied()) occupiedSpots++;
            
            switch(spot->getAllowedType()) {
                case VehicleType::CAR:
                    carSpots++;
                    if (spot->isOccupied()) carOccupied++;
                    break;
                case VehicleType::BIKE:
                    bikeSpots++;
                    if (spot->isOccupied()) bikeOccupied++;
                    break;
                case VehicleType::TRUCK:
                    truckSpots++;
                    if (spot->isOccupied()) truckOccupied++;
                    break;
                case VehicleType::VAN:
                    vanSpots++;
                    if (spot->isOccupied()) vanOccupied++;
                    break;
            }
        }

        stringstream ss;
        ss << "{";
        ss << "\"name\":\"" << name << "\",";
        ss << "\"totalSpots\":" << totalSpots << ",";
        ss << "\"occupiedSpots\":" << occupiedSpots << ",";
        ss << "\"availableSpots\":" << (totalSpots - occupiedSpots) << ",";
        ss << "\"totalRevenue\":" << fixed << setprecision(2) << totalRevenue << ",";
        ss << "\"carSpots\":{\"total\":" << carSpots << ",\"occupied\":" << carOccupied << "},";
        ss << "\"bikeSpots\":{\"total\":" << bikeSpots << ",\"occupied\":" << bikeOccupied << "},";
        ss << "\"truckSpots\":{\"total\":" << truckSpots << ",\"occupied\":" << truckOccupied << "},";
        ss << "\"vanSpots\":{\"total\":" << vanSpots << ",\"occupied\":" << vanOccupied << "}";
        ss << "}";
        return ss.str();
    }

    string getAllSpots() const {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < spots.size(); i++) {
            if (i > 0) ss << ",";
            ss << spots[i]->toJSON();
        }
        ss << "]";
        return ss.str();
    }

    string getAllTickets() const {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < tickets.size(); i++) {
            if (i > 0) ss << ",";
            ss << tickets[i]->toJSON();
        }
        ss << "]";
        return ss.str();
    }

private:
    Vehicle* createVehicle(const string& plate, VehicleType type, const string& color) {
        switch(type) {
            case VehicleType::CAR: return new Car(plate, color);
            case VehicleType::BIKE: return new Bike(plate, color);
            case VehicleType::TRUCK: return new Truck(plate, color);
            case VehicleType::VAN: return new Van(plate, color);
            default: return new Car(plate, color);
        }
    }
};

// ==================== WEB SERVER CLASS ====================
class WebServer {
private:
    ParkingLot& parkingLot;
    SOCKET serverSocket;
    int port;

public:
    WebServer(ParkingLot& lot, int p = 8080) : parkingLot(lot), port(p) {
        #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        #endif
    }

    ~WebServer() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
        #ifdef _WIN32
        WSACleanup();
        #endif
    }

    string urlDecode(const string& str) {
        string result;
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] == '+') {
                result += ' ';
            } else if (str[i] == '%' && i + 2 < str.length()) {
                int value;
                sscanf(str.substr(i + 1, 2).c_str(), "%x", &value);
                result += static_cast<char>(value);
                i += 2;
            } else {
                result += str[i];
            }
        }
        return result;
    }

    map<string, string> parseQuery(const string& query) {
        map<string, string> params;
        stringstream ss(query);
        string pair;
        
        while (getline(ss, pair, '&')) {
            size_t pos = pair.find('=');
            if (pos != string::npos) {
                string key = pair.substr(0, pos);
                string value = urlDecode(pair.substr(pos + 1));
                params[key] = value;
            }
        }
        return params;
    }

    string handleRequest(const string& request) {
        stringstream ss(request);
        string method, path, version;
        ss >> method >> path >> version;

        if (path.find("/park?") == 0) {
            string query = path.substr(6);
            auto params = parseQuery(query);
            
            string plate = params["plate"];
            string type = params["type"];
            string color = params["color"];
            
            int spotNum = parkingLot.parkVehicle(plate, stringToVehicleType(type), color);
            
            stringstream json;
            if (spotNum > 0) {
                json << "{\"success\":true,\"spotNumber\":" << spotNum << ",\"message\":\"Vehicle parked successfully\"}";
            } else if (spotNum == -1) {
                json << "{\"success\":false,\"message\":\"Vehicle already parked\"}";
            } else {
                json << "{\"success\":false,\"message\":\"No available spots\"}";
            }
            
            return createHTTPResponse("200 OK", "application/json", json.str());
            
        } else if (path.find("/exit?") == 0) {
            string query = path.substr(6);
            auto params = parseQuery(query);
            string plate = params["plate"];
            
            string result = parkingLot.exitVehicle(plate);
            return createHTTPResponse("200 OK", "application/json", result);
            
        } else if (path == "/status") {
            return createHTTPResponse("200 OK", "application/json", parkingLot.getStatus());
            
        } else if (path == "/spots") {
            return createHTTPResponse("200 OK", "application/json", parkingLot.getAllSpots());
            
        } else if (path == "/tickets") {
            return createHTTPResponse("200 OK", "application/json", parkingLot.getAllTickets());
        }
        
        return createHTTPResponse("404 Not Found", "text/plain", "Not Found");
    }

    string createHTTPResponse(const string& status, const string& contentType, const string& body) {
        stringstream response;
        response << "HTTP/1.1 " << status << "\r\n";
        response << "Content-Type: " << contentType << "\r\n";
        response << "Content-Length: " << body.length() << "\r\n";
        response << "Access-Control-Allow-Origin: *\r\n";
        response << "Connection: close\r\n\r\n";
        response << body;
        return response.str();
    }

    void start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            cerr << "Failed to create socket" << endl;
            return;
        }

        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "Bind failed" << endl;
            return;
        }

        if (listen(serverSocket, 5) == SOCKET_ERROR) {
            cerr << "Listen failed" << endl;
            return;
        }

        cout << "\n=== PARKING LOT MANAGEMENT SYSTEM ===" << endl;
        cout << "Server running on http://localhost:" << port << endl;
        cout << "Press Ctrl+C to stop" << endl;
        cout << "====================================\n" << endl;

        while (true) {
            sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);

            if (clientSocket != INVALID_SOCKET) {
                char buffer[4096] = {0};
                recv(clientSocket, buffer, sizeof(buffer), 0);

                string request(buffer);
                string response = handleRequest(request);

                send(clientSocket, response.c_str(), response.length(), 0);
                closesocket(clientSocket);
            }
        }
    }
};

// ==================== MAIN FUNCTION ====================
int main() {
    ParkingLot lot("Downtown Parking");
    
    cout << "Initializing parking lot..." << endl;
    cout << "Total spots: 40 (20 Cars, 10 Bikes, 5 Trucks, 5 Vans)" << endl;

    WebServer server(lot, 8080);
    server.start();

    return 0;
}

/*
==================== COMPILATION INSTRUCTIONS ====================

Linux/Mac:
    g++ -std=c++11 parking_lot_server.cpp -o parking_server
    ./parking_server

Windows (MinGW):
    g++ -std=c++11 parking_lot_server.cpp -o parking_server.exe -lws2_32
    parking_server.exe

Windows (Visual Studio):
    cl /EHsc parking_lot_server.cpp ws2_32.lib
    parking_server.exe

==================== API ENDPOINTS ====================

1. Park Vehicle:
   GET /park?plate=ABC123&type=Car&color=Red

2. Exit Vehicle:
   GET /exit?plate=ABC123

3. Get Status:
   GET /status

4. Get All Spots:
   GET /spots

5. Get All Tickets:
   GET /tickets

==================================================================
*/