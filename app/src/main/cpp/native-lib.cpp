#include <unistd.h>
#include "com_example_voicechange_MainActivity.h"
#include <android/log.h>

using namespace FMOD;

#define TAG "ZJW"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

extern "C"
JNIEXPORT void JNICALL
Java_com_example_voicechange_MainActivity_voiceChangeNative(JNIEnv *env, jobject thiz,
                                                            jint mode, jstring path) {
   char * content_ = "默认 播放完毕";

   const char * path_ = env->GetStringUTFChars(path, NULL);

   //音效引擎系统
   System *system = NULL;

   //声音
   Sound * sound = NULL;

   //通道 音轨
   Channel * channel = NULL;

   //数字信号处理
   DSP * dsp = NULL;

   //todo 一、创建系统
   System_Create(&system);

   //todo 二、系统的初始化  1.最大通道数  2.初始化标记  3.额外数据
   system->init(32, FMOD_INIT_NORMAL, 0);

   //todo 三、创建声音  1.路径  2.声音初始化标记  3.额外数据  4.声音指针
   system->createSound(path_, FMOD_DEFAULT, 0, &sound);

   //todo 四、播放声音  1.声音  2.通道分组  3.控制   4.通道
   system->playSound(sound, 0, false, &channel);

   //todo 五、添加特效
   switch (mode) {
      case com_example_voicechange_MainActivity_MODE_NORMAL:
         content_ =  "原生 播放完毕";
         break;
      case com_example_voicechange_MainActivity_MODE_LUOLI:
         content_ =  "萝莉 播放完毕";

         system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
         dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
         channel->addDSP(0,dsp);
         break;
      case com_example_voicechange_MainActivity_MODE_DASHU:
         content_ =  "大叔 播放完毕";
         system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
         dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
         channel->addDSP(0,dsp);
         break;
      case com_example_voicechange_MainActivity_MODE_JINGSONG:
         content_ =  "惊悚 播放完毕";

         // 惊悚音效特点：很多声音的拼接
         // todo 音调低
         system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
         dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
         channel->addDSP(0,dsp);
         // todo 搞点回声
         system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
         dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); //回音 延时
         dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); //回音 衰减度
         channel->addDSP(1,dsp);
         // todo 颤抖
         system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
         dsp->setParameterInt(FMOD_DSP_TREMOLO_FREQUENCY, 20);
         dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8f);
         channel->addDSP(2, dsp);
         break;
      case com_example_voicechange_MainActivity_MODE_GAOGUAI:
         content_ =  "搞怪 播放完毕";

         //小黄人 频率快
         float mFrequency;
         channel->getFrequency(&mFrequency);
         channel->setFrequency(mFrequency * 1.5f);

         system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
         dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
         channel->addDSP(0,dsp);
         break;
      case com_example_voicechange_MainActivity_MODE_KONGLING:
         content_ =  "空灵 播放完毕";

         system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
         dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); //回音 延时
         dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); //回音 衰减度
         channel->addDSP(0,dsp);
         break;
   }

   //等待播放完毕 再回收
   bool isPlay = true;
   while (isPlay) {
      channel->isPlaying(&isPlay);
      usleep(1000*1000);

   }

   //回收
   system->close();
   sound->release();
   system->release();
   env->ReleaseStringUTFChars(path, path_);

   //播放完毕时，返回给java层信号
   jclass jclazz = env->FindClass("com/example/voicechange/MainActivity");
   jmethodID jmethodId = env->GetMethodID(jclazz, "playerEnd", "(Ljava/lang/String;)V");
   jstring value = env->NewStringUTF(content_);
   env->CallVoidMethod(thiz, jmethodId, value);

}