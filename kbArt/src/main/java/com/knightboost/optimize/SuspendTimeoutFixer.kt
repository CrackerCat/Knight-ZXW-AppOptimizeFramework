package com.knightboost.optimize
import androidx.annotation.Keep
import com.knightboost.artvm.KbArt

object SuspendTimeoutFixer {
    init {
        KbArt.initShadowHook()
    }

    /**
     * 防止 suspend thread timeout 调用 ThreadSuspendingPeerWarning时 日志级别为Fatal导致的进程崩溃
     * Hook成功后会有一些影响
     * 1. 对于Java层Thread.getStackTrace的调用，原本是会直接崩溃，被Hook后，函数调用会返回 空数组
     * 2. 对于setThreadName的调用，等于没设置成功
     * 因此如果最终要防止程序崩溃，业务层还需要针对频繁调用的地方进行单独处理
     * @param callback
     */
    @Keep
    external fun preventThreadSuspendTimeoutFatalLog(
        replaceSuspendByPeerToThreadIdForAllThread:Boolean,
        callback: FixSuspendThreadTimeoutCallback
    )

    @Keep
    external fun addThread(thread:Thread)

    @Keep
    external fun removeThread(thread:Thread)
}