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

class Barrier {
public:
    explicit Barrier(int count) : count(count), total(count) {}

    void Wait() {
        std::unique_lock<std::mutex> lock(m);
        if (--count == 0) {
            count = total;
            cv.notify_all();
        } else {
            cv.wait(lock, [this] { return count == total; });
        }
    }


private:
    int count;
    int total;
    std::mutex m;
    std::condition_variable cv;
};

#endif //THREADS_BAR_BARRIER_H
