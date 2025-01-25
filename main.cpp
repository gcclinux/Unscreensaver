#include "caching.hpp"
#include "checkkey.hpp"
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