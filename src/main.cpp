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

#include "../include/Console.h"
#include "../include/Bar.h"

int main(int argc, const char* argv[]) {
    Console::println("Starting Threads Bar Application");

    // Initialize random application parameters
    std::random_device getRandomNumber;
    auto nClients = getRandomNumber() % 100;
    auto nWaiters = getRandomNumber() % 100 + 1;
    auto waitersCap = getRandomNumber() % 100;
    auto nRounds = getRandomNumber() % 5 + 1;

    // Start application
    Bar bar(nClients, nWaiters, waitersCap, nRounds);
    bar.open();
    bar.run();
    bar.close();

    return 0;
}