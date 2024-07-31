# Укажите имя проекта
project(easy_plot_cpp)

# Укажите минимальную требуемую версию CMake
cmake_minimum_required(VERSION 3.2)

# Создайте интерфейсную библиотеку
add_library(easy_plot_cpp_lib INTERFACE)

# Добавьте директорию с заголовочными файлами
target_include_directories(easy_plot_cpp_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
