package cn.novate.ndk.day01;

import android.content.Context;

/**
 * Email: 2185134304@qq.com
 * Created by Novate 2018/8/4 10:23
 * Version 1.0
 * Params:
 * Description:    采用 native方法 对请求参数进行签名
*/

public class SignatureUtils {


    /**
     * 让 static 静态代码块加载 这个工具类
     */
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }



    /**
     * 采用 native方法 对请求参数进行签名
     */
    public static native String signatureParams(String params) ;

    /**
     * 校验签名，只允许自己的app才可以使用这个 so，就是需要在后台配置包名和签名
     */
    public static native void signatureVerify(Context context) ;
}
