#pragma once

class LogManager {

    public :

        static LogManager& GetSingleton() {

            static LogManager instance;

            return instance;

        };

        enum class LogLevel {

            Critical,
            Debug,
            Error,
            Info,
            Trace,
            Warn

        };

        bool Init();

        void SendCritical(const char message);
        void SendDebug(const char message);
        void SendError(const char message);
        void SendInformation(const char message);
        void SendTrace(const char message);
        void SendWarning(const char message);

    private :

        bool CreateLogFile();
        bool SetLogLevel(LogLevel level);

};