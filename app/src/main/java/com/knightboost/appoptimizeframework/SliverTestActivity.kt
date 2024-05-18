package com.knightboost.appoptimizeframework

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.os.SystemClock
import android.util.Log
import android.view.LayoutInflater
import androidx.appcompat.app.AppCompatActivity
import com.knightboost.appoptimizeframework.databinding.ActivitySliverTestBinding
import com.knightboost.artvm.ArtThread
import com.knightboost.sliver.HookSuspendThreadTimeoutCallback
import com.knightboost.sliver.Sliver
import com.knightboost.test.SuspendTimeoutTest
import java.lang.StringBuilder
import java.util.HexFormat

class SliverTestActivity : AppCompatActivity() {

    companion object{
        fun start(context: Context){
            context.startActivity(Intent(context,SliverTestActivity::class.java))
        }
    }

    val mainThreadTid = ArtThread.getThreadId(ArtThread.getNativePeer(Looper.getMainLooper().thread));

    lateinit var binding: ActivitySliverTestBinding;
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        setContentView(R.layout.activity_sliver_test)
        binding = ActivitySliverTestBinding.inflate(LayoutInflater.from(this))
        setContentView(binding.main)

        binding.btnThreadStacktraceTest.setOnClickListener {


            val nativePeer = ArtThread.getNativePeer(Looper.getMainLooper().thread)

            val mainThreadCpuMicroTime = ArtThread.getCpuMicroTime(nativePeer)
            Log.d("zxw","主线程CpuMicroTime "+mainThreadCpuMicroTime)

            val frames = Sliver.nativeGetMethodStackTrace(nativePeer);
            val prettyMethods = Sliver.prettyMethods(frames)
            Log.e("zxw","methods is "+prettyMethods)
        }

        binding.btnTestGetThreadCpuTime.setOnClickListener {
            val nativePeer = ArtThread.getNativePeer(Looper.getMainLooper().thread)
            val mainThreadCpuMicroTime = ArtThread.getCpuMicroTime(nativePeer)
            Log.d("zxw","主线程CpuMicroTime "+mainThreadCpuMicroTime)
        }

        binding.btnSuspendTimeoutCase.setOnClickListener {
            //主线程模拟死循环，进入不可
            Handler(Looper.getMainLooper()).postDelayed( {
            },100)
            //异步线程测试是否会出现 timeout
            Thread {
                while (true) {
                    Thread.sleep(50)

                    Log.e("zxw", "开始获取主线程调用栈")
                    val b0 = SystemClock.elapsedRealtimeNanos()
                    var nativePeer = ArtThread.suspendThreadByThreadId(mainThreadTid)
                    if (nativePeer ==-1L){
                        Log.e("zxw","暂停主线程失败")
                        return@Thread
                    }
                    val stackTrace = Looper.getMainLooper().thread.stackTrace
                    ArtThread.resumeThread(nativePeer)
                    val b1 = SystemClock.elapsedRealtimeNanos()
                    Log.e("zxw", "获取主线程调用栈成功耗时 ${(b1-b0)/1000} 微秒")
                }

            }.start()
        }

        Sliver.preventThreadSuspendTimeoutFatalLog(object :HookSuspendThreadTimeoutCallback{
            override fun triggerSuspendTimeout() {
                Log.e("sliver","系统触发了timeout")
            }

            override fun onError(error: String?) {
                Log.e("sliver","preventThreadSuspendTimeoutFatal error")
            }
        })
        binding.btnSuspendMainThread.setOnClickListener {

            Thread {
                while (true){
                    Thread.sleep(1000)
                    SuspendTimeoutTest.callNativeThreadSuspendTimeout(Thread.currentThread(),ArtThread.getNativePeer(Thread.currentThread()));
                }
            }.start()


        }

    }
}