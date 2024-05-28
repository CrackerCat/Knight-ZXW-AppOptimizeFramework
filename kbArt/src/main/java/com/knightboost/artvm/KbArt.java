package com.knightboost.artvm;

import com.bytedance.shadowhook.ShadowHook;

public class KbArt {
    private static boolean inited = false;

    static {
        loadSo();
    }

    public synchronized static void initShadowHook(){
        if (ShadowHook.getInitErrno() == 2){ //如果未初始化则执行初始化
            ShadowHook.init(
                    new ShadowHook.ConfigBuilder()
                            .setMode(ShadowHook.Mode.SHARED)
                            .build()
            );
        }
    }

    public static void loadSo(){
        try {
            if (inited){
                return;
            }
            System.loadLibrary("kbArt");
            inited = true;
        }catch (Exception e){
            inited = false;
        }
    }



    public static native boolean nSetJdwpAllowed(boolean allowed);

    public static native boolean nIsJdwpAllow();

    public static native boolean nSetJavaDebuggable(boolean debuggable);

    public static native boolean nDisableClassVerify();

    public static native boolean nDelayJit();

    public static native  boolean nResumeJit();

}
