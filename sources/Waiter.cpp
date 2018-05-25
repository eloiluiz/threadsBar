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

#include "../headers/Waiter.h"
#include "../headers/Console.h"

Waiter::Waiter(Bar *bar, const int CAP_WAITER) : bar(bar), CAP_WAITER(CAP_WAITER) {
    std::thread(&Waiter::run, this).detach();
}

void Waiter::run() {
    Console::println("Waiter[", this, "] says: Hello!!");
    bar->waitRoundBarrier();
    while (bar->isOpen()) {
        // Wait for client's orders
        bar->waitClientsBarrier();

        // Work to deliver every order
        while (bar->hasOrder()) {
            // Receive orders from Bar
            receiveOrder();

            // Check if there are orders to deliver
            if (!orders.empty()) {
                // Prepare orders
                prepareOrder();
                // Deliver orders
                deliverOrder();
            }
        }

        Console::println("Waiter[", this, "] is waiting for the next round");
        bar->waitWaitersBarrier();
        bar->waitRoundBarrier();
    }

    Console::println("Waiter[", this, "] says: Goodbye!!");
}

void Waiter::receiveOrder() {

    // Receive orders while available and while the waiter capacity is not reached
    while (bar->hasOrder() && orders.size() < CAP_WAITER) {

        // Get order from bar
        Client *client = bar->getOrder();

        // Check if the waiter got any actual order
        if (client != nullptr) {
            // Store the client's order
            orders.push_back(client);
            Console::println("Waiter[", this, "] answers Client[", client, "]");

            // Give way to another waiter
            std::this_thread::yield();
        }
    }
}

void Waiter::prepareOrder() {

    // Prepares each order
    for (auto order : orders) {
        Console::println("Waiter[", this, "] is preparing order from Client[", order, "]");

        // Takes up to 1 second to prepare each order
        unsigned int time = getRandomNumber() % 1000000;
        usleep(time);
    }
}

void Waiter::deliverOrder() {

    // Deliver each order
    while (!orders.empty()) {

        // Get the next order to be delivered
        Client *client = orders.front();
        orders.pop_front();

        // Entrega pedido do cliente
        client->receiveOrder();
        Console::println("Waiter[", this, "] delivered an order to Client[", client, "]");
    }
}
