//增量式PID
void t12_pid()
{
  if (set_temp >= 40 && t12_switch == 1) //设置温度大于40度&&总开关开启
  {
    //ek0 = set_temp - t12_ad;
    ek0 = set_temp - t12_temp;  //计算现在的差值
    if (ek0 < 0) pid_out = 0;          //差值0度以下
    //差值大于10度
    else if (ek0 > 10) {
      pid_out = 255;
      t12_temp_read_time = 900;
    }
    else if (ek0 <= 10 )//&& ek0 >= 0)    //差值0-10度内
    {
      t12_temp_read_time = 200;  //缩短采样时间
      pid_out = float((p * ek0) - (i * ek1) + (d * ek2) + ((0.0005 * t12_temp + 0.026) * t12_temp)); //最后为0差值补偿
      //pid_out = float((p * ek0) - (i * ek1) + (d * ek2) + (0.05 * t12_temp));
      //pid_out = float((p * (ek0 - ek1)) + (i * ek0) + (d * (ek0 - 2 * ek1 + ek2)));
      if (pid_out > 255) pid_out = 255;
      else if (pid_out < 0)pid_out = 0;
      ek2 = ek1;
      ek1 = ek0;
    }
    analogWrite(t12_pwm_pin, pid_out);
  }

  else {
    pid_out = 0;
    digitalWrite(t12_pwm_pin, 0);
  }
  ec_read(); //读取电流
}
