/*
 * This file is part of the threadsBar distribution (https://github.com/eloiluiz/threadsBar).
 * Copyright (c) 2018 Eloi Luiz Giacobbo Filho.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef THREADS_BAR_CONSOLE_H
#define THREADS_BAR_CONSOLE_H

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <mutex>

/**
 * @brief Console utility class designed to compose string messages before calling the standard output stream and avoid
 * concurrency problems.
 */
class Console {
private:
    Console() = default;

    inline static void innerPrint(std::ostream &stream) {}

    template<typename Head, typename... Tail>
    inline static void innerPrint(std::ostream &stream, Head const head, Tail const ...tail) {
        stream << head;
        innerPrint(stream, tail...);
    }

public:
    template<typename Head, typename... Tail>
    inline static void print(Head const head, Tail const ...tail) {
        // Create a stream buffer
        std::stringbuf buffer;
        std::ostream stream(&buffer);
        // Feed input parameters to the stream object
        innerPrint(stream, head, tail...);
        // Print into console and flush
        std::cout << buffer.str();
    }

    template<typename Head, typename... Tail>
    inline static void println(Head const head, Tail const ...tail) {
        print(head, tail..., "\n");
    }
};

#endif //THREADS_BAR_CONSOLE_H
