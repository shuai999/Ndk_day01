package cn.novate.ndk.day01;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

/**
 * Email: 2185134304@qq.com
 * Created by Novate 2018/8/4 17:25
 * Version 1.0
 * Params:
 * Description:
*/

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    // Example of a call to a native method
    TextView tv = (TextView) findViewById(R.id.sample_text);

    // 参数的加密不能每次都加密，应该是在运行时架构中，也就是在你抽取的网络请求工具类中加密，
        // 只需要加密一次就可以，否则你就需要在所有请求网络中全部进行加密，这个时候就需要：
        // 1. 对参数进行字典排序，用HashMap进行排序；2. 生成待加密的连接，如userName=Novate&userPwd=123456，这样加密之后就传给后台即可

        String signature = SignatureUtils.signatureParams("userName=Novate&userPwd=123456") ;

        // 把signature作为参数传递给服务器，服务器会对这个加密的字符串进行比较，这样你就不能伪造字符串
    tv.setText(signature);

        PackageInfo packageInfo = null;
        try {
            packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        Signature[] signatures = packageInfo.signatures;
        Log.e("TAG",signatures[0].toCharsString());
        tv.setText(SignatureUtils.signatureParams("userName=Novate&userPwd=123456"));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    /*public native String stringFromJNI();*/

}
