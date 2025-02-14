#include "Logger.h"

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file." << std::endl;
    }
}

Logger::~Logger() {
    logFile.close();
}

void Logger::log(LogLevel level, const std::string& message) {
    // Get current timestamp
    time_t now = time(0);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string timestamp(buffer);

    // Create log entry
    std::ostringstream logEntry;
    logEntry << "[" << timestamp << "] " << levelTostring(level) << ": ";
    
    // Save the log to the vector
    Log new_log{level, logEntry.str(), message};
    logs.push_back(new_log);

    logEntry << message << std::endl;
    // Output to console
    std::cout << logEntry.str();

    // Output to log file
    if (logFile.is_open()) {
        logFile << logEntry.str();
        logFile.flush(); // Ensure immediate write to file
    }
}

Logger& Logger::operator<<(const std::string& message) {
    log(LOG_INFO, message);
    return *this;
}

void Logger::buildGUI() {
    ImGui::BeginChild("Log", ImVec2(0, 0), true);
    for(auto log : logs){
        ImGui::Text("%s %s", log.appendix.c_str(),  log.message.c_str());
    }
    ImGui::EndChild();
}

Logger& Logger::getInstance() {
    static Logger instance(generateLogFilename());
    return instance;
}

std::string Logger::levelTostring(LogLevel level) {
    switch (level) {
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO";
    case LOG_WARNING:
        return "WARNING";
    case LOG_ERROR:
        return "ERROR";
    case LOG_CRITICAL:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

std::string generateLogFilename() {
    time_t now = time(0);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "log_%Y%m%d_%H%M%S.txt", &timeinfo);
    return std::string(buffer);
}