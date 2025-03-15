#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Для sleep

// Функция для тестирования логирования из разных функций
void test_function() {
    LOG_INFO("This is an info message from test_function.");
    LOG_ERROR("This is an error message with a stack trace.");
}

// Функция, выполняемая в потоках
void *thread_function(void *arg) {
    long thread_id = (long)arg;
    LOG_INFO("Message from thread %ld", thread_id);
    sleep(1); // Имитация работы потока
    return NULL;
}

int main() {
    // Проверка инициализации логгера
    if (!logger_init("app.log")) {
        fprintf(stderr, "Failed to initialize logger.\n");
        return EXIT_FAILURE;
    }

    // Логирование из main
    LOG_DEBUG("Debug message in main.");
    LOG_INFO("Info message in main.");
    LOG_WARNING("Warning message in main.");

    // Логирование из другой функции
    test_function();

    // Тестирование многопоточности
    pthread_t threads[5];
    for (long i = 0; i < 5; ++i) {
        if (pthread_create(&threads[i], NULL, thread_function, (void *)i) != 0) {
            LOG_ERROR("Failed to create thread %ld", i);
        }
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < 5; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            LOG_ERROR("Failed to join thread %d", i);
        }
    }

    // Проверка граничного случая: попытка записи в недоступный файл
    LOG_INFO("Attempting to log after cleanup...");
    logger_cleanup(); // Освобождение ресурсов
    LOG_ERROR("This message should not appear in the log file.");

    return EXIT_SUCCESS;
}