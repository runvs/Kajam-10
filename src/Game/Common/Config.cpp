#include "Config.hpp"
#include <external/json.hpp>

#include <fstream>

ConfigSettings::ConfigSettings(std::string input_file_path)
{
    std::ifstream file_stream("config.json", std::ios_base::in);

    if (file_stream.fail()) {
        throw std::exception { "could not open config.json" };
    }

    nlohmann::json json;
    json << file_stream;

    json.at("serverIp").get_to(serverIp);
    json.at("serverPort").get_to(serverPort);
}

ConfigSettings::~ConfigSettings() { }
