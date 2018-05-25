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

#ifndef THREADS_BAR_WAITER_H
#define THREADS_BAR_WAITER_H

#include "Bar.h"
#include <mutex>
#include <random>
#include <thread>
#include <condition_variable>

// Declaring dependencies
class Bar;

class Client;

class Waiter {
public:
    Waiter(Bar *bar, int CAP_WAITER);

    void run();

    void receiveOrder();

    void prepareOrder();

    void deliverOrder();

private:
    Bar *bar;
    const int CAP_WAITER;

    // Orders queue
    std::deque<Client *> orders;

    // Orders preparing time
    std::random_device getRandomNumber;
};

#endif // THREADS_BAR_WAITER_H
