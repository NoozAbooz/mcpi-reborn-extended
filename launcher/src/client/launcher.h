#pragma once

#include <string>
#include <functional>

// Defaults
#define DEFAULT_USERNAME "AlexPi"
#define DEFAULT_RENDER_DISTANCE "Short"

// Feature Flags
std::string strip_feature_flag_default(std::string flag, bool *default_ret);
void load_available_feature_flags(std::function<void(std::string)> callback);