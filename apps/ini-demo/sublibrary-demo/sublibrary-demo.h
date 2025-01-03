// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(_MSC_VER)
#if  defined(SUBLIBRARY_DEMO_EXPORTS) || \
     defined(sublibrary_demo_EXPORTS)
#define  SUBLIBRARY_DEMO_API __declspec(dllexport)
#else
#define  SUBLIBRARY_DEMO_API __declspec(dllimport)
#endif //  SUBLIBRARY_DEMO_EXPORTS
#endif // _MSC_VER

// TODO: GCC & Clang
#ifndef SUBLIBRARY_DEMO_API
#define  SUBLIBRARY_DEMO_API
#endif // !SUBLIBRARY_DEMO_API

extern "C" SUBLIBRARY_DEMO_API void sublibrary_demo_print_library_name();
