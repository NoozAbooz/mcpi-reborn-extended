#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*misc_update_function_t)(unsigned char *minecraft);
void misc_run_on_update(misc_update_function_t function);

void Level_saveLevelData_injection(unsigned char *level);

__attribute__((visibility("internal"))) void _init_misc_cpp();
__attribute__((visibility("internal"))) void _init_misc_logging();

void misc_set_sprinting(int enable);

#ifdef __cplusplus
}
#endif
