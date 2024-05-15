package com.knightboost.sliver;

import androidx.annotation.Keep;

public class Sliver {

    public static native long[] nativeGetMethodStackTrace(long nativePeer);

    public static native String[] prettyMethods(long[] frames);

    /**
     * 防止 suspend thread timeout 调用 ThreadSuspendingPeerWarning时 日志级别为Fatal导致的进程崩溃
     * @param callback
     */
    public static native void preventThreadSuspendTimeoutFatal(
            HookSuspendThreadTimeoutCallback callback);

    /**
     * 监听suspend timeout
     */
    @Keep
    public static void onSuspendTimeout(){

    };
}
