#include "storage.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

Storage::Storage(const std::string& filename) : filename_(filename) {
    load();
}

Storage::~Storage() noexcept {
    try {
        save();
    } catch (const std::exception& e) {
        std::cerr << "Error saving storage in destructor: " << e.what() << std::endl;
    }
}

bool Storage::set(const std::string& key, const std::string& value) {
    if (key.empty()) {
        throw std::invalid_argument("Key cannot be empty");
    }

    std::lock_guard<std::mutex> lock(mutex_);
    data_[key] = value;
    return save();
}

std::optional<std::string> Storage::get(const std::string& key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(key);
    return it != data_.end() ? std::optional<std::string>(it->second) : std::nullopt;
}

bool Storage::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(key);
    if (it != data_.end()) {
        data_.erase(it);
        return save();
    }
    return false;
}

bool Storage::load() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ifstream file(filename_);
    
    if (!file.is_open()) {
        // Not an error if file doesn't exist yet
        return true;
    }
    
    data_.clear();
    std::string line;
    while (std::getline(file, line)) {
        auto delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            data_[key] = value;
        }
    }
    
    return true;
}

bool Storage::save() {
    std::ofstream file(filename_, std::ios::trunc);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename_ << std::endl;
        return false;
    }
    
    for (const auto& [key, value] : data_) {
        file << key << "=" << value << std::endl;
    }
    
    file.flush();
    return file.good();
}