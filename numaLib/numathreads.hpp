#pragma once
#ifndef NUMATHREADS_HPP
#define NUMATHREADS_HPP

#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include <numa.h>
#include <thread>

template <int NodeID>
class thread_numa : public std::thread {
public:
    using thread_type = std::thread;

    // Constructor that takes a function and its arguments
    template <typename Func, typename... Args>
    thread_numa(Func&& func, Args&&... args) {
        // Pin the current thread to the specified NUMA node
        pin_thread_to_node(NodeID);

        // Create a thread with the provided function and arguments
        thread_ = std::thread(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    // Destructor: join the thread if it is joinable
    ~thread_numa() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    // Delete copy constructor and copy assignment operator
    thread_numa(const thread_numa&) = delete;
    thread_numa& operator=(const thread_numa&) = delete;

    // Move constructor and assignment operator
    thread_numa(thread_numa&& other) noexcept : thread_(std::move(other.thread_)) {}
    thread_numa& operator=(thread_numa&& other) noexcept {
        if (this != &other) {
            if (thread_.joinable()) {
                thread_.join();
            }
            thread_ = std::move(other.thread_);
        }
        return *this;
    }

    void join() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    thread_type thread_;

    
    void pin_thread_to_node(int Node_num) {
        if (numa_available() == -1) {
            std::cerr << "NUMA is not available on this system." << std::endl;
            return;
        }
        numa_run_on_node(Node_num);  // Pin thread to NUMA node
    }
};


#endif

