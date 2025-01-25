#include "caching.hpp"
#include "checkkey.hpp"
#include "checkconfig.hpp"
#include "render.hpp"
#include "readconfig.hpp"
#include "background.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

void renderLoop(int argc, char* argv[]) {
    RenderImage(argc, argv);
}

int main(int argc, char* argv[]) {
    // Check if the config file exists
    if (!isConfigFileExists()) {
        return 1;
    } else {
        UnScreenSaverConfig imgConfig;
        // Check if the config file exists and is valid
        if (!isConfigValid(imgConfig)) {
            return 1;
        }
    }
    UnScreenSaverConfig imgConfig = getConfigFromYaml();
    std::string picturePath = imgConfig.folderPath + "/" + imgConfig.searchPattern;

    // Initial download
    if (isAccessKeyValid()) {
        // Count the number of images in the folder
        int imageCount = 0;
        for (const auto& entry : fs::directory_iterator(picturePath)) {
            if (entry.is_regular_file()) {
                imageCount++;
            }
        }
        if (imageCount < imgConfig.quantify) {
            downloadImageFromUnsplash();
        }
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