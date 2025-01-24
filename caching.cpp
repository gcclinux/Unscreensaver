#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <filesystem>
#include "readconfig.hpp"
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

// Callback function to handle the data received from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Callback function to write data to a file
size_t WriteFileCallback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void downloadImageFromUnsplash() {
    // Create an instance of UnScreenSaverConfig and set the values
    UnScreenSaverConfig config = getConfigFromYaml();

    // Construct the URL
    std::string url = "https://api.unsplash.com/photos/random?query=" + config.searchPattern + "&client_id=" + config.accessKey;

    // Initialize CURL
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    // Parse the JSON response
    auto jsonResponse = nlohmann::json::parse(readBuffer);
    std::string imageId = jsonResponse["id"];
    std::string imageUrl = jsonResponse["urls"]["small"];

    // Construct the image path
    std::string folderPath = config.folderPath + "/" + config.searchPattern;
    std::string imagePath = folderPath + "/" + imageId + ".jpg";

    // Check if the folder exists, if not create it
    if (!fs::exists(folderPath)) {
        for (const auto& entry : fs::directory_iterator(config.folderPath)) {
            fs::remove_all(entry.path());
        }
        fs::create_directories(folderPath);
    }

    // Check if the file already exists
    std::ifstream file(imagePath);
    if (file.good()) {
        std::cout << "File already exists: " << imagePath << std::endl;
        return;
    }

    // Download the image
    curl = curl_easy_init();
    if(curl) {
        FILE* fp = fopen(imagePath.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, imageUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        fclose(fp);
        curl_easy_cleanup(curl);
    }
}