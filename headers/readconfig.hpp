#pragma once
#include <string>
#include <yaml-cpp/yaml.h>

struct UnScreenSaverConfig {
    int quantify;
    int width;
    int height;
    int duration;
    std::string folderPath;
    std::string accessKey;
    std::string secretKey;
    std::string searchPattern;
};

UnScreenSaverConfig getConfigFromYaml();