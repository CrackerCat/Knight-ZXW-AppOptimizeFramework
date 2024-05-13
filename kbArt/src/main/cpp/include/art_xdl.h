//
// Created by Administrator on 2024/3/25.
//

#pragma once
void *open_art_handle();
void *dsym(void *handle, const char *symbol);
void *close_handle(void* handle);
void *findArtSoSym(const char*symbol);
#define once_dysm
