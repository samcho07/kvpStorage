#pragma once

#include "storage.h"
#include <string>
#include <memory>

/**
 * @brief Interface for command processing
 * 
 * Defines the contract for processing user commands
 */
class ICommandProcessor {
public:
    virtual ~ICommandProcessor() = default;

    /**
     * @brief Process a single command
     * @param command The command string to process
     */
    virtual void processCommand(const std::string& command) = 0;

    /**
     * @brief Run the command processing loop
     */
    virtual void run() = 0;
};

/**
 * @brief Concrete implementation of command processor
 * 
 * Handles user interactions and command parsing for key-value storage
 */
class CommandProcessor : public ICommandProcessor {
public:
    /**
     * @brief Construct a new Command Processor
     * @param storage Shared pointer to storage implementation
     */
    explicit CommandProcessor(std::shared_ptr<IStorage> storage);

    /**
     * @brief Process a single command
     * @param command The command string to process
     */
    void processCommand(const std::string& command) override;

    /**
     * @brief Run the command processing loop
     */
    void run() override;

private:
    std::shared_ptr<IStorage> storage_;

    /**
     * @brief Trim leading and trailing whitespace from a string
     * @param str The string to trim
     * @return std::string Trimmed string
     */
    std::string trim(const std::string& str) const;
};