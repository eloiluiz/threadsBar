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

    // Start Client's logic
    while (bar->isOpen()) {

        // Check and manage client orders
        if (isOrdering()) {
            order();
            waitOrder();
            consumesOrder();
        } else {
            // Notify end of ordering stage
            bar->waitStageBarrier();
        }

        Console::println("Client[", this, "] is waiting for the next round");

        // Notify end of consuming stage
        bar->waitStageBarrier();

        // Actually wait for the start of next round
        bar->waitRoundBarrier();
    }

    Console::println("Client[", this, "] says: Goodbye!!");
}

client_state_t Client::getState() {
    return state;
}

bool Client::isOrdering() {
    // Check if the client will order at current round
    unsigned int willOrder = getRandomNumber() % 10;

    // Update client state and return the result
    if (willOrder < 9) {
        state = ORDERING;
        return true;
    } else {
        state = IDLE;
        Console::println("Client[", this, "] won't order at round ", bar->getRound());
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

    // Notify end of ordering stage
    bar->waitStageBarrier();

    // Wait for his order
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this] { return state == CONSUMING; });
}

void Client::receiveOrder() {
    // Update client state
    state = CONSUMING;
    Console::println("Client[", this, "] is receiving his order");

    // Receive the order and wakes up current client
    cv.notify_all();
}

void Client::consumesOrder() {
    Console::println("Client[", this, "] is consuming his order");

    // Consume order for a random time (up to 1 second)
    unsigned int time = getRandomNumber() % 1000000;
    usleep(time);

    // Update client state
    state = IDLE;
}
