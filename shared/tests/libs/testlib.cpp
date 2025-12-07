/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testlib
*/
#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif
extern "C"
{
    EXPORT int get_number()
    {
        return 42;
    }
}
