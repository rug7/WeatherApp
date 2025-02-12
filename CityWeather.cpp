#include "CityWeather.h"
#include "stb_image.h"
#include "httplib.h"
#include "json.hpp"
#include "imgui.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

CityWeather::CityWeather(const std::string& name) : cityName(name), temperature(0.0f), humidity(0), pressure(0), windSpeed(0.0f), weatherIconTexture(0) {
    weatherLabel = cityName + ": Updating...";
    weatherIconTexture = LoadWeatherIcon("Externals/weather_icon.png");
}

GLuint CityWeather::LoadWeatherIcon(const std::string& filePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (data == nullptr) {
        std::cerr << "Failed to load weather icon: " << filePath << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void CityWeather::UpdateWeatherData() {
    FetchWeatherData();
}

void CityWeather::FetchWeatherData() {
    // OpenWeatherMap API key
    std::string apiKey = "fca1d27d648fbdf79758043a64459748";
    std::string endpoint = "/data/2.5/weather?q=" + cityName + "&appid=" + apiKey + "&units=metric";

    httplib::Client cli("http://api.openweathermap.org");
    auto res = cli.Get(endpoint.c_str());

    if (res && res->status == 200) {
        auto jsonResponse = json::parse(res->body);
        temperature = jsonResponse["main"]["temp"].get<float>();
        humidity = jsonResponse["main"]["humidity"].get<int>();
        pressure = jsonResponse["main"]["pressure"].get<int>();
        windSpeed = jsonResponse["wind"]["speed"].get<float>();

        weatherLabel = cityName + ": " + std::to_string(temperature) + "°C";
    }
    else {
        std::cerr << "Failed to fetch weather data for " << cityName << ": " << res.error() << std::endl;
        weatherLabel = cityName + ": Error fetching data";
    }
}

void CityWeather::UpdateTemperature(float temp) {
    temperature = temp;
    weatherLabel = cityName + ": " + std::to_string(temperature) + "°C";
}

void CityWeather::UpdateHumidity(int hum) {
    humidity = hum;
}

void CityWeather::UpdatePressure(int pres) {
    pressure = pres;
}

void CityWeather::UpdateWindSpeed(float speed) {
    windSpeed = speed;
}

void CityWeather::Render(GLuint iconTexture) {
    // Adjust card size and styling
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(10, 10));

    // Use the larger font for the city name
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
    ImGui::Text("%s", cityName.c_str());
    ImGui::PopFont();

    if (weatherIconTexture) {
        ImGui::Image((void*)(intptr_t)weatherIconTexture, ImVec2(70, 70));  // Display the weather icon
    }
    ImGui::Text("%.1f C", temperature);

    // Render details button with icon
    ImGui::PushID(this); // Push unique ID for this city
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));  // Make button background transparent
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));  // Make button background transparent when hovered
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));  // Make button background transparent when active
    if (ImGui::ImageButton((void*)(intptr_t)iconTexture, ImVec2(35, 35))) {  // Increased icon size
        // Handle button press
        ImGui::OpenPopup("CityDetailsPopup");
    }
    ImGui::PopStyleColor(3);  // Restore style colors
    ImGui::PopID();

    ImGui::PopStyleVar(4);  // Restore styles
}


std::string CityWeather::GetCityName() const {
    return cityName;
}

float CityWeather::GetTemperature() const {
    return temperature;
}

int CityWeather::GetHumidity() const {
    return humidity;
}

int CityWeather::GetPressure() const {
    return pressure;
}

float CityWeather::GetWindSpeed() const {
    return windSpeed;
}
