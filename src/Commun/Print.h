#ifndef _PRINT_H
#define _PRINT_H

#define PRINT_primaire true
#define PRINT_secondaire true
#define PRINT_test true
#define PRINT_error true
#define PRINT_status true

// Couleurs ANSI
#define COLOR_GREEN   "\033[32m"
#define COLOR_ORANGE  "\033[38;5;208m"  // orange 
#define COLOR_PINK    "\033[35m"
#define COLOR_RED     "\033[31m"
#define COLOR_STATUS "\033[36m"   // cyan
#define COLOR_RESET   "\033[0m"


//fonction de base

template<typename... Args>
inline void print_primaire(const Args&... args) {
    if constexpr (PRINT_primaire) {
        std::cout << COLOR_GREEN;
        (std::cout << ... << args);
        std::cout << COLOR_RESET << std::endl;
    }
}

template<typename... Args>
inline void print_secondaire(const Args&... args) {
    if constexpr (PRINT_secondaire) {
        std::cout << COLOR_ORANGE;
        (std::cout << ... << args);
        std::cout << COLOR_RESET << std::endl;
    }
}

template<typename... Args>
inline void print_secondaire_attention(const Args&... args) {
    if constexpr (PRINT_secondaire) {
        std::cout << COLOR_PINK;
        (std::cout << ... << args);
        std::cout << COLOR_RESET << std::endl;
    }
}

template<typename... Args>
inline void print_error(const Args&... args) {
    if constexpr (PRINT_error) {
        std::cout << COLOR_RED<<"Erreur:";
        (std::cout << ... << args);
        std::cout << COLOR_RESET << std::endl;
    }
}

template<typename... Args>
inline void print_status(bool in_progress, const Args&... args) {
    if constexpr (!PRINT_status) return;
    std::cout << COLOR_STATUS;
    (std::cout << ... << args);
    std::cout << (in_progress ? " ..." : " Done!") << COLOR_RESET << std::endl;
}

// le par default non controlable
template<typename... Args>
inline void print(const Args&... args) {
    (std::cout << ... << args);
    std::cout<<std::endl;
}

#endif