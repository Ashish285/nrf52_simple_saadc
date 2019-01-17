#include "nrfx_saadc.h"
#include "nrf_drv_saadc.h"
#include "nrf_delay.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SAMPLES_IN_BUFFER 1

static nrf_saadc_value_t m_buffer[SAMPLES_IN_BUFFER];
int16_t adc_result=0;

void idle_state_handle()
{
  nrf_pwr_mgmt_run();
}

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
  if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
  {
    ret_code_t err_code;
    
    err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);
    adc_result = p_event->data.done.p_buffer[0];
  }
}

void init_saadc()
{
  ret_code_t err_code;
  /*Config the channel*/
  nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDD);
  //channel_config.acq_time = NRF_SAADC_ACQTIME_20US;
  
  err_code = nrf_drv_saadc_init(NULL, NULL);
  APP_ERROR_CHECK(err_code);
  
  err_code = nrf_drv_saadc_channel_init(0, &channel_config);
  APP_ERROR_CHECK(err_code);
  
 // err_code = nrf_drv_saadc_buffer_convert(m_buffer,SAMPLES_IN_BUFFER);
 // APP_ERROR_CHECK(err_code);
}

void take_adc_sample()
{
  nrfx_saadc_sample_convert(NRF_SAADC_INPUT_VDD,&adc_result);
}

int main()
{
  NRF_LOG_INIT(NULL);
  
  NRF_LOG_DEFAULT_BACKENDS_INIT();                                                      //Use RTT as default backend
  NRF_LOG_INFO("Start\r\n");
  
  init_saadc();
  while(1)
  {
    take_adc_sample();
    NRF_LOG_INFO("ADC Value: %d",adc_result);
    nrf_delay_ms(1000);
    //idle_state_handle();
  }
}
