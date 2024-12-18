#pragma once

class LogManager {

    public :

        enum class LogLevel {

            Critical,
            Debug,
            Error,
            Info,
            Trace,
            Warn

        };

        static LogManager& GetSingleton() {

            static LogManager instance;
            return instance;

        };

        bool Init();

        static void Log(LogLevel level, const std::string& message, bool shouldBreakLine);
        static void BreakLine();
        static void SetLogLevel();

    private :

        bool CreateLogFile();

};