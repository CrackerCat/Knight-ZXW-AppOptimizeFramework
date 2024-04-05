//
// Created by Administrator on 2024/4/4.
//
#pragma once

namespace art {
namespace mirror {

class Object;

// Classes shared with the managed side of the world need to be packed so that they don't have
// extra platform specific padding.
#define MANAGED PACKED(4)
}
}