//
// Created by Administrator on 2024/3/25.
//
#include <xdl.h>
#include "art_xdl.h"
#include "common.h"

void *open_art_handle() {
  char *artPath = getLibArtPath();
  void* handle = xdl_open(artPath,
                          XDL_TRY_FORCE_LOAD);
  return handle;
}
void *dsym(void *handle, const char *symbol) {
  void *sym = xdl_dsym(handle, symbol, nullptr);
  return sym;
}

void *close_handle(void* handle){
  return xdl_close(handle);
}

void *findArtSoSym(const char*symbol){
  void *handle = open_art_handle();
  if (handle == nullptr){
    return nullptr;
  }
  void *sym = dsym(handle,symbol);
  xdl_close(handle);
  return sym;
}

