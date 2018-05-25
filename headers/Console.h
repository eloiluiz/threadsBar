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

class Console {
private:
    Console() {}

    inline static void innerPrint(std::ostream &stream) {}

    template<typename Head, typename... Tail>
    inline static void innerPrint(std::ostream &stream, Head const head, Tail const ...tail) {
        stream << head;
        innerPrint(stream, tail...);
    }

    inline static void innerPrintln(std::ostream &stream) {
        stream << "\n";
    }

    template<typename Head, typename... Tail>
    inline static void innerPrintln(std::ostream &stream, Head const head, Tail const ...tail) {
        stream << head;
        innerPrintln(stream, tail...);
    }

public:
    template<typename Head, typename... Tail>
    inline static void print(Head const head, Tail const ...tail) {
        static std::mutex mutex;
        // Create a stream buffer
        std::stringbuf buffer;
        std::ostream stream(&buffer);
        // Feed input parameters to stream
        innerPrint(stream, head, tail...);
        // Enter critical region
        mutex.lock();
        // Print into console an flush
        std::cout << buffer.str() << std::flush;
        // Exit critical region
        mutex.unlock();
    }

    template<typename Head, typename... Tail>
    inline static void println(Head const head, Tail const ...tail) {
        static std::mutex mutex;
        // Create a stream buffer
        std::stringbuf buffer;
        std::ostream stream(&buffer);
        // Feed input parameters to stream
        innerPrintln(stream, head, tail...);
        // Enter critical region
        mutex.lock();
        // Print into console an flush
        std::cout << buffer.str() << std::flush;
        // Exit critical region
        mutex.unlock();
    }
};

#endif //THREADS_BAR_CONSOLE_H
