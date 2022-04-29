#include <string>
#include <unordered_map>
#include <functional>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

#define WARN(format, ...) { fprintf(stderr, "[WARN]: " format "\n", __VA_ARGS__); }
#define ERR(format, ...) { fprintf(stderr, "[ERR]: (%s:%i): " format "\n", __FILE__, __LINE__, __VA_ARGS__); exit(EXIT_FAILURE); }

// Load Symbol From ELF File
static void load_symbol(const char *source, std::function<void(std::string, unsigned char *, uint32_t)> callback) {
    // File Data
    FILE *file_obj = NULL;
    unsigned char *file_map = NULL;
    long int file_size = 0;

    // Code
    {
        // Load Main Binary
        file_obj = fopen(source, "rb");

        // Verify Binary
        if (!file_obj) {
            WARN("Unable To Open: %s", source);
            goto end;
        }

        // Get File Size
        fseek(file_obj, 0L, SEEK_END);
        file_size = ftell(file_obj);
        fseek(file_obj, 0L, SEEK_SET);

        // Map File To Pointer
        file_map = (unsigned char *) mmap(0, file_size, PROT_READ, MAP_PRIVATE, fileno(file_obj), 0);

        // Check ELF Magic
        if (file_map[EI_MAG0] != ELFMAG0 || file_map[EI_MAG1] != ELFMAG1 || file_map[EI_MAG2] != ELFMAG2 || file_map[EI_MAG3] != ELFMAG3) {
            WARN("Not An ELF File: %s", source);
            goto end;
        }
        if (file_map[EI_CLASS] != ELFCLASS32) {
            WARN("ELF File Isn't 32-Bit: %s", source);
            goto end;
        }
        if (file_map[EI_DATA] != ELFDATA2LSB) {
            WARN("ELF File Isn't Little-Endian: %s", source);
            goto end;
        }

        // Parse ELF
        Elf32_Ehdr *elf_header = (Elf32_Ehdr *) file_map;
        Elf32_Shdr *elf_section_headers = (Elf32_Shdr *) (file_map + elf_header->e_shoff);
        int elf_section_header_count = elf_header->e_shnum;

        // Locate Section Names
        Elf32_Shdr elf_shstrtab = elf_section_headers[elf_header->e_shstrndx];
        unsigned char *elf_shstrtab_p = file_map + elf_shstrtab.sh_offset;

        // Locate String Table
        unsigned char *elf_strtab_p = NULL;
        for (int i = 0; i < elf_section_header_count; ++i) {
            Elf32_Shdr header = elf_section_headers[i];
            // Check Section Type
            if (header.sh_type == SHT_STRTAB) {
                // Check Section Name
                char *section_name = (char *) (elf_shstrtab_p + header.sh_name);
                if (strcmp(section_name, ".dynstr") == 0) {
                    // Found
                    elf_strtab_p = file_map + header.sh_offset;
                    break;
                }
            }
        }
        if (elf_strtab_p == NULL) {
            WARN("Unable To Find String Table In: %s", source);
            goto end;
        }

        // Locate Symbol Tables
        std::unordered_map<std::string, Elf32_Sym *> symbols;
        for (int i = 0; i < elf_section_header_count; ++i) {
            // Get Section Header
            Elf32_Shdr header = elf_section_headers[i];
            // Check Section Type
            if (header.sh_type == SHT_DYNSYM) {
                // Symbol Table
                Elf32_Sym *table = (Elf32_Sym *) (file_map + header.sh_offset);
                for (int j = 0; (j * sizeof (Elf32_Sym)) < header.sh_size; j++) {
                    // Check Symbol Name
                    std::string symbol_name = (char *) (elf_strtab_p + table[j].st_name);
                    if (symbol_name.rfind("PCM_", 0) == 0) {
                        // Found
                        symbols[symbol_name] = &table[j];
                    }
                }
            }
        }

        // Loop
        for (auto it : symbols) {
            // Check Symbol
            Elf32_Sym *symbol = it.second;
            if (symbol != NULL) {
                // Convert Virtual Address To File Offset
                Elf32_Shdr symbol_section_header = elf_section_headers[symbol->st_shndx];
                int vaddr_to_offset = -symbol_section_header.sh_addr + symbol_section_header.sh_offset;
                Elf32_Off symbol_offset = symbol->st_value + vaddr_to_offset;
                // Access Symbol
                unsigned char *value = file_map + symbol_offset;
                uint32_t size = symbol->st_size;
                callback(it.first, value, size);
            }
        }
    }

 end:
    // Unmap And Close File
    if (file_map != NULL) {
        munmap(file_map, file_size);
    }
    if (file_obj != NULL) {
        fclose(file_obj);
    }
}

// Main
int main(int argc, char *argv[]) {
    if (argc != 2) {
        ERR("%s", "Invalid Arguments");
    }
    load_symbol(argv[1], [](std::string sound, unsigned char *symbol, uint32_t size) {
        printf("const unsigned char %s[] = {", sound.c_str());
        for (uint32_t i = 0; i < size; i++) {
            printf("%u", (unsigned int) symbol[i]);
            if (i < (size - 1)) {
                printf(", ");
            }
        }
        printf("};\n");
    });
}