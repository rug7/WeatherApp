Weather App
A modern, feature-rich weather application built with C++ and ImGui, providing real-time weather information for cities worldwide with a sleek and intuitive user interface.
---------------------------------------------------------------
Features
.🌍 Real-time weather data for cities worldwide
.🔍 Smart search with autocomplete functionality
.⭐ Favorite cities system
.🎨 Modern and intuitive user interface
.🔐 User authentication system
.📱 Responsive design
.🌡️ Detailed weather information including:
        .Temperature
        .Humidity
        .Pressure
        .Wind Speed

---------------------------------------------------------------
Technical Stack
.C++17
.OpenGL for rendering
.ImGui for user interface
.GLFW for window management
.GLEW for OpenGL extension loading
.nlohmann/json for JSON parsing
.cpp-httplib for HTTP requests
---------------------------------------------------------------
Project Structure
.WeatherApp.h/cpp: Main application logic and UI rendering
.CityWeather.h/cpp: Weather data management for individual cities
.main.cpp: Application entry point
---------------------------------------------------------------
Dependencies
.OpenGL
.GLFW3
.GLEW
.ImGui
.cpp-httplib
.nlohmann/json
---------------------------------------------------------------
Building the Project
1-Clone the repository:

git clone https://github.com/yourusername/weather-app.git
cd weather-app
---------------------------------------------------------------
2-Install dependencies:

# For Ubuntu/Debian
sudo apt-get install libglfw3-dev libglew-dev

# For macOS
brew install glfw glew

# For Windows
vcpkg install glfw3:x64-windows glew:x64-windows
---------------------------------------------------------------
3-Build the project:

mkdir build
cd build
cmake ..
make
---------------------------------------------------------------
Usage
1-Launch the application
2-Log in with your credentials (default: username: "1", password: "1")
3-Use the search bar to find cities
4-Click the star icon to add cities to favorites
5-Click on the info icon for detailed weather information
---------------------------------------------------------------
Features in Detail
.Search Functionality
.Real-time city search with autocomplete
.Supports partial matching
.Displays city and country information
---------------------------------------------------------------
Weather Cards
.Displays up to 7 cities simultaneously
.Shows current temperature
.Quick access to detailed information
.Easy removal option
---------------------------------------------------------------
Favorite System
.Save frequently checked cities
.Quick access to favorite locations
.Persistent storage between sessions
---------------------------------------------------------------
Contributing
1-Fork the repository
2-Create your feature branch (git checkout -b feature/AmazingFeature)
3-Commit your changes (git commit -m 'Add some AmazingFeature')
4-Push to the branch (git push origin feature/AmazingFeature)
5-Open a Pull Request
---------------------------------------------------------------
Acknowledgments
.OpenWeather API for weather data
.ImGui for the UI framework
.All contributors and testers
---------------------------------------------------------------
Contact
Majd Abdo - Khader Yaghmour