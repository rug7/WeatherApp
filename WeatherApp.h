#ifndef WEATHER_APP_H
#define WEATHER_APP_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "CityWeather.h"
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include "httplib.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include "httplib.h"
#include "json.hpp"
class WeatherApp {
public:
    WeatherApp();
    ~WeatherApp();

    void Run();
    void FetchAndDisplayWeather();
    GLuint loginBackgroundTexture;  // For login screen background
    std::vector<std::string> citySuggestions;
    bool showSuggestions = false;


    // Google Places API key (if using Google Places)
    const std::string PLACES_API_KEY = "AIzaSyBx3dMjzZz8TXdcxsifLJojNHghdNtYWDA";

private:
    void Initialize();
    void MainLoop();
    void Cleanup();
    void RenderGUI();
    void RenderCityDetails();
    void RenderLoginForm();  // New method for login form
    void FetchPlacesAutocomplete(const std::string& input);
    std::string GetCurrentTime1();
    std::string GetCurrentDate();
    const size_t MAX_CITIES = 7;
    bool showNotification = false;
    std::string notificationMessage;
    float notificationTimer = 0.0f;
    void FetchCitySuggestions(const std::string& input);
    CityWeather tempCity{""}; // Initialize with empty string
    // Add these members
    std::vector<std::string> autocompleteSuggestions;
    bool showAutocompleteSuggestions;
    std::string lastSearchInput;
    const std::string GOOGLE_API_KEY = "AIzaSyBx3dMjzZz8TXdcxsifLJojNHghdNtYWDA";
    struct CityInfo {
        std::string name;
        std::string country;
    };
    std::vector<CityInfo> allCities;

    bool isLoggedIn;  // New flag for login status
    char username[128];  // Username input
    char password[128];  // Password input
    char errorMessage[128];  // Error message for login form
    bool LoadCitiesFromJson();



    GLFWwindow* window;
    std::vector<CityWeather> cities;
    char citySearch[128];
    std::string searchCity;
    bool showCityDetails;
    CityWeather* currentCity;
    std::thread weatherThread;
    std::atomic<bool> stopThread;
    GLuint backgroundTexture;
    GLuint searchIconTexture;
    GLuint detailsButtonIconTexture; // New variable
    GLuint favoriteButtonIconTexture;
    GLuint favoriteButtonIconTextureFilled;
    GLuint welcomeIconTexture;
    bool showFavoriteCities; // New item
    void FetchCityDetails(const std::string& city);
    GLuint LoadIconTexture(const char* filename); // New method

    void SaveCredentials();  // New method to save credentials
    bool LoadCredentials();  // New method to load credentials

    void AddFavoriteCity(const std::string& cityName);
    void RemoveFavoriteCity(const std::string& cityName);
    void SaveFavoriteCities();
    bool LoadFavoriteCities();
    bool IsFavoriteCity(const std::string& cityName) const; // new item
    std::vector<std::string> favoriteCities;
};

#endif // WEATHER_APP_H
