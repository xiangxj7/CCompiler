#pragma once

#include <string>
#include "TkAnalysis.h"
void open_file(void);
void read_from_file(void);
void write_into_file(token_info&);
void write_into_file(std::string);
void close_file(void);