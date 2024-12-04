#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>

namespace fs = std::filesystem;

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
    // File paths
    const std::string originalFilePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_characterProfile_0)";
    const std::string backupBeforePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\alt_characterProfile_0_before)";
    const std::string backupAfterPath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\alt_characterProfile_0_after)";
    const std::string executablePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS.exe)";

    // Create objects
    CharacterBackup characterBackup(originalFilePath, backupBeforePath, backupAfterPath);
    ATLYSSProgram atlyssProgram(executablePath);

    // Check if the original file exists
    if (!fs::exists(originalFilePath)) {
        std::cerr << "Original file not found: " << originalFilePath << std::endl;
        return 1;
    }

    // Create "before" backup
    if (!characterBackup.createBeforeBackup()) {
        return 1;
    }

    // Start and monitor the ATLYSS program
    if (!atlyssProgram.startAndMonitor()) {
        return 1;
    }

    // Create "after" backup
    if (!characterBackup.createAfterBackup()) {
        return 1;
    }

    return 0;
}
