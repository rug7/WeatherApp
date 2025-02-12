#include "WeatherApp.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include "stb_image.h"  // To load the background image


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <cctype>
#pragma comment(lib, "ws2_32.lib")
#endif

using json = nlohmann::json;

void WeatherApp::SaveCredentials() {
    std::ofstream file("credentials.txt");
    if (file.is_open()) {
        file << username << std::endl;
        file << password << std::endl;
        file.close();
    }
    else {
        std::cerr << "Unable to open file for writing credentials." << std::endl;
    }
}

//bool WeatherApp::LoadCredentials() {
//    std::ifstream file("credentials.txt");
//    if (file.is_open()) {
//        file.getline(username, 128);
//        file.getline(password, 128);
//        file.close();
//        return true;
//    }
//    else {
//        std::cerr << "Unable to open file for reading credentials." << std::endl;
//        return false;
//    }
//}

void WeatherApp::RenderLoginForm() {
    // Render full-screen background first
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Login Background", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Image((void*)(intptr_t)loginBackgroundTexture,
                 ImGui::GetIO().DisplaySize);

    // Copyright text at bottom
    float windowWidth = ImGui::GetIO().DisplaySize.x;
    float windowHeight = ImGui::GetIO().DisplaySize.y;
    const char* copyright = "© 2025 Weather App. All rights reserved.";
    float textWidth = ImGui::CalcTextSize(copyright).x;
    ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f,
                               windowHeight - 30));
    ImGui::TextColored(ImVec4(1,1,1,0.5f), "%s", copyright);

    ImGui::End();


    // Modern color scheme
    ImVec4 bgColor = ImVec4(0.15f, 0.15f, 0.15f, 0.98f);  // Dark background
    ImVec4 accentColor = ImVec4(0.2f, 0.6f, 0.86f, 1.0f); // Blue accent
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // White text
    ImVec4 inputBgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Darker input background

    // Set window style
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleColor(ImGuiCol_Button, accentColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.65f, 0.91f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBgColor);

    // Center the login window
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImVec2 windowSize(400, 500);
    ImGui::SetNextWindowPos(ImVec2((screenSize.x - windowSize.x) * 0.5f,
                                   (screenSize.y - windowSize.y) * 0.5f));
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("Login", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove);

    // Center-aligned title
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]); // Use larger font
    float titleWidth = ImGui::CalcTextSize("Welcome").x;
    ImGui::SetCursorPosX((windowSize.x - titleWidth) * 0.5f);
    ImGui::Text("Welcome");
    ImGui::PopFont();

    // Spacing
    ImGui::Spacing();
    ImGui::Spacing();

    // Input fields with rounded corners
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 10));

    // Username
    ImGui::Text("Username");
    ImGui::SetNextItemWidth(windowSize.x - 40);
    ImGui::InputText("##username", username, IM_ARRAYSIZE(username));

    ImGui::Spacing();

    // Password
    ImGui::Text("Password");
    ImGui::SetNextItemWidth(windowSize.x - 40);
    ImGui::InputText("##password", password, IM_ARRAYSIZE(password),
                     ImGuiInputTextFlags_Password);

    ImGui::Spacing();
    ImGui::Spacing();

    float buttonWidth = 200;
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) * 0.5f);
    if (ImGui::Button("Login", ImVec2(buttonWidth, 45))) {
        // Add login logic here
        char storedUsername[128];
        char storedPassword[128];

        std::ifstream file("credentials.txt");
        if (file.is_open()) {
            file.getline(storedUsername, 128);
            file.getline(storedPassword, 128);
            file.close();

            if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
                isLoggedIn = true;
                SaveCredentials();
                errorMessage[0] = '\0';  // Clear error message
            }
            else {
                strcpy_s(errorMessage, "Invalid username or password.");
            }
        }
        else {
            std::cerr << "Unable to open credentials file." << std::endl;
            // Create default credentials if file doesn't exist
            std::ofstream createFile("credentials.txt");
            if (createFile.is_open()) {
                createFile << "1" << std::endl;
                createFile << "1" << std::endl;
                createFile.close();

                // Auto-login with default credentials
                if (strcmp(username, "1") == 0 && strcmp(password, "1") == 0) {
                    isLoggedIn = true;
                    errorMessage[0] = '\0';
                }
            }
        }
    }

    ImGui::PopStyleVar(2);

    // Error message
    if (errorMessage[0] != '\0') {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        float errorWidth = ImGui::CalcTextSize(errorMessage).x;
        ImGui::SetCursorPosX((windowSize.x - errorWidth) * 0.5f);
        ImGui::Text("%s", errorMessage);
        ImGui::PopStyleColor();
    }

    ImGui::End();
    ImGui::PopStyleColor(5);
}

GLuint LoadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

GLuint LoadPNGTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
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

GLuint WeatherApp::LoadIconTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load icon: " << filename << std::endl;
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

void WeatherApp::AddFavoriteCity(const std::string& cityName) { // new item
    favoriteCities.push_back(cityName); // new item
    SaveFavoriteCities(); // new item
}

void WeatherApp::RemoveFavoriteCity(const std::string& cityName) { // new item
    favoriteCities.erase(std::remove(favoriteCities.begin(), favoriteCities.end(), cityName), favoriteCities.end()); // new item
    SaveFavoriteCities(); // new item
}

void WeatherApp::SaveFavoriteCities() { // new item
    std::ofstream file("favorites.txt"); // new item
    if (file.is_open()) { // new item
        for (const auto& city : favoriteCities) { // new item
            file << city << std::endl; // new item
        }
        file.close(); // new item
    }
}

//bool WeatherApp::LoadFavoriteCities() { // new item
//    std::ifstream file("favorites.txt"); // new item
//    if (file.is_open()) { // new item
//        std::string city; // new item
//        while (std::getline(file, city)) { // new item
//            favoriteCities.push_back(city); // new item
//        }
//        file.close(); // new item
//        return true; // new item
//    }
//    return false; // new item
//}

bool WeatherApp::IsFavoriteCity(const std::string& cityName) const { // new item
    return std::find(favoriteCities.begin(), favoriteCities.end(), cityName) != favoriteCities.end(); // new item
}

WeatherApp::WeatherApp() {
    // Add this at the start of the constructor
    // Create default credentials if they don't exist
    std::ifstream checkFile("credentials.txt");
    if (!checkFile.good()) {
        std::ofstream createFile("credentials.txt");
        if (createFile.is_open()) {
            createFile << "1" << std::endl;
            createFile << "1" << std::endl;
            createFile.close();
            std::cout << "Created default credentials file" << std::endl;
        }
    }
    if (!LoadCitiesFromJson()) {
        std::cerr << "Failed to load cities database" << std::endl;
    }
    checkFile.close();
    // Initialize GLFW
    if (!glfwInit())
        return;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1920, 1080, "Weather App", NULL, NULL);  // Adjusted window size
    if (!window) {
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW

    glewExperimental = GL_TRUE; // Ensure GLEW uses modern techniques for managing OpenGL functionality
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW!\n");
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Load custom fonts
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Light.ttf", 30.0f);  // Default font size
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Regular.ttf", 32.0f);  // Larger font size
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Light.ttf", 60.0f);  // Time and Date font size
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Light.ttf", 40.0f);  // City Cards
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Bold.ttf", 45.0f); // Welcome
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // In the constructor
    loginBackgroundTexture = LoadTexture("Externals/mainBackground.jpg");
    if (loginBackgroundTexture == 0) {
        std::cout << "Trying alternative path..." << std::endl;
        loginBackgroundTexture = LoadTexture("../Externals/mainBackground.jpg");
    }
    if (loginBackgroundTexture == 0) {
        std::cout << "Trying absolute path..." << std::endl;
        loginBackgroundTexture = LoadTexture("C:/Users/majda/CLionProjects/WeatherAppCustom/Externals/mainBackground.jpg");
    }


    // Initialize city weather objects
    cities.push_back(CityWeather("London"));
    cities.push_back(CityWeather("New York"));
    cities.push_back(CityWeather("Tokyo"));
    cities.push_back(CityWeather("Paris"));
    cities.push_back(CityWeather("Berlin"));

    // Initialize search bar
    strcpy_s(citySearch, "");
    showCityDetails = false;
    currentCity = nullptr;
    showAutocompleteSuggestions = false;
    lastSearchInput = "";
    autocompleteSuggestions.clear();


    // Load background image
    backgroundTexture = LoadTexture("Externals/wallpaper.jpg");  // Updated image path

    // Load search icon
    searchIconTexture = LoadPNGTexture("Externals/search_icon.png");  // Path to your PNG image

    // Load button icon texture
    detailsButtonIconTexture = LoadIconTexture("Externals/details_icon.png");

    // Load favorite button icon textures
    favoriteButtonIconTexture = LoadIconTexture("Externals/favorite_icon.png");  // Path to your favorite icon PNG
    favoriteButtonIconTextureFilled = LoadIconTexture("Externals/starred_icon.png");

    // Initialize the stop flag for the weather fetch thread
    stopThread = false;

    // Initialize login form variables
    isLoggedIn = false; // Start with the login form
    strcpy_s(username, "");
    strcpy_s(password, "");
    // Initialize error message
    strcpy_s(errorMessage, "");

    // Start the weather fetch thread
    weatherThread = std::thread(&WeatherApp::FetchAndDisplayWeather, this);
}



WeatherApp::~WeatherApp() {
    // Stop the weather fetch thread
    stopThread = true;
    if (weatherThread.joinable()) {
        weatherThread.join();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete textures
    glDeleteTextures(1, &loginBackgroundTexture);
    glDeleteTextures(1, &backgroundTexture);
    glDeleteTextures(1, &searchIconTexture);
    glDeleteTextures(1, &detailsButtonIconTexture);
    glDeleteTextures(1, &favoriteButtonIconTexture);
    glDeleteTextures(1, &favoriteButtonIconTextureFilled);

    glfwDestroyWindow(window);
    glfwTerminate();

    // Delete background texture
    glDeleteTextures(1, &backgroundTexture);
}

void WeatherApp::Run() {
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render the GUI
        if (!isLoggedIn) {
            RenderLoginForm();  // Render the login form if not logged in
        }
        else {
            RenderGUI();  // Render the main GUI if logged in
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}
void WeatherApp::RenderGUI() {
    // Professional color scheme
    ImVec4 bgColor = ImVec4(0.13f, 0.17f, 0.23f, 1.0f);
    ImVec4 cardColor = ImVec4(0.18f, 0.23f, 0.30f, 0.95f);
    ImVec4 accentColor = ImVec4(0.31f, 0.60f, 0.89f, 1.0f);
    ImVec4 textColor = ImVec4(0.90f, 0.92f, 0.95f, 1.0f);

    // Set styles
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, cardColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleColor(ImGuiCol_Button, accentColor);

    // Main window
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1920, 1080), ImGuiCond_Always);

    if (ImGui::Begin("Weather App", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove)) {

        // Background Image
        ImGui::GetWindowDrawList()->AddImage(
                (void*)(intptr_t)backgroundTexture,
                ImVec2(0, 0),
                ImVec2(1920, 1080)
        );

        // Time and Date section
        float margin = 70.0f;
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        ImGui::SetCursorPos(ImVec2(margin, margin));
        ImGui::Text("%s", GetCurrentTime1().c_str());
        ImGui::SetCursorPos(ImVec2(margin, margin + 70));
        ImGui::Text("%s", GetCurrentDate().c_str());
        ImGui::PopFont();

        // Search bar
        ImGui::SetCursorPos(ImVec2(margin, margin + 200));

// Style setup for search bar
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 25.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 12));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.20f, 0.25f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.17f, 0.22f, 0.27f, 0.95f));

// Search input
        static char searchBuffer[256] = "";
        static int currentItem = -1;

// Search bar with clear button
        ImGui::SetNextItemWidth(670);
        bool searchChanged = ImGui::InputText("##Search", searchBuffer, IM_ARRAYSIZE(searchBuffer),
                                              ImGuiInputTextFlags_EnterReturnsTrue);

// Clear button
        if (strlen(searchBuffer) > 0) {
            ImGui::SameLine();

            // Clear button styling
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.3f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.35f, 0.35f, 0.6f));

            // Render clear button
            if (ImGui::Button("×##clear", ImVec2(35, 35))) {
                searchBuffer[0] = '\0';
                showAutocompleteSuggestions = false;
                currentItem = -1;
                lastSearchInput = "";
            }

            ImGui::PopStyleColor(3);
        }

// Pop search bar styles
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);


// Handle keyboard navigation
        if (ImGui::IsItemFocused()) {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
                if (currentItem > 0) {
                    currentItem--;
                } else if (currentItem == -1 && !autocompleteSuggestions.empty()) {
                    currentItem = autocompleteSuggestions.size() - 1;
                }
            }
            else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
                if (currentItem < (int)autocompleteSuggestions.size() - 1) {
                    currentItem++;
                } else if (currentItem == -1 && !autocompleteSuggestions.empty()) {
                    currentItem = 0;
                }
            }
            else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
                if (currentItem >= 0 && currentItem < (int)autocompleteSuggestions.size()) {
                    const std::string& selected = autocompleteSuggestions[currentItem];
                    strcpy_s(searchBuffer, selected.c_str());

                    // Extract city name without country
                    std::string cityName = selected;
                    size_t commaPos = selected.find(',');
                    if (commaPos != std::string::npos) {
                        cityName = selected.substr(0, commaPos);
                    }

                    strcpy_s(citySearch, cityName.c_str());
                    searchCity = cityName;
                    FetchCityDetails(cityName);
                    showAutocompleteSuggestions = false;
                    currentItem = -1;
                } else if (strlen(searchBuffer) > 0) {
                    // Handle direct input
                    searchCity = searchBuffer;
                    FetchCityDetails(searchCity);
                    showAutocompleteSuggestions = false;
                    currentItem = -1;
                }
            }
            else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
                showAutocompleteSuggestions = false;
                currentItem = -1;
            }
        }

// Check for changes in input to show suggestions
        if (strlen(searchBuffer) >= 2) {
            if (lastSearchInput != searchBuffer) {
                lastSearchInput = searchBuffer;
                autocompleteSuggestions.clear();
                currentItem = -1;

                FetchCitySuggestions(searchBuffer);
                showAutocompleteSuggestions = true;
            }
        } else {
            showAutocompleteSuggestions = false;
        }

// Show suggestions dropdown
        if (showAutocompleteSuggestions && !autocompleteSuggestions.empty()) {
            ImGui::SetNextWindowPos(ImVec2(margin, margin + 250));
            ImGui::SetNextWindowSize(ImVec2(700, ImGui::GetTextLineHeightWithSpacing() *
                                                 std::min(autocompleteSuggestions.size(), size_t(5)) + 20));

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.25f, 0.32f, 0.95f));

            if (ImGui::Begin("##Suggestions", nullptr,
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoSavedSettings)) {

                for (int i = 0; i < autocompleteSuggestions.size(); i++) {
                    const bool is_selected = (i == currentItem);
                    if (ImGui::Selectable(autocompleteSuggestions[i].c_str(), is_selected)) {
                        strcpy_s(searchBuffer, autocompleteSuggestions[i].c_str());
                        strcpy_s(citySearch, autocompleteSuggestions[i].c_str());
                        searchCity = autocompleteSuggestions[i];
                        FetchCityDetails(searchCity);
                        showAutocompleteSuggestions = false;
                        currentItem = -1;
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }

                    // Keep suggestions visible when hovering
                    if (ImGui::IsItemHovered()) {
                        currentItem = i;
                    }
                }
                ImGui::End();
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }

// Handle Enter key press for direct input
        if (searchChanged) {
            if (strlen(searchBuffer) > 0) {
                strcpy_s(citySearch, searchBuffer);
                searchCity = citySearch;
                FetchCityDetails(searchCity);
                showAutocompleteSuggestions = false;
                currentItem = -1;
            }
        }
//
//        ImGui::PopItemWidth();
//        ImGui::PopStyleColor(2);
//        ImGui::PopStyleVar(2);

        // Weather Cards
        float cardWidth = 220.0f;
        float cardHeight = 160.0f;
        float startY = margin + 350.0f;
        float spacingX = 40.0f;

        // Only display MAX_CITIES cards
        for (size_t i = 0; i < std::min(cities.size(), (size_t)MAX_CITIES); ++i) {
            ImGui::SetCursorPos(ImVec2(margin + i * (cardWidth + spacingX), startY));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 15.0f);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.20f, 0.25f, 0.32f, 0.95f));

            if (ImGui::BeginChild(("WeatherCard" + std::to_string(i)).c_str(), ImVec2(cardWidth, cardHeight), true)) {
                // City name
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                float cityNameWidth = ImGui::CalcTextSize(cities[i].GetCityName().c_str()).x;
                ImGui::SetCursorPosX((cardWidth - cityNameWidth) * 0.5f);
                ImGui::SetCursorPosY(15);
                ImGui::Text("%s", cities[i].GetCityName().c_str());
                ImGui::PopFont();

                // Temperature
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                std::string tempStr = std::to_string(static_cast<int>(cities[i].GetTemperature())) + "°C";
                float tempWidth = ImGui::CalcTextSize(tempStr.c_str()).x;
                ImGui::SetCursorPosX((cardWidth - tempWidth) * 0.5f);
                ImGui::SetCursorPosY(cardHeight * 0.4f);
                ImGui::Text("%s", tempStr.c_str());
                ImGui::PopFont();

                // Buttons
                ImGui::SetCursorPosY(cardHeight - 40);
                ImGui::SetCursorPosX(10);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                if (ImGui::ImageButton((void*)(intptr_t)detailsButtonIconTexture, ImVec2(30, 30))) {
                    currentCity = &cities[i];
                    showCityDetails = true;
                }
                ImGui::PopStyleColor(3);

                ImGui::SameLine();
                ImGui::SetCursorPosX(cardWidth - 40);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 0.7f));
                if (ImGui::Button(("×##" + std::to_string(i)).c_str(), ImVec2(30, 30))) {
                    cities.erase(cities.begin() + i);
                }
                ImGui::PopStyleColor(2);

                ImGui::EndChild();
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }

        // Warning message if there are more cities than can be displayed
        if (cities.size() > MAX_CITIES) {
            ImGui::SetCursorPos(ImVec2(margin, startY + cardHeight + 10));
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
                               "Maximum number of visible cities reached (%d/%zu)",
                               MAX_CITIES, cities.size());
        }

        // Favorite Cities Panel
        ImGui::SetCursorPos(ImVec2(1580, 70));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.20f, 0.25f, 0.32f, 0.95f));

        if (ImGui::BeginChild("Favorite Cities", ImVec2(300, 300), true)) {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            float listWidth = ImGui::GetWindowSize().x;
            ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.0f, 1.0f), "Favorite Cities");
            ImGui::PopFont();

            ImGui::Separator();
            ImGui::Spacing();

            if (favoriteCities.empty()) {
                ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
                float textWidth = ImGui::CalcTextSize("No favorite cities yet").x;
                ImGui::SetCursorPosX((listWidth - textWidth) * 0.5f);
                ImGui::TextDisabled("No favorite cities yet");
            } else {
                for (size_t i = 0; i < favoriteCities.size(); i++) {
                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.22f, 0.27f, 0.33f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.30f, 0.36f, 0.9f));

                    if (ImGui::Button(favoriteCities[i].c_str(), ImVec2(listWidth - 45, 35))) {
                        searchCity = favoriteCities[i];
                        FetchCityDetails(searchCity);
                        showCityDetails = true;
                    }

                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 0.7f));
                    if (ImGui::Button("×", ImVec2(35, 35))) {
                        RemoveFavoriteCity(favoriteCities[i]);
                    }
                    ImGui::PopStyleColor(4);
                    ImGui::PopID();
                    ImGui::Spacing();
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        // Footer
        float windowHeight = ImGui::GetIO().DisplaySize.y;
        float footerHeight = 30.0f;
        ImGui::SetCursorPos(ImVec2(0, windowHeight - footerHeight));

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
        if (ImGui::BeginChild("Footer", ImVec2(ImGui::GetIO().DisplaySize.x, footerHeight), false)) {
            const char* copyright = "© 2025 Weather App. All rights reserved.";
            float textWidth = ImGui::CalcTextSize(copyright).x;
            ImGui::SetCursorPos(ImVec2((ImGui::GetIO().DisplaySize.x - textWidth) * 0.5f, 5));
            ImGui::TextColored(ImVec4(1,1,1,0.7f), "%s", copyright);
            ImGui::EndChild();
        }
        ImGui::PopStyleColor();

        ImGui::End();

        if (showCityDetails && currentCity) {
            RenderCityDetails();
        }
        ImGui::PopStyleColor(4); // Pop the initial style colors

    }
}

void WeatherApp::FetchPlacesAutocomplete(const std::string& input) {
    // Encode the input for URL
    std::string encodedInput;
    for (char c : input) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encodedInput += c;
        } else {
            char hex[4];
            sprintf_s(hex, "%%%02X", (unsigned char)c);
            encodedInput += hex;
        }
    }

    // Construct the URL
    std::string url = "/maps/api/place/autocomplete/json"
                      "?input=" + encodedInput +
                      "&types=(cities)" +
                      "&key=" + GOOGLE_API_KEY;

    // Make the API request
    httplib::Client cli("maps.googleapis.com");
    auto res = cli.Get(url.c_str());

    if (res && res->status == 200) {
        try {
            auto json = nlohmann::json::parse(res->body);
            autocompleteSuggestions.clear();

            // Parse predictions
            if (json.contains("predictions")) {
                for (const auto& prediction : json["predictions"]) {
                    if (prediction.contains("description")) {
                        autocompleteSuggestions.push_back(prediction["description"].get<std::string>());
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to parse Places API response: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Failed to fetch Places autocomplete suggestions" << std::endl;
    }
}

void WeatherApp::RenderCityDetails() {
    // Center window
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 400));

    // Create window
    if (ImGui::Begin("City Details", &showCityDetails,
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize)) {

        // City name and favorite button
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        ImGui::Text("%s", currentCity->GetCityName().c_str());
        ImGui::PopFont();

        ImGui::SameLine();
        bool isFavorite = IsFavoriteCity(currentCity->GetCityName());
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
        // Use ImageButton instead of Button with the appropriate icon
        if (ImGui::ImageButton((void*)(intptr_t)(isFavorite ? favoriteButtonIconTextureFilled : favoriteButtonIconTexture),
                               ImVec2(30, 30))) {
            if (isFavorite) {
                RemoveFavoriteCity(currentCity->GetCityName());
            } else {
                AddFavoriteCity(currentCity->GetCityName());
            }
        }
        ImGui::PopStyleColor(3);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Weather details in a grid
        if (ImGui::BeginTable("weather_details", 2, ImGuiTableFlags_BordersInnerV)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Temperature");
            ImGui::TableNextColumn();
            ImGui::Text("%.1f°C", currentCity->GetTemperature());

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Humidity");
            ImGui::TableNextColumn();
            ImGui::Text("%d%%", currentCity->GetHumidity());

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Pressure");
            ImGui::TableNextColumn();
            ImGui::Text("%d hPa", currentCity->GetPressure());

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Wind Speed");
            ImGui::TableNextColumn();
            ImGui::Text("%.1f m/s", currentCity->GetWindSpeed());

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Center the close button
        float windowWidth = ImGui::GetWindowSize().x;
        float buttonWidth = 120;
        ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

        if (ImGui::Button("Close", ImVec2(buttonWidth, 35))) {
            showCityDetails = false;
        }
    }
    ImGui::End();
}
std::string WeatherApp::GetCurrentTime1() {
    auto now = std::time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%H:%M:%S");
    return oss.str();
}

std::string WeatherApp::GetCurrentDate() {
    auto now = std::time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%A, %d %B %Y");
    return oss.str();
}

void WeatherApp::FetchAndDisplayWeather() {
    // List of cities
    std::vector<std::string> cityNames = { "London", "New York", "Tokyo", "Paris", "Berlin" };

    while (!stopThread.load()) {
        for (size_t i = 0; i < cities.size(); ++i) {
            cities[i].UpdateWeatherData();
        }

        // Add a delay to prevent constant API calls
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
}

void WeatherApp::FetchCityDetails(const std::string& city) {
    // Extract city name from "City, Country" format
    std::string cityName = city;
    size_t commaPos = city.find(',');
    if (commaPos != std::string::npos) {
        cityName = city.substr(0, commaPos);
    }

    // Rest of your existing code, but use cityName for creating the CityWeather object
    const std::string API_KEY = "fca1d27d648fbdf79758043a64459748";
    httplib::Client cli("http://api.openweathermap.org");

    auto res = cli.Get(("/data/2.5/weather?q=" + cityName + "&appid=" + API_KEY + "&units=metric").c_str());

    if (res && res->status == 200) {
        // City exists, proceed with adding/updating
        auto json = nlohmann::json::parse(res->body);

        // Check if city already exists
        for (auto& cityWeather : cities) {
            if (cityWeather.GetCityName() == cityName) {  // Compare with cityName instead of city
                cityWeather.UpdateWeatherData();
                currentCity = &cityWeather;
                showCityDetails = true;
                return;
            }
        }

        // Create a new city with only the city name
        CityWeather newCity(cityName);  // Use cityName instead of city
        newCity.UpdateWeatherData();

        if (cities.size() < MAX_CITIES) {
            cities.push_back(newCity);
            currentCity = &cities.back();
        } else {
            tempCity = newCity;
            currentCity = &tempCity;
        }
        showCityDetails = true;
    } else {
        showNotification = true;
        notificationMessage = "City not found!";
        notificationTimer = 2.0f;
    }
}

void WeatherApp::FetchCitySuggestions(const std::string& input) {
    if (input.length() < 2) return;

    autocompleteSuggestions.clear();
    std::string inputLower = input;
    std::transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);

    for (const auto& city : allCities) {
        std::string cityLower = city.name;
        std::transform(cityLower.begin(), cityLower.end(), cityLower.begin(), ::tolower);

        if (cityLower.find(inputLower) != std::string::npos) {
            // Add city name with country
            autocompleteSuggestions.push_back(city.name + ", " + city.country);
        }
    }

    // Limit the number of suggestions
    if (autocompleteSuggestions.size() > 10) {
        autocompleteSuggestions.resize(10);
    }
}
bool WeatherApp::LoadCitiesFromJson() {
    std::ifstream file("Externals/cities.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open cities.json" << std::endl;
        return false;
    }

    try {
        json citiesJson = json::parse(file);
        allCities.clear();

        for (const auto& city : citiesJson["cities"]) {
            CityInfo cityInfo;
            cityInfo.name = city["name"].get<std::string>();
            cityInfo.country = city["country"].get<std::string>();
            allCities.push_back(cityInfo);
        }

        // Sort cities by name
        std::sort(allCities.begin(), allCities.end(),
                  [](const CityInfo& a, const CityInfo& b) {
                      return a.name < b.name;
                  });

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing cities.json: " << e.what() << std::endl;
        return false;
    }
}