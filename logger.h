#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// Инициализация логгера
bool logger_init(const char *log_file_path);

// Освобождение ресурсов логгера
void logger_cleanup(void);

// Функция журналирования
void logger_log(LogLevel level, const char *file, int line, const char *func, const char *message, ...);

// Макросы для удобства использования
#define LOG_DEBUG(msg, ...) logger_log(LOG_DEBUG, __FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) logger_log(LOG_INFO, __FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) logger_log(LOG_WARNING, __FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) logger_log(LOG_ERROR, __FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)

#endif // LOGGER_H