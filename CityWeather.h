#ifndef CITY_WEATHER_H
#define CITY_WEATHER_H


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

class CityWeather {
public:
    CityWeather(const std::string& name);
    void UpdateWeatherData();
    void Render(GLuint iconTexture);
    std::string GetCityName() const;
    float GetTemperature() const; // Getter for temperature
    int GetHumidity() const; // Getter for humidity
    int GetPressure() const; // Getter for pressure
    float GetWindSpeed() const; // Getter for wind speed

private:
    std::string cityName;
    std::string weatherLabel;
    float temperature;
    int humidity;
    int pressure;
    float windSpeed;
    GLuint weatherIconTexture;
    void FetchWeatherData();
    void UpdateTemperature(float temp);
    void UpdateHumidity(int hum);
    void UpdatePressure(int pres);
    void UpdateWindSpeed(float speed);
    GLuint LoadWeatherIcon(const std::string& filePath);
};

#endif // CITY_WEATHER_H
