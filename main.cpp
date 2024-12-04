#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>

namespace fs = std::filesystem;

int main() {
    // Path to the file to backup
    const std::string originalFilePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\atl_characterProfile_0)";
    const std::string backupFilePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS_Data\profileCollections\alt_characterProfile_0_before)";

    // Path to the ATLYSS.exe executable
    const char* executablePath = R"(D:\SteamLibrary\steamapps\common\ATLYSS\ATLYSS.exe)";

    // Check if the original file exists
    if (!fs::exists(originalFilePath)) {
        std::cerr << "Original file not found: " << originalFilePath << std::endl;
        return 1;
    }

    // Delete the backup file if it exists
    try {
        if (fs::exists(backupFilePath)) {
            fs::remove(backupFilePath);
            std::cout << "Existing backup file removed: " << backupFilePath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to remove existing backup: " << e.what() << std::endl;
        return 1;
    }

    // Create a new backup
    try {
        fs::copy_file(originalFilePath, backupFilePath);
        std::cout << "Backup created: " << backupFilePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to create backup: " << e.what() << std::endl;
        return 1;
    }

    // Create a STARTUPINFO structure
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi = { 0 };

    // Start the process
    if (!CreateProcess(
            NULL,                 // No module name (use command line)
            const_cast<char*>(executablePath), // Command line
            NULL,                 // Process handle not inheritable
            NULL,                 // Thread handle not inheritable
            FALSE,                // Set handle inheritance to FALSE
            0,                    // No creation flags
            NULL,                 // Use parent's environment block
            NULL,                 // Use parent's starting directory
            &si,                  // Pointer to STARTUPINFO structure
            &pi)                  // Pointer to PROCESS_INFORMATION structure
    ) {
        std::cerr << "Failed to start Atlyss.exe. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "ATLYSS.exe started with PID: " << pi.dwProcessId << std::endl;

    // Monitor the process
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Once the process ends, print the message
    std::cout << "ATLYSS stopped" << std::endl;

    // Clean up
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
