package android.util;

public class Log {
    public static int e(String tag, String msg){
        throw new RuntimeException();
    }
    public static int e(String tag, String msg, Throwable tr){
        throw new RuntimeException();
    }

    public static int w(String tag, String msg){
        throw new RuntimeException();
    }
    public static int w(String tag, String msg, Throwable tr){
        throw new RuntimeException();
    }

}
