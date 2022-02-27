#define DELAY_SAMPLE 10         // delay between samples (ms)
#define NUM_SAMPLES 10          // number of analog samples to take per reading

class Battery{
  private:
    int _analogPin;
    int _r1;
    int _r2;
    float _voltage_min;
    float _voltage_max;
    int _level_min;
    int _level_max;
    float _resistor_ratio;

    int _getSum(){
      // take a number of analog samples and add them up
      int sum=0;
      unsigned char sample_count;
      while (sample_count < NUM_SAMPLES) {
        sum += analogRead(_analogPin);
        sample_count++;
        delay(DELAY_SAMPLE);
      }
      return sum;
    }

  public:
    void begin(int analogPin, float voltage_min, float voltage_max, int r1, int r2){
      _analogPin = analogPin;
      _r1=r1;
      _r2=r2;
      _voltage_min=voltage_min;
      _voltage_max=voltage_max;

      _resistor_ratio = (float)_r2/((float)_r1+(float)_r2);
      _level_min = (float)_resistor_ratio*(float)_voltage_min*1024;
      _level_max = (float)_resistor_ratio*(float)_voltage_max*1024;
    }

    void begin(int analogPin, float voltage_min, float voltage_max){
      _analogPin = analogPin;
      _voltage_min=voltage_min;
      _voltage_max=voltage_max;
    }

    int analogPin(){return _analogPin;}
    int r1(){return _r1;}
    int r2(){return _r2;}
    float resistorRatio(){return _resistor_ratio;}
    int levelMin(){return _level_min;}
    int levelMax(){return _level_max;}

    int level(){
      // calculate the average level
      int sum = _getSum();
      return (float)sum / (float)NUM_SAMPLES;
    }

    int percentage(){
      // convert battery level to percent
      int percentage = map(level(), _level_min, _level_max, 0, 100);
      return percentage;
    }

    float voltage(){
      float voltage = (float)map(level(), _level_min, _level_max, _voltage_min*100, _voltage_max*100)/100;
      return voltage;
    }
};
