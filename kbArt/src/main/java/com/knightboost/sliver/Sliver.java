package com.knightboost.sliver;

import androidx.annotation.Keep;

public class Sliver {

    public static native long[] nativeGetMethodStackTrace(long nativePeer);

    public static native String[] prettyMethods(long[] frames);

    public static native void preventThreadSuspendTimeoutFatal(long tid,
                                                               HookSuspendThreadTimeoutCallback callback);

    /**
     * 监听suspend timeout
     */
    @Keep
    public static void onSuspendTimeout(){

    };
}
