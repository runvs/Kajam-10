#ifndef CONFIG_HPP_GUARD
#define CONFIG_HPP_GUARD

#include <external/json.hpp>
#include <string>

class ConfigSettings {
public:
    ConfigSettings(std::string input_file_path);
    ~ConfigSettings();

    std::string serverIp;
    unsigned short serverPort;
    unsigned short clientPort;
};

#endif // CONFIG_HPP_GUARD
