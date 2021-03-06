#include "Config.hpp"
#include <external/json.hpp>

#include <fstream>

ConfigSettings::ConfigSettings(std::string input_file_path)
{
    std::ifstream file_stream("config.json", std::ios_base::in);
    if (file_stream.fail()) {
        throw std::exception { "could not open config.json" };
    }

    nlohmann::json json = nlohmann::json::parse(file_stream);

    json.at("serverIp").get_to(serverIp);
    json.at("serverPort").get_to(serverPort);
    json.at("clientPort").get_to(clientPort);
}

ConfigSettings::~ConfigSettings() { }
