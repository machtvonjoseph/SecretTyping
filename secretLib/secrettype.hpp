#pragma once
#ifndef SECRETTYPE_HPP
#define SECRETTYPE_HPP


#include <utility>
#include <type_traits>

#include <numaif.h> //set_mempolicy
#include <unistd.h> //getpagesize
#include <numa.h>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cassert>


template <typename T>
class secret:public T{
    public:
        secret() {

        }
};


#endif