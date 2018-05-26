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

#include "../include/Client.h"
#include "../include/Console.h"

Client::Client(Bar *bar) : bar(bar) {
    std::thread(&Client::run, this).detach();
}

void Client::run() {
    Console::println("Client[", this, "] says: Hello!!");
    bar->waitRoundBarrier();

    // Start client logic
    while (bar->isOpen()) {
        // Check and manage client orders
        if (hasOrder()) {
            order();
            waitOrder();
            consumesOrder();
        } else {
            // Notify waiters
            bar->waitClientsBarrier();
        }

        Console::println("Client[", this, "] is waiting for the next round");
        bar->waitRoundBarrier();
    }

    Console::println("Client[", this, "] says: Goodbye!!");
}

client_state_t Client::getState() {
    return state;
}

bool Client::hasOrder() {
    // Check if the client will order at the current round
    unsigned int pedido = getRandomNumber() % 10;

    // Update client state and return the result
    if (pedido < 9) {
        state = ORDERING;
        return true;
    } else {
        state = IDLE;
        Console::println("Client[", this, "] wont order at round ", bar->getRound());
        return false;
    }
}

void Client::order() {
    Console::println("Client[", this, "] is ordering at round ", bar->getRound());
    bar->putOrder(this);
}

void Client::waitOrder() {
    // Update client state
    state = WAITING;
    Console::println("Client[", this, "] is waiting for his order");

    // Notify waiters
    bar->waitClientsBarrier();

    // Wait for his order
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this] { return state == CONSUMING; });
}

void Client::receiveOrder() {
    // Update client state
    state = CONSUMING;
    Console::println("Client[", this, "] is receiving his order");

    // Receives the order and wakes up the client
    cv.notify_all();
}

void Client::consumesOrder() {
    Console::println("Client[", this, "] is consuming his order");

    // Consumes order for a random time (up to 1 second)
    unsigned int time = getRandomNumber() % 1000000;
    usleep(time);

    // Atualiza estado atual do cliente
    state = IDLE;
}
