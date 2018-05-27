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

#ifndef THREADS_BAR_BARRIER_H
#define THREADS_BAR_BARRIER_H

#include <mutex>
#include <condition_variable>

/**
 * @brief Barrier class designed to provide a simple thread synchronisation mechanism used to cause a set of threads to
 * wait until they each perform a certain function or each reach a particular point in their execution.
 */
class Barrier {
public:
    explicit Barrier(unsigned int count) : count(count), total(count) {}

    void Wait() {
        std::unique_lock<std::mutex> lock(m);
        if (--count == 0) {
            generation++;
            count = total;
            cv.notify_all();
        } else {
            auto localGeneration = generation;
            cv.wait(lock, [this, localGeneration] { return localGeneration != generation; });
        }
    }

private:
    // Defines the total number of threads synchronize
    const unsigned int total;
    // Counts the number of threads to wait at current generation
    unsigned int count;
    // Allows the reuse of an Barrier object, differentiating each point of synchronization as generations
    unsigned int generation{0};
    // Control objects
    std::mutex m;
    std::condition_variable cv;
};

#endif //THREADS_BAR_BARRIER_H
