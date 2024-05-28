package com.knightboost.sliver;


import com.bytedance.shadowhook.ShadowHook;
import com.knightboost.artvm.KbArt;
import com.knightboost.optimize.FixSuspendThreadTimeoutCallback;

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


}
