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

#include "../include/Bar.h"
#include "../include/Console.h"
#include <algorithm>

Bar::Bar(int N_CLIENTS, int N_WAITERS, int CAP_WAITERS, int ROUNDS) : N_CLIENTS(N_CLIENTS),
                                                                      N_WAITERS(N_WAITERS),
                                                                      CAP_WAITERS(CAP_WAITERS),
                                                                      ROUNDS(ROUNDS) {
    roundBarrier = new Barrier(N_CLIENTS + N_WAITERS + 1);
    clientsBarrier = new Barrier(N_WAITERS + N_CLIENTS + 1);
    waitersBarrier = new Barrier(N_WAITERS + 1);
}

Bar::~Bar() {
    delete roundBarrier;
    delete clientsBarrier;
}

void Bar::open() {
    if (hasClients()) {
        Console::println("\n\nThe Threads Bar is opening!");

        // Create Client instances
        for (int i = 0; i < N_CLIENTS; i++) {
            clients.push_back(new Client(this));
        }

        // Create Waiter instances
        for (int i = 0; i < N_WAITERS; i++) {
            waiters.push_back(new Waiter(this, N_WAITERS));
        }

        // Wait for threads to start
        sleep(1);
    } else {
        Console::println("\n\nThe Threads Bar won't open! There are no clients today.");
    }
}

void Bar::run() {
    while (isOpen()) {
        Console::println("\n\nStarting round ", round, "!");
        // Wait for current round to finish
        roundBarrier->Wait();

        // Wait for clients to order
        clientsBarrier->Wait();

        // Wait for waiters to serve
        waitersBarrier->Wait();

        // Update round parameters
        round++;
    }

    // Finish rounds
    Console::println("\n\nEnd of last round!");
    roundBarrier->Wait();

    // Wait for threads to finish
    sleep(1);
}

void Bar::close() {
    Console::println("\n\nThe Threads Bar is closing!");

    // Print statistics
    printStatistics();

    // Clear current Bar objects and attributes
    clearAttributes();
}

bool Bar::hasClients() {
    return N_CLIENTS > 0;
}

void Bar::waitRoundBarrier() {
    roundBarrier->Wait();
}

void Bar::waitClientsBarrier() {
    clientsBarrier->Wait();
}

void Bar::waitWaitersBarrier() {
    waitersBarrier->Wait();
}

bool Bar::isOpen() {
    return round <= ROUNDS;
}

int Bar::getRound() {
    return round;
}

bool Bar::hasOrder() {
    return !orders.empty();
}

Client *Bar::getOrder() {
    Client *client = nullptr;
    std::unique_lock<std::mutex> lock(orders_mutex);
    if (!orders.empty()) {
        client = orders.front();
        orders.pop_front();
    }
    return client;
}

void Bar::putOrder(Client *client) {
    std::unique_lock<std::mutex> lock(orders_mutex);
    orders.push_back(client);
}

void Bar::printStatistics() {

}

void Bar::clearAttributes() {
    // Clear remaining orders
    orders.clear();

    // Destroy and Clear clients
    for_each(clients.begin(), clients.end(), [](Client *client) { delete client; });
    clients.clear();

    // Destroy and Clear waiters
    for_each(waiters.begin(), waiters.end(), [](Waiter *waiter) { delete waiter; });
    waiters.clear();
}
