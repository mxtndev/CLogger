#include "logger.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <execinfo.h> // Для backtrace
#include <unistd.h>   // Для getpid

static FILE *log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Инициализация логгера
bool logger_init(const char *log_file_path) {
    pthread_mutex_lock(&log_mutex);

    if (log_file != NULL) {
        pthread_mutex_unlock(&log_mutex);
        return false; // Логгер уже инициализирован
    }

    log_file = fopen(log_file_path, "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        pthread_mutex_unlock(&log_mutex);
        return false;
    }

    // Записываем информацию о начале логирования
    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
    fprintf(log_file, "=== Logging started at %s ===\n", timestamp);
    fflush(log_file);

    pthread_mutex_unlock(&log_mutex);
    return true;
}

// Освобождение ресурсов логгера
void logger_cleanup(void) {
    pthread_mutex_lock(&log_mutex);

    if (log_file != NULL) {
        time_t now = time(NULL);
        struct tm *time_info = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
        fprintf(log_file, "=== Logging ended at %s ===\n", timestamp);
        fclose(log_file);
        log_file = NULL;
    }

    pthread_mutex_unlock(&log_mutex);
}

// Функция журналирования
void logger_log(LogLevel level, const char *file, int line, const char *func, const char *message, ...) {
    pthread_mutex_lock(&log_mutex);

    if (log_file == NULL) {
        time_t now = time(NULL);
        struct tm *time_info = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
        fprintf(stderr, "[%s] Logger is not initialized.\n", timestamp);
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    // Получаем текущее время
    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    // Определяем уровень логирования
    const char *level_str;
    switch (level) {
        case LOG_DEBUG:   level_str = "DEBUG"; break;
        case LOG_INFO:    level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:   level_str = "ERROR"; break;
        default:          level_str = "UNKNOWN"; break;
    }

    // Форматируем сообщение
    va_list args;
    va_start(args, message);
    fprintf(log_file, "[%s] [%s] %s:%d (%s): ", timestamp, level_str, file, line, func);
    vfprintf(log_file, message, args);
    fprintf(log_file, "\n");
    va_end(args);

    // Для уровня ERROR выводим стек вызовов
    if (level == LOG_ERROR) {
        void *buffer[10];
        int size = backtrace(buffer, 10);
        char **symbols = backtrace_symbols(buffer, size);

        if (symbols != NULL) {
            fprintf(log_file, "Stack trace:\n");
            for (int i = 0; i < size; ++i) {
                fprintf(log_file, "  %s\n", symbols[i]);
            }
            free(symbols);
        } else {
            fprintf(log_file, "Failed to generate stack trace.\n");
        }
    }

    fflush(log_file);
    pthread_mutex_unlock(&log_mutex);
}