#!/bin/bash

# CONSTEXPR_BACKTRACE=-fconstexpr-backtrace-limit=0
TEMPLATE_DEPTH=-ftemplate-depth=640000

clang++ \
    ${CONSTEXPR_BACKTRACE} \
    ${TEMPLATE_DEPTH} \
    -std=c++14 \
    test.cpp \
    -o test \
    && chmod +x test && ./test
