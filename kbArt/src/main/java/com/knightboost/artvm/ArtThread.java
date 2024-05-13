package com.knightboost.artvm;

import java.lang.reflect.Field;

public class ArtThread {

    static {
        KbArt.loadSo();
    }

    /**
     * 返回Java Thread 对应内核线程的tid
     * @param thread
     * @return
     */
    public static native int getTid(Thread thread);

    /**
     * 返回art::Thread 当前的cpuMicroTime
     * @param nativePeer
     * @return
     */
    public static native long getCpuMicroTime(long nativePeer);

    //TODO 测试需改为jlong，即返回线程指针地址

    /**
     * 暂停目标Java线程，如果暂停成功，会返回目标线程的ArtThread native地址
     * @param threadId
     * @return  -1 表示suspend 失败 ,成功情况下 会返回该Java线程 对应的ArtThread 地址
     */
    public static native long suspendThreadByThreadId(int threadId);
    public static native void resumeThread(long threadNativePeer);



    public static long getNativePeer(Thread thread){
        try {
            Field nativePeer = Thread.class.getDeclaredField("nativePeer");
            nativePeer.setAccessible(true);
            long nativePeerValue = (long) nativePeer.get(thread);
            return nativePeerValue;
        } catch (Exception e) {
            return -1;
        }
    }
}
