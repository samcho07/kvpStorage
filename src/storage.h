#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <optional>
#include <stdexcept>

/**
 * @brief Interface for key-value storage operations
 * 
 * Defines the contract for key-value storage with thread-safe operations
 */
class IStorage {
public:
    virtual ~IStorage() = default;

    /**
     * @brief Set a key-value pair in storage
     * @param key The key to store
     * @param value The value to associate with the key
     * @return bool Indicates success of the operation
     * @throws std::invalid_argument If key is empty
     */
    virtual bool set(const std::string& key, const std::string& value) = 0;

    /**
     * @brief Retrieve a value for a given key
     * @param key The key to look up
     * @return std::optional<std::string> The value if found, empty otherwise
     */
    virtual std::optional<std::string> get(const std::string& key) const = 0;

    /**
     * @brief Remove a key-value pair
     * @param key The key to remove
     * @return bool Indicates success of the operation
     */
    virtual bool remove(const std::string& key) = 0;

    /**
     * @brief Load data from persistent storage
     * @return bool Indicates success of loading
     */
    virtual bool load() = 0;

    /**
     * @brief Save data to persistent storage
     * @return bool Indicates success of saving
     */
    virtual bool save() = 0;
};

/**
 * @brief Concrete implementation of key-value storage
 * 
 * Provides thread-safe, file-based key-value storage with persistence
 */
class Storage : public IStorage {
public:
    /**
     * @brief Construct a new Storage object
     * @param filename Path to the storage file
     */
    explicit Storage(const std::string& filename);
    
    /**
     * @brief Destructor to ensure data is saved
     */
    ~Storage() noexcept override;

    /**
     * @brief Set a key-value pair
     * @param key The key to store
     * @param value The value to associate with the key
     * @return bool Indicates success of the operation
     * @throws std::invalid_argument If key is empty
     */
    bool set(const std::string& key, const std::string& value) override;

    /**
     * @brief Retrieve a value for a given key
     * @param key The key to look up
     * @return std::optional<std::string> The value if found, empty otherwise
     */
    std::optional<std::string> get(const std::string& key) const override;

    /**
     * @brief Remove a key-value pair
     * @param key The key to remove
     * @return bool Indicates success of the operation
     */
    bool remove(const std::string& key) override;

    /**
     * @brief Load data from file to memory
     * @return bool Indicates success of loading
     */
    bool load() override;

    /**
     * @brief Save data from memory to file
     * @return bool Indicates success of saving
     */
    bool save() override;

private:
    std::string filename_;                                  // Storage file name
    std::unordered_map<std::string, std::string> data_;     // In-memory storage
    mutable std::mutex mutex_;                              // Thread synchronization
};