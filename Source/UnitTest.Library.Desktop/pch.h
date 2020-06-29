#pragma once

// Standard
#include <cstdint>
#include <string>
#include <crtdbg.h>

// Microsoft Native Unit Testing FrameWork
#include <CppUnitTest.h>

// size_t type literal
inline std::size_t operator "" _z(unsigned long long int x)
{
    return static_cast<size_t>(x);
}