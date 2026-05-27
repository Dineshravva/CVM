#pragma once

#include "Opcode.h"
#include <vector>
#include <cstdint>
#include <cstddef>

class Disassembler {
public:
    static void print(const std::vector<uint8_t>& bytecode);

private:
    static int64_t readInt64(
        const std::vector<uint8_t>& bytecode,
        size_t& ip
    );

    static int32_t readInt32(
        const std::vector<uint8_t>& bytecode,
        size_t& ip
    );
};