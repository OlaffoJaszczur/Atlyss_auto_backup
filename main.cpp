#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <windows.h>

namespace fs = std::filesystem;

// Helper function to get the current executable's directory
std::string getCurrentDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    return fs::path(buffer).parent_path().string();
}

// Class to handle character backup operations
class CharacterBackup {
private:
    std::string directoryPath;   // Directory containing the files
    std::string filePrefix;      // File name prefix
    std::string backupSuffixBefore;
    std::string backupSuffixAfter;
    std::vector<int> fileIndices; // Indices for the character files

public:
    // Constructor
    CharacterBackup(const std::string& directory, const std::string& prefix,
                    const std::string& suffixBefore, const std::string& suffixAfter,
                    const std::vector<int>& indices)
        : directoryPath(directory), filePrefix(prefix),
          backupSuffixBefore(suffixBefore), backupSuffixAfter(suffixAfter),
          fileIndices(indices) {}

    // Create "before" backups for all relevant files
    bool createBeforeBackups() {
        return createBackups(backupSuffixBefore);
    }

    // Create "after" backups for all relevant files
    bool createAfterBackups() {
        return createBackups(backupSuffixAfter);
    }

private:
    // Helper function to create backups with a specific suffix
    bool createBackups(const std::string& backupSuffix) {
        bool allBackupsSuccessful = true;

        for (int index : fileIndices) {
            std::string originalFilePath = directoryPath + "\\ATLYSS_Data\\profileCollections\\" + filePrefix + std::to_string(index);
            std::string backupFilePath = directoryPath + "\\ATLYSS_Data\\profileCollections\\" + filePrefix + std::to_string(index) + backupSuffix;

            if (fs::exists(originalFilePath)) {
                std::cout << "Found file: " << originalFilePath << std::endl;
                try {
                    if (fs::exists(backupFilePath)) {
                        fs::remove(backupFilePath);
                        std::cout << "Existing backup file removed: " << backupFilePath << std::endl;
                    }
                    fs::copy_file(originalFilePath, backupFilePath);
                    std::cout << "Backup created: " << backupFilePath << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Failed to create backup (" << backupFilePath << "): " << e.what() << std::endl;
                    allBackupsSuccessful = false;
                }
            } else {
                std::cout << "File not found, skipping: " << originalFilePath << std::endl;
            }
        }

        return allBackupsSuccessful;
    }
};

// Class to handle storage backup operations
class StorageBackup {
private:
    std::vector<std::pair<std::string, std::string>> backupFiles; // Pairs of original file paths and their backup paths

public:
    // Constructor
    StorageBackup(const std::vector<std::pair<std::string, std::string>>& files) : backupFiles(files) {}

    // Create backups
    bool createBackups() {
        bool allBackupsSuccessful = true;

        for (const auto& [originalPath, backupPath] : backupFiles) {
            try {
                if (fs::exists(originalPath)) {
                    if (fs::exists(backupPath)) {
                        fs::remove(backupPath);
                        std::cout << "Existing backup file removed: " << backupPath << std::endl;
                    }
                    fs::copy_file(originalPath, backupPath);
                    std::cout << "Backup created: " << backupPath << std::endl;
                } else {
                    std::cout << "File not found, skipping: " << originalPath << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Failed to create backup (" << backupPath << "): " << e.what() << std::endl;
                allBackupsSuccessful = false;
            }
        }

        return allBackupsSuccessful;
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
    // Auto-detect the current directory
    std::string currentDirectory = getCurrentDirectory();
    std::cout << "Current directory: " << currentDirectory << std::endl;

    // Directory and file name settings for character backup
    const std::string characterFilePrefix = "atl_characterProfile_";
    const std::string backupSuffixBefore = "_before";
    const std::string backupSuffixAfter = "_after";
    const std::vector<int> characterIndices = {0, 1, 2, 3, 4, 5, 6};

    // Storage backup file paths
    const std::vector<std::pair<std::string, std::string>> storageFilesBefore = {
        {currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank", currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_before"},
        {currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_01", currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_01_before"},
        {currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_02", currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_02_before"}
    };

    const std::vector<std::pair<std::string, std::string>> storageFilesAfter = {
        {currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank", currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_after"},
        {currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_01", currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_01_after"},
        {currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_02", currentDirectory + "\\ATLYSS_Data\\profileCollections\\atl_itemBank_02_after"}
    };

    const std::string executablePath = currentDirectory + "\\ATLYSS.exe";

    // Create objects
    CharacterBackup characterBackup(currentDirectory, characterFilePrefix, backupSuffixBefore, backupSuffixAfter, characterIndices);
    StorageBackup storageBackupBefore(storageFilesBefore);
    StorageBackup storageBackupAfter(storageFilesAfter);
    ATLYSSProgram atlyssProgram(executablePath);

    // Create "before" backups
    if (!characterBackup.createBeforeBackups() || !storageBackupBefore.createBackups()) {
        return 1;
    }

    // Start and monitor the ATLYSS program
    if (!atlyssProgram.startAndMonitor()) {
        return 1;
    }

    // Create "after" backups
    if (!characterBackup.createAfterBackups() || !storageBackupAfter.createBackups()) {
        return 1;
    }

    return 0;
}
