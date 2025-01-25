#include "caching.hpp"
#include "checkkey.hpp"
#include "render.hpp"
#include "readconfig.hpp"
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
        if (imageCount < imgConfig.quantify) {
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

void renderLoop(int argc, char* argv[]) {
    RenderImage(argc, argv);
}

int main(int argc, char* argv[]) {
    UnScreenSaverConfig imgConfig = getConfigFromYaml();
    std::string picturePath = imgConfig.folderPath + "/" + imgConfig.searchPattern;

    // Initial download
    if (isAccessKeyValid()) {
        downloadImageFromUnsplash();
    } else {
        std::cerr << "Invalid Unsplash access key." << std::endl;
        return 1;
    }

    // Start the background download loop
    std::thread downloadThread(backgroundDownloadLoop, imgConfig, picturePath);
    downloadThread.detach();

    // Start the render loop
    std::thread renderThread(renderLoop, argc, argv);
    renderThread.join();

    return 0;
}