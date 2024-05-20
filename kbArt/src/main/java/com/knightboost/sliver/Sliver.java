package com.knightboost.sliver;


import androidx.annotation.Keep;

import com.bytedance.shadowhook.ShadowHook;
import com.knightboost.artvm.KbArt;

public class Sliver {
    static {
        //保证shadowhook初始化
        if (ShadowHook.getInitErrno() == 2){
            ShadowHook.init(
                    new ShadowHook.ConfigBuilder()
                            .setMode(ShadowHook.Mode.SHARED)
                            .build()
            );
        }
        KbArt.loadSo();
    }


    public static native long[] nativeGetMethodStackTrace(long nativePeer);

    public static native String[] prettyMethods(long[] frames);

    /**
     * 防止 suspend thread timeout 调用 ThreadSuspendingPeerWarning时 日志级别为Fatal导致的进程崩溃
     * Hook成功后会有一些影响
     *    1. 对于Java层Thread.getStackTrace的调用，原本是会直接崩溃，被Hook后，函数调用会返回 空数组
     *    2. 对于setThreadName的调用，等于没设置成功
     * 因此如果最终要防止程序崩溃，业务层还需要针对频繁调用的地方进行单独处理
     * @param callback
     */
    public static native void preventThreadSuspendTimeoutFatalLog(
            HookSuspendThreadTimeoutCallback callback);

    /**
     * 监听suspend timeout
     */
    @Keep
    public static void onSuspendTimeout(){

    };
}
