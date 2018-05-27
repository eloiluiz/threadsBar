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

#ifndef THREADS_BAR_BAR_H
#define THREADS_BAR_BAR_H

#include <vector>
#include <semaphore.h>
#include <iostream>
#include <deque>
#include "Waiter.h"
#include "Client.h"
#include "Barrier.h"

// Declaring dependencies
class Waiter;

class Client;

/**
 * @brief Threads Bar main class, responsible to manage the application threads and control its execution.
 */
class Bar {
public:
    Bar(unsigned int N_CLIENTS, unsigned int N_WAITERS, unsigned int CAP_WAITERS, unsigned int ROUNDS);

    void open();

    void run();

    void close();

    bool hasClients();

    bool isOpen();

    void waitRoundBarrier();

    void waitStageBarrier();

    int getRound();

    bool hasOrder();

    Client *getOrder();

    void putOrder(Client *client);

    virtual ~Bar();

private:
    void printStatistics();

    void clearAttributes();

private:
    // Bar definitions
    const unsigned int ROUNDS;
    const unsigned int N_CLIENTS;
    const unsigned int N_WAITERS;
    const unsigned int CAP_WAITERS;

    // Control objects
    Barrier *roundBarrier;
    Barrier *stageBarrier;
    unsigned int round{1};

    // Bar collections
    std::mutex orders_mutex;
    std::vector<Waiter *> waiters;
    std::vector<Client *> clients;
    std::deque<Client *> orders;
};

#endif // THREADS_BAR_BAR_H
