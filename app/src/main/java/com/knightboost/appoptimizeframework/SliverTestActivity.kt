package com.knightboost.appoptimizeframework

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.LayoutInflater
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.knightboost.appoptimizeframework.databinding.ActivitySliverTestBinding
import com.knightboost.artvm.ArtThread
import com.knightboost.sliver.FixSuspendThreadTimeoutCallback
import com.knightboost.sliver.Sliver
import com.knightboost.test.SuspendTimeoutTest
import kotlin.text.StringBuilder

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
            Thread {
                val nativeGetMethodStackTrace = Sliver.nativeGetMethodStackTrace(
                    ArtThread.getNativePeer(Looper.getMainLooper().thread)
                )
                val prettyMethods = Sliver.prettyMethods(nativeGetMethodStackTrace)
                val stackBuilder = StringBuilder()
                for (method in prettyMethods) {
                    stackBuilder.append(method).append("\n")
                }
                Log.e("sliver","stack is $stackBuilder")
            }.start()
        }



        binding.btnReplaceSuspendMethod.setOnClickListener {
            Sliver.testBuildInUnReachable();
//            val thread = Thread (){
//                while (true){
//                    Thread.sleep(1000L)
//                    val stackTrace = Looper.getMainLooper().thread.stackTrace
//                }
//            }
//            thread.name ="sampleThread"
//            thread.start()
//            Sliver.replaceThreadByPeerToById(thread,object :ReplaceSuspendThreadMethodCallback{
//                override fun onReplaceSuccess() {
//                    Log.e("sliver","函数替换成功")
//                }
//
//                override fun onError(error: String?) {
//                    Log.e("sliver","函数替换失败 $error")
//
//                }
//            })
//
//            val thread2 = Thread (){
//                while (true){
//                    Thread.sleep(1000L)
//                    val stackTrace = Looper.getMainLooper().thread.stackTrace
//                }
//            }
//            thread2.name ="sampleThread2"
//            thread2.start()
        }

        binding.btnTestGetThreadCpuTime.setOnClickListener {
            val nativePeer = ArtThread.getNativePeer(Looper.getMainLooper().thread)
            val mainThreadCpuMicroTime = ArtThread.getCpuMicroTime(nativePeer)
            Log.d("zxw","主线程CpuMicroTime "+mainThreadCpuMicroTime)
        }

        binding.btnSuspendTimeoutCase.setOnClickListener {
            Sliver.preventThreadSuspendTimeoutFatalLog(object : FixSuspendThreadTimeoutCallback {
                override fun triggerSuspendTimeout() {
                    Log.e("sliver","系统触发了timeout")
                    Handler(Looper.getMainLooper()).post {
                        Toast.makeText(this@SliverTestActivity,"系统触发了timeout",Toast.LENGTH_SHORT).show()
                    }
                }

                override fun onError(error: String?) {
                    Toast.makeText(this@SliverTestActivity,"error "+error,Toast.LENGTH_SHORT).show()
                }
            })
            Thread {
                SuspendTimeoutTest.callNativeThreadSuspendTimeout(Thread.currentThread(),ArtThread.getNativePeer(Thread.currentThread()));
            }.start()
        }


        binding.btnSuspendMainThread.setOnClickListener {
            Thread {
                Log.e(
                    "zxw", "开始获取主线程调用栈,主线程 threadId = ${mainThreadTid}" +
                            " tid = ${ArtThread.getTid(Looper.getMainLooper().thread)}"
                )
                var nativePeer = ArtThread.suspendThreadByThreadId(mainThreadTid)
                if (nativePeer == -1L) {
                    Log.e("zxw", "暂停主线程失败")
                    return@Thread
                } else {
                    Log.e(
                        "zxw", "native peer = ${nativePeer} ,自己获取的nativePeer = ${
                            ArtThread.getNativePeer(Looper.getMainLooper().thread)
                        }"
                    )
                }
                val stackTrace = Looper.getMainLooper().thread.stackTrace
                ArtThread.resumeThread(nativePeer)
            }.start()

        }

    }
}