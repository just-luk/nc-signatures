// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
//
// Licensees holding a valid commercial license may use this project
// in accordance with the standard license agreement terms provided
// with the Software (see accompanying file LICENSE.rst or
// https://www.steinwurf.com/license), unless otherwise different
// terms and conditions are agreed in writing between Licensee and
// Steinwurf ApS in which case the license will be regulated by that
// separate written agreement.
//
// License for Non-Commercial Usage
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

// Detect compilers and CPU architectures
#if defined(__clang__) || defined(__llvm__)
#if defined(__i386__) || defined(__x86_64__)
#define FIFI_PLATFORM_X86 1
#elif defined(__arm__) || defined(__arm64__) || defined(__aarch64__)
#define FIFI_PLATFORM_ARM 1
#elif defined(__mips__)
#define FIFI_PLATFORM_MIPS 1
#elif defined(__asmjs__)
#define FIFI_PLATFORM_ASMJS 1
#endif
#elif defined(__GNUC__)
#if defined(__i386__) || defined(__x86_64__)
#define FIFI_PLATFORM_X86 1
#elif defined(__arm__) || defined(__arm64__) || defined(__aarch64__)
#define FIFI_PLATFORM_ARM 1
#elif defined(__mips__)
#define FIFI_PLATFORM_MIPS 1
#endif
#elif defined(_MSC_VER)
#if defined(_M_IX86) || defined(_M_X64)
#define FIFI_PLATFORM_X86 1
#elif defined(_M_ARM) || defined(_M_ARMT)
#define FIFI_PLATFORM_ARM 1
#endif
#else
#error "Unable to determine compiler"
#endif
