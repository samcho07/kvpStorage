#include "command_processor.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

CommandProcessor::CommandProcessor(std::shared_ptr<IStorage> storage) 
    : storage_(std::move(storage)) {
    if (!storage_) {
        throw std::invalid_argument("Storage cannot be null");
    }
}

void CommandProcessor::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    // Convert command to uppercase for case-insensitive handling
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    try {
        if (cmd == "SET") {
            std::string key, value;
            iss >> key;
            std::getline(iss, value);
            
            // Remove leading whitespace from value
            value = trim(value);

            if (key.empty() || value.empty()) {
                std::cout << "Error: Invalid SET command. Usage: SET <key> <value>" << std::endl;
                return;
            }

            if (storage_->set(key, value)) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Failed to set key-value pair" << std::endl;
            }
        } 
        else if (cmd == "GET") {
            std::string key;
            iss >> key;

            if (key.empty()) {
                std::cout << "Error: Invalid GET command. Usage: GET <key>" << std::endl;
                return;
            }

            auto value = storage_->get(key);
            if (value) {
                std::cout << *value << std::endl;
            } else {
                std::cout << std::endl;
            }
        } 
        else if (cmd == "DELETE") {
            std::string key;
            iss >> key;

            if (key.empty()) {
                std::cout << "Error: Invalid DELETE command. Usage: DELETE <key>" << std::endl;
                return;
            }

            if (storage_->remove(key)) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Failed to delete key" << std::endl;
            }
        } 
        else {
            std::cout << "Unknown command. Supported commands: SET, GET, DELETE" << std::endl;
        }
    } 
    catch (const std::exception& e) {
        std::cout << "Error processing command: " << e.what() << std::endl;
    }
}

void CommandProcessor::run() {
    std::string line;
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        if (line == "EXIT" || line == "exit") {
            break;
        }
        
        processCommand(line);
        std::cout << "> ";
    }
}

std::string CommandProcessor::trim(const std::string& str) const {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}
