#include <jni.h>
#include "art_runtime.h"
#include "sliver/fetch_stack_visitor.h"
#include "vector"
#include <chrono>
#include "logger.h"
#include "logger.h"
#include "art_thread.h"
#include "suspend_reason.h"
using namespace art;


extern "C"
JNIEXPORT jlongArray JNICALL
Java_com_knightboost_sliver_Sliver_nativeGetMethodStackTrace(JNIEnv *env,
                                                             jclass clazz,
                                                             jlong native_peer) {

  auto* thread = reinterpret_cast<Thread *>(native_peer);

  bool timeOut;
  Thread *current_thread = Thread::Current();

  bool isSameThread = false;
  if (current_thread == thread){
    isSameThread = true;
  }
  void* suspendThread = nullptr;
  if (!isSameThread){
    //TODO 判断是否超时
    suspendThread = ArtRuntime::Get()->GetThreadList()->SuspendThreadByThreadId(thread->GetThreadId(),
                                                         art::SuspendReason::kForUserCode,
                                                         &timeOut);

  }

  if (suspendThread == nullptr){
    //suspend 失败，返回空数组
    jlongArray result = env->NewLongArray(0);
    return result;
  }

  std::vector<std::uintptr_t> stack_methods;
  auto f = [](ArtMethod *method, void *visitorData) -> bool {
    auto *methods = reinterpret_cast<std::vector<std::uintptr_t> *>(visitorData);
    methods->push_back(reinterpret_cast<std::uintptr_t>(&*method));
    return true;
  };

  FetchStackVisitor visitor(thread,
                            &stack_methods,
                            f);
  visitor.WalkStack(false);

  if (!isSameThread){
    LOGE("sliver"," thread = %p ,suspendThread = %p",thread,suspendThread);
    ArtRuntime::Get()->GetThreadList()->Resume(thread, art::SuspendReason::kForUserCode);
  }

  jlongArray methodArray = env->NewLongArray((jsize) stack_methods.size());

  jlong *destElements = env->GetLongArrayElements(methodArray, nullptr);
  for (int i = 0; i < stack_methods.size(); i++) {
    destElements[i] =(jlong) stack_methods[i];
  }
  env->ReleaseLongArrayElements(methodArray, destElements, 0);
  return methodArray;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_knightboost_sliver_Sliver_prettyMethods(JNIEnv *env, jclass clazz, jlongArray methods) {
  jlong *methods_ptr = env->GetLongArrayElements(methods, nullptr);
  jsize size = env->GetArrayLength(methods);
  jobjectArray ret = env->NewObjectArray(size, env->FindClass("java/lang/String"),
                                         nullptr);

  for (int i = 0; i < size; i++) {
    const std::string
        &pretty_method = reinterpret_cast<ArtMethod *>(methods_ptr[i])->PrettyMethod(true);
    pretty_method.c_str();
    jstring frame = env->NewStringUTF(pretty_method.c_str());
    env->SetObjectArrayElement(ret, i, frame);
    env->DeleteLocalRef(frame);
  }
  env->ReleaseLongArrayElements(methods,methods_ptr,0);
  return ret;

}
