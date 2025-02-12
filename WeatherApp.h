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


private:
    void Initialize();
    void Cleanup();
    void RenderGUI();
    void RenderCityDetails();
    void RenderLoginForm();  // New method for login form
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

    void FetchCityDetails(const std::string& city);
    static GLuint LoadIconTexture(const char* filename); // New method
    void SaveCredentials();  // New method to save credentials
    void AddFavoriteCity(const std::string& cityName);
    void RemoveFavoriteCity(const std::string& cityName);
    void SaveFavoriteCities();
    bool IsFavoriteCity(const std::string& cityName) const; // new item
    std::vector<std::string> favoriteCities;
};

#endif // WEATHER_APP_H
