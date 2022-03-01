#include <arduinoFFT.h> 
#define ADC_SAMPLE_COUNT  512           //采样个数 必须2的N次方
#define ADC_SAMPLE_RATE   512           //采样频率，分辨率是ADC_SAMPLE_RATE/ADC_SAMPLE_COUNT，即相邻两点的频率间隔
#define FFT_LowBound      0             //FFT下限
#define FFT_UpBound       200           //只计算打印特定频率范围的FFT幅值结果，可以节省时间
#define PI2               6.28318530717959
#define Fn(a,f,p,i) (a) * cos(PI2 * (f) * ((double)(i)/ADC_SAMPLE_RATE) + (p) * PI2 / 360) //测试信号 
double vReal[ADC_SAMPLE_COUNT];          // 实部
double vImag[ADC_SAMPLE_COUNT];          // 虚部
arduinoFFT FFT = arduinoFFT(vReal,vImag,ADC_SAMPLE_COUNT,ADC_SAMPLE_RATE);    
void setup() {
  Serial.begin(9600);
  FFT_Test();
}

void loop() {
}
/*
 * 测试FFT的分析结果准确性，注意采样频率需为信号最高频率的2倍
 */
void FFT_Test()
{
  for(int i=0; i<ADC_SAMPLE_COUNT; i++)
  {
    double f1 = 50;             //0Hz,幅值50
    double f2 = Fn(1,50,0,i);   //50Hz,幅值1,相位0度
    double f3 = Fn(1,150,0,i);   //150Hz,幅值1,相位0度
    vReal[i] = f1 + f2 + f3;  
    vImag[i] = 0; 
  }
  //FFT
  FFT.Windowing(FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD); 
  vReal[0] = sqrt(sq(vReal[0]) + sq(vImag[0])) / ADC_SAMPLE_COUNT;  //直流分量
  int Freq;
  for(int i = 1;i < ADC_SAMPLE_COUNT/2;i++)                         //各频率分量,
  {
    Freq = i*ADC_SAMPLE_RATE/ADC_SAMPLE_COUNT;
    if((Freq >= FFT_LowBound) && (Freq <= FFT_UpBound)){
      vReal[i] = sqrt(sq(vReal[i]) + sq(vImag[i])) * 2 / ADC_SAMPLE_COUNT;
      Serial.printf("%dHz\t%.0f\n",Freq,vReal[i]);
    }
  }
}
