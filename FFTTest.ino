#include <arduinoFFT.h> 
#include "driver/i2s.h"
#include "driver/adc.h"
#define ADC_SAMPLE_COUNT   1024     //N 采样个数 必须2的N次方
#define ADC_SAMPLE_RATE   40960     //Fs 采样频率 0.025ms一个点,尽可能
#define ADC_CHANNEL_NUM   ADC1_CHANNEL_4 //只能是ADC1
double vReal[ADC_SAMPLE_COUNT];  // 实部
double vImag[ADC_SAMPLE_COUNT]; // 虚部
arduinoFFT FFT = arduinoFFT(vReal,vImag,ADC_SAMPLE_COUNT,ADC_SAMPLE_RATE);  
int32_t m;  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FFT_Test();
}

void loop() {
  // put your main code here, to run repeatedly:

}

//将ADC_SAMPLE_COUNT和ADC_SAMPLE_RATE改成一样数值,好查看数据
void FFT_Test()
{
  // 构造测试信号
  static unsigned long mea;
  static unsigned long meaw;
  meaw = millis();
  mea += 100;
  Serial.println(millis());
  while(millis() - meaw <= 30000){
  if(millis() - mea >= 100){
    mea = millis(); 
  for(int i=0; i<ADC_SAMPLE_COUNT; i++)
  {
    vReal[i] = analogRead(32);  
    vImag[i] = 0; 
  }
  // 计算FFT
  FFT.Windowing(FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD); 
  // 计算幅值
  //vReal[0] = sqrt(sq(vReal[0]) + sq(vImag[0])) / ADC_SAMPLE_COUNT;
  //Serial.println(vReal[0]);

  for(int i = 1;i < ADC_SAMPLE_COUNT/2;i++) //因为数据具有对称性,所以计算一般就可以了
  {
    if((i*ADC_SAMPLE_RATE/ADC_SAMPLE_COUNT)>= 10000 && (i*ADC_SAMPLE_RATE/ADC_SAMPLE_COUNT)< 20000){
      vReal[i] = sqrt(sq(vReal[i]) + sq(vImag[i])) * 2 / ADC_SAMPLE_COUNT;
      //Serial.printf("Hz%d,%.0f\n",i*ADC_SAMPLE_RATE/ADC_SAMPLE_COUNT,vReal[i]);
    }
  }
  //Serial.println(m);
  }
  }
  Serial.println(millis());
}
