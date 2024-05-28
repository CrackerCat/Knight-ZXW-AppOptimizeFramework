//
// Created by Knight-ZXW on 2024/5/14.
//
#ifndef KB_ART_JNI_H_
#define KB_ART_JNI_H_
#include "jni.h"
namespace kbArt {
extern JavaVM *gJavaVM; // 全局变量用于保存JavaVM指针


JNIEnv *getJNIEnv();
}
#endif
