package com.knightboost.appoptimizeframework

import android.content.Context
import android.content.Intent
import android.os.*
import android.util.Log
import android.view.LayoutInflater
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.knightboost.appoptimizeframework.databinding.ActivitySliverTestBinding
import com.knightboost.artvm.ArtThread
import com.knightboost.optimize.FixSuspendThreadTimeoutCallback
import com.knightboost.optimize.SuspendTimeoutFixer
import com.knightboost.sliver.Sliver
import com.knightboost.test.SuspendTimeoutTest
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Random

class SliverTestActivity : AppCompatActivity(), FixSuspendThreadTimeoutCallback {

    companion object{
        fun start(context: Context){
            context.startActivity(Intent(context,SliverTestActivity::class.java))
        }
    }

    val mainThreadTid = ArtThread.getThreadId(ArtThread.getNativePeer(Looper.getMainLooper().thread));

    var targetThread:Thread? =null
    lateinit var binding: ActivitySliverTestBinding;
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        setContentView(R.layout.activity_sliver_test)
        binding = ActivitySliverTestBinding.inflate(LayoutInflater.from(this))
        setContentView(binding.main)

        threadLiveAndDeath()
        val sdf = SimpleDateFormat("yyyy-MM-dd HH:mm:ss")
        val currentDateandTime: String = sdf.format(Date())
        binding.startTime.setText("启动时间 "+currentDateandTime)


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
                showMsg("sliver 测试成功")
            }.start()
        }

        binding.btnRemoveProtectedThread.setOnClickListener {
            targetThread?.let {
                SuspendTimeoutFixer.removeThread(it)
            }
        }

        binding.btnSuspendTimeoutCase.setOnClickListener {
            SuspendTimeoutFixer.preventThreadSuspendTimeoutFatalLog(true,this)
            if (Build.VERSION.SDK_INT<31){//模拟Timeout
                Thread {
                    SuspendTimeoutTest.callNativeThreadSuspendTimeout(Thread.currentThread(),ArtThread.getNativePeer(Thread.currentThread()));
                }.start()
            }

        }

        binding.btnSuspendTimeoutCase2.setOnClickListener {
            SuspendTimeoutFixer.preventThreadSuspendTimeoutFatalLog(false,this)
            //保证该线程不出现suspend Timeout
            val targetThread = Thread {
                while (true) {
                    Thread.sleep(5)
                    val stackTrace = Looper.getMainLooper().thread.stackTrace
                }
            }
            targetThread.start()
            SuspendTimeoutFixer.addThread(targetThread)

            if (Build.VERSION.SDK_INT<31){//模拟Timeout
                Thread {
                    SuspendTimeoutTest.callNativeThreadSuspendTimeout(Thread.currentThread(),ArtThread.getNativePeer(Thread.currentThread()));
                }.start()
            }
        }

        binding.btnTestGetThreadCpuTime.setOnClickListener {
            val nativePeer = ArtThread.getNativePeer(Looper.getMainLooper().thread)
            val mainThreadCpuMicroTime = ArtThread.getCpuMicroTime(nativePeer)
            showMsg("time :$mainThreadCpuMicroTime")

        }


        binding.btnGcMock.setOnClickListener {
            GcCase()
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

    private fun threadLiveAndDeath() {
        Thread {
            while (true) {
                Thread.sleep(5)
                Thread.getAllStackTraces()
            }
        }.start()

        Thread {
            while (true) {
                //模拟不断创建线程和死亡的操作
                Thread.sleep(5L)
                Thread {
                    //模拟存活不同时间
                    val time = kotlin.random.Random.nextInt(0, 50)
                    Thread.sleep(time.toLong())
                }.start()
            }
        }.start()
    }

    fun showMsg(msg:String){
        if (Thread.currentThread().id == Looper.getMainLooper().thread.id){
            Toast.makeText(this,msg,Toast.LENGTH_SHORT).show()
        } else{
            Handler(Looper.getMainLooper()).post {
                Toast.makeText(this,msg,Toast.LENGTH_SHORT).show()
            }
        }
        Log.e("sliver",msg)
    }
    override fun triggerSuspendTimeout() {
        showMsg("触发SuspendTimeout 被拦截")
    }

    override fun onError(error: String?) {
        showMsg("Hook失败 $error")
    }

    override fun hookSuspendThreadByIdSuccess() {
        showMsg("hookSuspendThreadByIdSuccess 成功");
    }

    override fun hookSuspendThreadByPeerWarningSuccess() {
        showMsg("hookSuspendThreadByPeerWarningSuccess 成功");

    }

     fun GcCase(){
         Thread{
             while (true) {
                 for (i in 0..99) {
                     val size = kotlin.random.Random.nextInt(0, 1024)
                     val memory = ByteArray(1024 * size) // 申请 0~1MB内存
                     println("Allocated ${memory.size/1024f} Kb memory")
                 }
                 try {
                     Thread.sleep(500) // 模拟停顿一段时间
                 } catch (e: InterruptedException) {
                     e.printStackTrace()
                 }
             }
         }.start()
     }
}