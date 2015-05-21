#pragma once

#include <string>

#define SCREEN_WIDTH 1620
#define SCREEN_HEIGHT 800
#define DATA_DIRECTORY std::string("${CHGAME_INCLUDE_DIR}/")

#ifndef NDEBUG
#cmakedefine DEBUG
#endif

