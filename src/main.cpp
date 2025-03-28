#include "storage.h"
#include "command_processor.h"
#include <iostream>
#include <memory>

/**
 * @brief Application entry point
 * @param argc Number of command-line arguments
 * @param argv Command-line argument array
 * @return int Exit status
 */
int main(int argc, char* argv[]) {
    try {
        // Default storage file name
        std::string fileName = "kvp_storage.db";

        // Allow custom file name via command-line argument
        if (argc > 1) {
            fileName = argv[1];
        }

        // Create storage with specified file
        auto storage = std::make_shared<Storage>(fileName);

        // Verify storage is loaded successfully
        if (!storage->load()) {
            std::cerr << "Failed to load storage from file: " << fileName << std::endl;
            return 1;
        }

        // Create command processor with storage
        CommandProcessor processor(storage);

        // Run interactive command processing
        processor.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}