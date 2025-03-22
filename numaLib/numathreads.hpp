#pragma once
#ifndef NUMATHREADS_HPP
#define NUMATHREADS_HPP

#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include <numa.h>
#include <thread>
#include <map>

#define MAX_CPUS 80

template <int NodeID>
class thread_numa : public std::thread {
public:
    using thread_type = std::thread;

    // Constructor that takes a function and its arguments
    template <typename Func, typename... Args>
    thread_numa(Func&& func, Args&&... args): std::thread(std::forward<Func>(func), std::forward<Args>(args)...) {
        // Pin the current thread to the specified NUMA node
        

        // Create a thread with the provided function and arguments
       // thread_ = std::thread(std::forward<Func>(func), std::forward<Args>(args)...);
        pin_thread_to_node(this, NodeID);
    }

    // Destructor: join the thread if it is joinable
    ~thread_numa() = default;

    // Delete copy constructor and copy assignment operator
    thread_numa(const thread_numa&) = delete;
    thread_numa& operator=(const thread_numa&) = delete;

    // Move constructor and assignment operator
    thread_numa(thread_numa&& other) noexcept :  std::thread(other) {}
    thread_numa& operator=(thread_numa&& other) noexcept {
        if (this != &other) {
            std::thread::operator=(std::move(other));  // Call base class move assignment
        }
        return *this;
    }

    // void join() {
    //     if (this->joinable()) {
    //         this->join();
    //     }
    // }

private:
    //thread_type thread_;
    static inline std::map<int, cpu_set_t*> node_to_cpumask;

    static decltype(node_to_cpumask)::iterator convert_bitmask_to_cpuset(int Node_num){
        if (numa_available() == -1) {
            throw std::runtime_error( "NUMA is not available on this system.");
        }
        struct bitmask* mask= numa_allocate_cpumask();
        int result = numa_node_to_cpus(Node_num, mask);
        if (result != 0 ){
            throw std::runtime_error("Getting bitmask failed");
        }
        cpu_set_t* cpuset = CPU_ALLOC(MAX_CPUS);
        CPU_ZERO(cpuset);
        for(int i =0 ; i < MAX_CPUS; ++i){
            if(numa_bitmask_isbitset(mask,(unsigned)i)){
                CPU_SET(i, cpuset);
             // std::cout<<Node_num<<"cpu"<<i<<std::endl;
            }
        }
        auto it = node_to_cpumask.insert({Node_num, cpuset});
        assert(it.second);
        return it.first;
    }
    
    void pin_thread_to_node(std::thread* tid, int Node_num) {
        if (numa_available() == -1) {
            std::cerr << "NUMA is not available on this system." << std::endl;
            return;
        }

        auto it = node_to_cpumask.find(Node_num);
        if(it == node_to_cpumask.end()){
           it = convert_bitmask_to_cpuset(Node_num);
        }
        pthread_t pid= (pthread_t) tid->native_handle();
        // printf("it->second %p\n", it->second);
        if(pthread_setaffinity_np(pid, CPU_ALLOC_SIZE(MAX_CPUS), it->second)){
            throw std::runtime_error("could not bind thread");
        }
        //numa_run_on_node(Node_num);  // Pin thread to NUMA node
    }
};


#endif

