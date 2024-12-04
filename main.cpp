#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <vector>

namespace fs = std::filesystem;

// Class to handle backup operations for a list of files
class StorageBackup {
private:
    std::vector<std::pair<std::string, std::string>> backupFiles; // Pairs of original file paths and their backup paths

public:
    // Constructor
    StorageBackup(const std::vector<std::pair<std::string, std::string>>& files) : backupFiles(files) {}

    // Create backups
    bool createBackups() {
        for (const auto& [originalPath, backupPath] : backupFiles) {
            try {
                if (fs::exists(backupPath)) {
                    fs::remove(backupPath);
                    std::cout << "Existing backup file removed: " << backupPath << std::endl;
                }
                fs::copy_file(originalPath, backupPath);
                std::cout << "Backup created: " << backupPath << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Failed to create backup (" << backupPath << "): " << e.what() << std::endl;
                return false;
            }
        }
        return true;
    }
};

// Class to handle character backup operations
class CharacterBackup {
private:
    std::string originalFilePath;
    std::string backupBeforePath;
    std::string backupAfterPath;

public:
    // Constructor
    CharacterBackup(const std::string& original, const std::string& before, const std::string& after)
        : originalFilePath(original), backupBeforePath(before), backupAfterPath(after) {}

    // Create a backup file
    bool createBackup(const std::string& backupPath) {
        try {
            if (fs::exists(backupPath)) {
                fs::remove(backupPath);
                std::cout << "Existing backup file removed: " << backupPath << std::endl;
            }
            fs::copy_file(originalFilePath, backupPath);
            std::cout << "Backup created: " << backupPath << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Failed to create backup (" << backupPath << "): " << e.what() << std::endl;
            return false;
        }
    }

    // Create "before" backup
    bool createBeforeBackup() {
        return createBackup(backupBeforePath);
    }

    // Create "after" backup
    bool createAfterBackup() {
        return createBackup(backupAfterPath);
    }
};

// Class to handle ATLYSS program execution
class ATLYSSProgram {
private:
    std::string executablePath;

public:
    // Constructor
    ATLYSSProgram(const std::string& path) : executablePath(path) {}

    // Start the program and wait for it to close
    bool startAndMonitor() {
        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi = { 0 };

        if (!CreateProcess(
                NULL,                          // No module name (use command line)
                const_cast<char*>(executablePath.c_str()), // Command line
                NULL,                          // Process handle not inheritable
                NULL,                          // Thread handle not inheritable
                FALSE,                         // Set handle inheritance to FALSE
                0,                             // No creation flags
                NULL,                          // Use parent's environment block
                NULL,                          // Use parent's starting directory
                &si,                           // Pointer to STARTUPINFO structure
                &pi)                           // Pointer to PROCESS_INFORMATION structure
        ) {
            std::cerr << "Failed to start ATLYSS.exe. Error code: " << GetLastError() << std::endl;
            return false;
        }

        std::cout << "ATLYSS.exe started with PID: " << pi.dwProcessId << std::endl;

        // Wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);

        std::cout << "ATLYSS stopped" << std::endl;

        // Clean up
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return true;
    }
};

// Main function
int main() {
    // File paths for character backup
    const std::string characterOriginalFilePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_characterProfile_0)";
    const std::string characterBackupBeforePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\alt_characterProfile_0_before)";
    const std::string characterBackupAfterPath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\alt_characterProfile_0_after)";

    // File paths for storage backup
    const std::vector<std::pair<std::string, std::string>> storageFiles = {
        {R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank)",
         R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_before)"},
        {R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_01)",
         R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_01_before)"},
        {R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_02)",
         R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_02_before)"}
    };

    // File paths for "after" storage backup
    const std::vector<std::pair<std::string, std::string>> storageFilesAfter = {
        {R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank)",
         R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_after)"},
        {R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_01)",
         R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_01_after)"},
        {R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_02)",
         R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_itemBank_02_after)"}
    };

    const std::string executablePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS.exe)";

    // Create objects
    CharacterBackup characterBackup(characterOriginalFilePath, characterBackupBeforePath, characterBackupAfterPath);
    StorageBackup storageBackupBefore(storageFiles);
    StorageBackup storageBackupAfter(storageFilesAfter);
    ATLYSSProgram atlyssProgram(executablePath);

    // Check if the original file exists
    if (!fs::exists(characterOriginalFilePath)) {
        std::cerr << "Original character file not found: " << characterOriginalFilePath << std::endl;
        return 1;
    }

    // Create "before" backups
    if (!characterBackup.createBeforeBackup() || !storageBackupBefore.createBackups()) {
        return 1;
    }

    // Start and monitor the ATLYSS program
    if (!atlyssProgram.startAndMonitor()) {
        return 1;
    }

    // Create "after" backups
    if (!characterBackup.createAfterBackup() || !storageBackupAfter.createBackups()) {
        return 1;
    }

    return 0;
}
