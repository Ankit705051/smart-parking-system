# Parking Lot Management System

A modern web-based parking lot management system that provides real-time monitoring and management of parking spaces.

## Features

- **Real-time Status Monitoring**: View total spots, occupied spots, available spots, and total revenue
- **Visual Parking Spots Display**: Interactive grid showing occupied and available parking spots
- **Vehicle Management**: Park and exit vehicles with license plate tracking
- **Recent Transactions**: View the latest parking tickets and transactions
- **Auto-refresh**: Data automatically refreshes every 3 seconds
- **Connection Status**: Real-time backend connection status indicator

## Technology Stack

- **Frontend**: HTML5, CSS3, JavaScript (ES6+)
- **Backend API**: RESTful API running on localhost:8080
- **Architecture**: Client-server architecture with async/await patterns

## Getting Started

### Prerequisites

- A modern web browser (Chrome, Firefox, Safari, Edge)
- Backend server running on localhost:8080

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/lot-management-system.git
   cd lot-management-system
   ```

2. Open `index.html` in your web browser

3. Ensure the backend API server is running on localhost:8080

### Backend API Endpoints

The frontend expects the following API endpoints:

- `GET /status` - Returns parking lot status
- `GET /spots` - Returns all parking spots
- `GET /tickets` - Returns parking tickets
- `POST /park` - Parks a vehicle
- `POST /exit` - Exits a vehicle

## Usage

1. **View Status**: The dashboard shows real-time parking lot statistics
2. **Park a Vehicle**: Enter license plate, vehicle type, and color to park
3. **Exit a Vehicle**: Enter license plate to exit and calculate payment
4. **Monitor Spots**: Visual grid shows occupied (red) and available (green) spots

## File Structure

```
lot-management-system/
├── index.html          # Main HTML file
├── script.js           # JavaScript functionality
├── style.css           # Styling (to be implemented)
├── README.md           # This file
└── .gitignore          # Git ignore file
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Future Enhancements

- [ ] Add responsive design for mobile devices
- [ ] Implement user authentication
- [ ] Add payment gateway integration
- [ ] Create admin dashboard
- [ ] Add reporting and analytics
- [ ] Implement vehicle type-based pricing
- [ ] Add multi-language support

## Contact

Your Name - [@yourusername](https://github.com/yourusername)

Project Link: [https://github.com/yourusername/lot-management-system](https://github.com/yourusername/lot-management-system)
