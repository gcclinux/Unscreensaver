#include "headers/background.hpp"
#include "headers/caching.hpp"
#include "headers/checkkey.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

void backgroundDownloadLoop(const UnScreenSaverConfig& imgConfig, const std::string& picturePath) {
    while (true) {
        // Count the number of images in the folder
        int imageCount = 0;
        for (const auto& entry : fs::directory_iterator(picturePath)) {
            if (entry.is_regular_file()) {
                imageCount++;
            }
        }

        // Check if the number of images is less than imgConfig.quantify
        if (imageCount < imgConfig.quantify && imageCount < 50) {
            if (isAccessKeyValid()) {
                downloadImageFromUnsplash();
            } else {
                std::cerr << "Invalid Unsplash access key." << std::endl;
                break;
            }
        }

        // Sleep for (imgConfig.duration - 1) seconds
        std::this_thread::sleep_for(std::chrono::seconds(imgConfig.duration - 1));
    }
}