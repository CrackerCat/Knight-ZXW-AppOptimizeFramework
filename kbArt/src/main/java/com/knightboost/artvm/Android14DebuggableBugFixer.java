package com.knightboost.artvm;

public class Android14DebuggableBugFixer {
    static {
        System.loadLibrary("kbArt");
    }
    public static native void fix();
}
