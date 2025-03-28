#include <gtest/gtest.h>
#include "../src/storage.h"
#include <filesystem>
#include <thread>

class StorageTest : public ::testing::Test {
protected:
    std::string testDbFile = "test_storage.db";

    void SetUp() override {
        // Ensure clean state before each test
        if (std::filesystem::exists(testDbFile)) {
            std::filesystem::remove(testDbFile);
        }
    }

    void TearDown() override {
        // Clean up test file after tests
        if (std::filesystem::exists(testDbFile)) {
            std::filesystem::remove(testDbFile);
        }
    }
};

TEST_F(StorageTest, BasicSetAndGet) {
    Storage storage(testDbFile);
    
    // Test setting and retrieving a simple key-value pair
    EXPECT_TRUE(storage.set("key1", "value1"));
    
    auto retrievedValue = storage.get("key1");
    ASSERT_TRUE(retrievedValue.has_value());
    EXPECT_EQ(retrievedValue.value(), "value1");
}

TEST_F(StorageTest, SetEmptyKeyThrows) {
    Storage storage(testDbFile);
    
    // Test that setting an empty key throws an exception
    EXPECT_THROW(storage.set("", "value"), std::invalid_argument);
}

TEST_F(StorageTest, GetNonExistentKey) {
    Storage storage(testDbFile);
    
    // Test retrieving a non-existent key returns empty optional
    auto retrievedValue = storage.get("non_existent_key");
    EXPECT_FALSE(retrievedValue.has_value());
}

TEST_F(StorageTest, RemoveKey) {
    Storage storage(testDbFile);
    
    // Set a key-value pair
    EXPECT_TRUE(storage.set("remove_key", "remove_value"));
    
    // Remove the key
    EXPECT_TRUE(storage.remove("remove_key"));
    
    // Verify key is removed
    auto retrievedValue = storage.get("remove_key");
    EXPECT_FALSE(retrievedValue.has_value());
}

TEST_F(StorageTest, Persistence) {
    {
        // Scope block to ensure storage is destroyed and data is saved
        Storage storage(testDbFile);
        storage.set("persist_key", "persist_value");
    }

    // Create new storage and load
    Storage loadStorage(testDbFile);
    
    auto retrievedValue = loadStorage.get("persist_key");
    ASSERT_TRUE(retrievedValue.has_value());
    EXPECT_EQ(retrievedValue.value(), "persist_value");
}

TEST_F(StorageTest, ThreadSafetyBasicTest) {
    Storage storage(testDbFile);
    
    // Simulate multiple threads setting values
    std::vector<std::thread> threads;
    for (int i = 0; i < 100; ++i) {
        threads.emplace_back([&storage, i]() {
            storage.set("thread_key_" + std::to_string(i), 
                        "thread_value_" + std::to_string(i));
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Verify all keys were set
    for (int i = 0; i < 100; ++i) {
        auto value = storage.get("thread_key_" + std::to_string(i));
        EXPECT_TRUE(value.has_value());
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}