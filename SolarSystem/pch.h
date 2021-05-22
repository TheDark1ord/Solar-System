#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>

/// Defined literals

// Time
long double operator"" s(long double val);
long double operator"" s(unsigned long long val);

long double operator"" min(long double val);
long double operator"" min(unsigned long long val);

long double operator"" h(long double val);
long double operator"" h(unsigned long long val);

long double operator"" d(long double val);
long double operator"" d(unsigned long long val);

long double operator"" y(long double val);
long double operator"" y(unsigned long long val);

// Distance
long double operator"" m(long double val);
long double operator"" m(unsigned long long val);

long double operator"" km(long double val);
long double operator"" km(unsigned long long val);

long double operator"" ls(long double val); // Light second
long double operator"" ls(unsigned long long val);

long double operator"" lm(long double val); // Light minute
long double operator"" lm(unsigned long long val);

long double operator"" ly(long double val); // Light year
long double operator"" ly(unsigned long long val);

long double operator"" au(long double val); // Astronomical unit
long double operator"" au(unsigned long long val);

long double operator"" pc(long double val); // Parsec
long double operator"" pc(unsigned long long val);

long double operator"" er(long double val); // Earth radius
long double operator"" er(unsigned long long val);

// Mass


// General settings
#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

inline const long double G = 6.67e-11;
inline const unsigned int Width = 1280, Height = 720;
inline const unsigned int antialiasingLevel = 8;
inline const unsigned int maxFPS = 75;
inline const char* WindowHeader = "Window Header";

inline const long double default_zoom = 1280 / 3au;
inline int current_time_frame = 4;
inline const int time_frame_array[]{ 1s, 1m, 1h, 12h, 1d, 5d, 10d, 20d };


#endif