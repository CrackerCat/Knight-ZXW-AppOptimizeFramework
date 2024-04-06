package com.knightboost.sliver;

public class Sliver {

    public static native long[] nativeGetMethodStackTrace(long nativePeer);

    public static native String[] prettyMethods(long[] frames);
}
