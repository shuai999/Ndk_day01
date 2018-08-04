#include <jni.h>
#include <string>
#include <string.h>
// 引入md5头文件即可，不要引入md5.cpp
#include "md5.h"
// 导入android的打印log的包
#include <android/log.h>


// 额外附加的字符串
static char* EXTRA_SIGNATURE = "NOVATE" ;
// 声明命名空间
using namespace std;


// 校验签名
// 校验成功的变量
static int is_verify = 0 ;
// 包名
static char *PACKAGE_NAME = "cn.novate.ndk.day01" ;
// 签名：实际中应该是打包后的签名，这里为了测试，是在MainActivity中直接用java的方式打印出的签名，然后下边用 c代码调用java代码
static char *APP_SIGNATURE = "308201dd30820146020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3137313031373035333032375a170d3437313031303035333032375a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330819f300d06092a864886f70d010101050003818d0030818902818100bbc7749770f1b4c9e8b37cdb7254fab9048243dc95b186eb9dd85b7f0ec22467f99bb07981cc3b5cbbb0d442fd2af6bb8e03ea2d666c97fac97a154b94455d47c27f81b55abaf788e87c344bdcd8d8777763bb88622a1211aa4c122a2e996ef059e294fef6e28897c0db1ac28e213bf5d3136a90a925079a9aea37cd4e653a750203010001300d06092a864886f70d0101050500038181008bc958dbc90795e1366899bdbefe7017b214fdd959d7b2e3b6d6d74c745e0735d4b9adb31133846f531353a03db5472c8cd55e0aadb57d321cea00279763e1129ca875b20df3059fbf209ed08fa036b2eedcd945e46614ac6a86130ffa1886a1f101cb702c2c6f6af272515c0826867f5c194a6b574f806df1f091b81c34773a" ;

extern "C"{

    // 在这里对下边方法声明一下，或者写一个.h头文件也行
    JNIEXPORT jstring JNICALL
    Java_cn_novate_ndk_day01_SignatureUtils_signatureParams(JNIEnv *env, jclass type, jstring params_);

    // 在这里对下边方法声明一下
    JNIEXPORT void JNICALL
    Java_cn_novate_ndk_day01_SignatureUtils_signatureVerify(JNIEnv *env, jclass type, jobject context);
}



JNIEXPORT jstring JNICALL
Java_cn_novate_ndk_day01_SignatureUtils_signatureParams(JNIEnv *env, jclass type, jstring params_) {
    const char *params = env->GetStringUTFChars(params_, 0);


    // 在这里判断校验是否成功，如果成功直接return
    if(is_verify == 0){
        return env -> NewStringUTF("error_signature") ;
    }



    // TODO
    // MD5签名规则，就是说加点料：比如：
    // 1. 字符串前边加一个名字;
    string signature_str(params) ;
    signature_str.insert(0,EXTRA_SIGNATURE);
    // 2. 后边去掉两位; 或者其他，这个规则自己随便定义，目的就是一个：就是不想别人破解
    signature_str = signature_str.substr(0,signature_str.length()-2) ;

    // 3. md5加密 C++和Java是一样的，唯一不同的就是需要自己回收内存
    MD5_CTX *ctx = new MD5_CTX() ;
    // 初始化MD5
    MD5Init(ctx) ;
    MD5Update(ctx , (unsigned char *)signature_str.c_str() , signature_str.length()) ;
    // 会把加密的数据放到digest[16]字符串 中，
    unsigned char digest[16] = {0};
    MD5Final(digest , ctx) ;


    // 生成32位字符串
    char md5_str[32] ;
    for (int i = 0 ; i < 16 ; i++){
        // 不足的情况下补0，比如是f，就是0f；
        sprintf(md5_str , "%s%02x" , md5_str , digest[i]) ;
    }


    // 释放资源
    env->ReleaseStringUTFChars(params_, params);


    // 直接返回signature_str.c_str()即可
    return env->NewStringUTF(md5_str);
}


/**
 * 下边方法是自带的，可以注释掉，不需要下边方法
 */
/*extern "C"
JNIEXPORT jstring JNICALL
Java_cn_novate_ndk_day01_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject *//* this *//*) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}*/


/**
PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
Signature[] signatures = packageInfo.signatures;
return signatures[0].toCharsString();
 */

// C 调用 java 代码，这个其实就是 JNI的基础
JNIEXPORT void JNICALL
Java_cn_novate_ndk_day01_SignatureUtils_signatureVerify(JNIEnv *env, jclass type, jobject context) {

    // TODO
    // 1. 获取包名
    jclass j_clz = env->GetObjectClass(context) ;
    // getPackageName:表示方法名；
    // ()Ljava/lang/String; ：表示方法参数；
    // ()：表示无参构造方法
    jmethodID j_mid = env->GetMethodID(j_clz , "getPackageName" , "()Ljava/lang/String;") ;
    jstring j_package_name = (jstring) env->CallObjectMethod(context , j_mid);

    // 2. 比对包名是否一样
    const char * c_package_name = env -> GetStringUTFChars(j_package_name , NULL) ;
    // strcmp方法: 字符串比较
    if(strcmp(c_package_name , PACKAGE_NAME) != 0){
        return;
    }

    // 打印log , %s表示占位符  打印结果：包名一致: cn.novate.ndk.day01
    __android_log_print(ANDROID_LOG_ERROR , "JNI_TAG" , "包名一致: %s" , c_package_name) ;

    // 3. 获取签名
    // 3.1 获取 PackageManager
    j_mid = env -> GetMethodID(j_clz , "getPackageManager" , "()Landroid/content/pm/PackageManager;") ;
    jobject  package_manager = env->CallObjectMethod(context , j_mid) ;
    // 3.2 获取 PackageInfo
    j_clz = env->GetObjectClass(package_manager) ;
    j_mid = env->GetMethodID(j_clz , "getPackageInfo" , "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;") ;
    // 0x00000040 = 64
    jobject package_info = env->CallObjectMethod(package_manager , j_mid , j_package_name , 0x00000040) ;
    // 3.3 获取 signature数组
    j_clz = env -> GetObjectClass(package_info) ;
    jfieldID j_fid = env->GetFieldID(j_clz , "signatures" , "[Landroid/content/pm/Signature;") ;
    jobjectArray signatures = (jobjectArray) env->GetObjectField(package_info , j_fid);
    // 3.4 获取 signature[0]
    jobject signature_first = env->GetObjectArrayElement(signatures,0) ;
    // 3.5 调用 signatures[0].toCharsString()
    j_clz = env->GetObjectClass(signature_first) ;
    j_mid = env->GetMethodID(j_clz , "toCharsString" , "()Ljava/lang/String;") ;
    jstring j_signature_str = (jstring) env->CallObjectMethod(signature_first , j_mid);
    const char * c_signature_str = env->GetStringUTFChars(j_signature_str,NULL) ;
    // 比较字符串
    // 比较签名是否相等，如果不相等，则直接 return；
    if(strcmp(c_signature_str , APP_SIGNATURE) != 0){
        return;
    }


    // 打印下日志
    __android_log_print(ANDROID_LOG_ERROR , "JNI_TAG" , "签名校验成功：%s" , c_signature_str) ;
    // 4. 比对签名是否一样，如果签名一样，就让 is_verify = 1 ;
    is_verify = 1 ;

}