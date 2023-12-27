#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>

#include <libreborn/libreborn.h>

#ifndef __arm__
#error "Patching Code Is ARM Only"
#endif

// BL Instruction Magic Number
#define BL_INSTRUCTION 0xeb
#define B_INSTRUCTION 0xea

// Generate A BL Instruction
static uint32_t generate_bl_instruction(void *from, void *to, int use_b_instruction) {
    if (abs(((int32_t) to) - ((int32_t) from)) > 32000000) {
        IMPOSSIBLE();
    }
    
    uint32_t instruction;
    unsigned char *instruction_array = (unsigned char *) &instruction;

    instruction_array[3] = use_b_instruction ? B_INSTRUCTION : BL_INSTRUCTION;

    unsigned char *pc = ((unsigned char *) from) + 8;
    int32_t offset = (int32_t) to - (int32_t) pc;
    int32_t target = offset >> 2;

    unsigned char *target_array = (unsigned char *) &target;
    instruction_array[0] = target_array[0];
    instruction_array[1] = target_array[1];
    instruction_array[2] = target_array[2];

    return instruction;
}

// Run For Every .text Section
static int _overwrite_calls_within_internal(const char *file, int line, void *from, void *to, void *target, void *replacement) {
    int found = 0;
    for (uint32_t i = (uint32_t) from; i < (uint32_t) to; i = i + 4) {
        unsigned char *addr = (unsigned char *) i;
        int use_b_instruction = addr[3] == B_INSTRUCTION;
        // Check If Instruction is B Or BL
        if (addr[3] == BL_INSTRUCTION || use_b_instruction) {
            uint32_t check_instruction = generate_bl_instruction(addr, target, use_b_instruction);
            unsigned char *check_instruction_array = (unsigned char *) &check_instruction;
            // Check If Instruction Calls Target
            if (addr[0] == check_instruction_array[0] && addr[1] == check_instruction_array[1] && addr[2] == check_instruction_array[2]) {
                // Patch Instruction
                uint32_t new_instruction = generate_bl_instruction(addr, replacement, use_b_instruction);
                _patch(file, line, addr, (unsigned char *) &new_instruction);
                found++;
            }
        }
    }
    return found;
}
struct overwrite_data {
    const char *file;
    int line;
    void *target;
    void *replacement;
    int found;
};
static void overwrite_calls_callback(ElfW(Addr) section_addr, ElfW(Word) size, void *data) {
    struct overwrite_data *args = (struct overwrite_data *) data;
    void *section = (void *) section_addr;
    args->found += _overwrite_calls_within_internal(args->file, args->line, section, (void *) (section_addr + size), args->target, args->replacement);
}

// Limit To 512 overwrite_calls() Uses
#define CODE_BLOCK_SIZE 4096
static unsigned char *code_block = NULL;
#define CODE_SIZE 8
static int code_block_remaining = CODE_BLOCK_SIZE;

static void _long_overwrite(void *start, void *target) {
    unsigned char patch_data[4] = {0x04, 0xf0, 0x1f, 0xe5}; // "ldr pc, [pc, #-0x4]"

    _patch(NULL, -1, start, patch_data);
    _patch_address(NULL, -1, (void *) (((unsigned char *) start) + 4), target);
}
static void update_code_block(void *target) {
    // BL Instructions Can Only Access A Limited Portion of Memory, So This Allocates Memory Closer To The Original Instruction, That When Run, Will Jump Into The Actual Target
    if (code_block == NULL) {
        code_block = mmap((void *) 0x200000, CODE_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (code_block == MAP_FAILED) {
            ERR("Unable To Allocate Code Block: %s", strerror(errno));
        }
        DEBUG("Code Block Allocated At: 0x%08x", (uint32_t) code_block);
    }
    if (code_block_remaining < CODE_SIZE) {
        ERR("Maximum Amount Of overwrite_calls() Uses Reached");
    }
    _long_overwrite(code_block, target);
}
static void increment_code_block() {
    code_block = code_block + CODE_SIZE;
    code_block_remaining = code_block_remaining - CODE_SIZE;
}

// Overwrite Specific B(L) Instruction
static void _overwrite_call_internal(const char *file, int line, void *start, void *target, int use_b_instruction) {
    // Add New Target To Code Block
    update_code_block(target);

    // Patch
    uint32_t new_instruction = generate_bl_instruction(start, code_block, use_b_instruction);
    _patch(file, line, start, (unsigned char *) &new_instruction);

    // Increment Code Block Position
    increment_code_block();
}
void _overwrite_call(const char *file, int line, void *start, void *target) {
    int use_b_instruction = ((unsigned char *) start)[3] == B_INSTRUCTION;
    _overwrite_call_internal(file, line, start, target, use_b_instruction);
}

// Overwrite All B(L) Intrusctions That Target The Specified Address
#define NO_CALLSITE_ERROR "(%s:%i) Unable To Find Callsites For 0x%08x"
void _overwrite_calls(const char *file, int line, void *start, void *target) {
    // Add New Target To Code Block
    update_code_block(target);

    struct overwrite_data data;
    data.file = file;
    data.line = line;
    data.target = start;
    data.replacement = code_block;
    data.found = 0;

    iterate_segments(overwrite_calls_callback, &data);

    // Increment Code Block Position
    increment_code_block();

    // Check
    if (data.found < 1) {
        ERR(NO_CALLSITE_ERROR, file, line, (uint32_t) start);
    }
}
void _overwrite_calls_within(const char *file, int line, void *from /* inclusive */, void *to /* exclusive */, void *target, void *replacement) {
    // Add New Target To Code Block
    update_code_block(replacement);

    // Patch
    int found = _overwrite_calls_within_internal(file, line, from, to, target, code_block);
    // Check
    if (found < 1) {
        ERR(NO_CALLSITE_ERROR, file, line, (uint32_t) target);
    }

    // Increment Code Block Position
    increment_code_block();
}

// Extract Target Address From B(L) Instruction
void *extract_from_bl_instruction(unsigned char *from) {
    unsigned char *pc = ((unsigned char *) from) + 8;

    int32_t target = *(int32_t *) from;
    target = (target << 8) >> 8;

    int32_t offset = target << 2;

    return (void *) (pc + offset);
}

// Overwrite Function
void _overwrite(const char *file, int line, void *start, void *target) {
    _overwrite_call_internal(file, line, start, target, 1);
}

// Print Patch Debug Data
#define PATCH_PRINTF(file, line, start, str) if (file != NULL) DEBUG("(%s:%i): Patching (0x%08x) - " str ": 0x%02x 0x%02x 0x%02x 0x%02x", file, line, (uint32_t) start, data[0], data[1], data[2], data[3]);

// Patch Instruction
void _patch(const char *file, int line, void *start, unsigned char patch[4]) {
    if (((uint32_t) start) % 4 != 0) {
        ERR("Invalid Address");
    }

    size_t page_size = sysconf(_SC_PAGESIZE);
    uintptr_t end = ((uintptr_t) start) + 4;
    uintptr_t page_start = ((uintptr_t) start) & -page_size;
    // Allow Writing To Code Memory
    mprotect((void *) page_start, end - page_start, PROT_READ | PROT_WRITE | PROT_EXEC); // PROT_EXEC Is Needed Because Other Code In The Page May Be Being Executed

    unsigned char *data = (unsigned char *) start;

    PATCH_PRINTF(file, line, start, "original");

    memcpy(data, patch, 4);

    PATCH_PRINTF(file, line, start, "result");

    // Reset Code Memory Permissions
    mprotect((void *) page_start, end - page_start, PROT_READ | PROT_EXEC);

    // Clear ARM Instruction Cache
    __clear_cache(start, (void *) end);
}

// Patch Address
void _patch_address(const char *file, int line, void *start, void *target) {
    uint32_t addr = (uint32_t) target;
    unsigned char *patch_data = (unsigned char *) &addr;
    _patch(file, line, start, patch_data);
}
