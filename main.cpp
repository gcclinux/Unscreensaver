#include "headers/caching.hpp"
#include "headers/checkkey.hpp"
#include "headers/checkconfig.hpp"
#include "headers/render.hpp"
#include "headers/readconfig.hpp"
#include "headers/background.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include "headers/help.hpp"
#include "headers/setup.hpp"

namespace fs = std::filesystem;

void renderLoop(int argc, char* argv[]) {
    RenderImage(argc, argv);
}

int main(int argc, char* argv[]) {
    // Check if args are valid --help or --colour or --setup else exit with invalid args
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg != "--help" && arg != "--colour" && arg != "--color" && arg != "--setup") {
            std::cout << getHelpText();
            return 1;
        } 
    }

    // Check if the config file exists
    if (argc > 1 && argv[1] == std::string("--setup")) {
        runSetup();
        return 0;
    } else if (!isConfigFileExists()) {
        std::cerr << "Config file does not exist. Run with --setup first." << std::endl;
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