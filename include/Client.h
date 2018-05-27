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

#ifndef THREADS_BAR_CLIENT_H
#define THREADS_BAR_CLIENT_H

#include "Bar.h"
#include <mutex>
#include <random>
#include <thread>
#include <condition_variable>

// Declaring dependencies
class Bar;

// Client state definition
typedef enum clientState {
    ORDERING,
    WAITING,
    CONSUMING,
    IDLE,
} client_state_t;

class Client {
public:

    explicit Client(Bar *bar);

    void run();

    client_state_t getState();

    bool isOrdering();

    void order();

    void waitOrder();

    void receiveOrder();

    void consumesOrder();

private:
    // Bar reference
    Bar *bar;

    // Client state definition
    client_state_t state = ORDERING;

    // Orders consuming time
    std::random_device getRandomNumber;

    // Order control
    std::mutex m;
    std::condition_variable cv;
};


#endif // THREADS_BAR_CLIENT_H
