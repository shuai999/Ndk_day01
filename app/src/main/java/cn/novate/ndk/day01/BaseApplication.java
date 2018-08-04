package cn.novate.ndk.day01;

import android.app.Application;

/**
 * Created by Administrator on 2018/8/4.
 */

public class BaseApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        // 签名授权
        SignatureUtils.signatureVerify(this);
    }
}
