#include <inttypes.h>
#include "SPIfg.h"
#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include <ti/drivers/Board.h>

 SPI_Handle  spi;

    uint8_t SPI_transf(uint8_t data) {
        uint8_t tx = data;
        uint8_t rx;

        SPI_Transaction trans;
        trans.txBuf = &tx;
        trans.count = sizeof(tx);
        trans.rxBuf = &rx;
        bool tsOK = SPI_transfer(spi, &trans);

        if(tsOK)
        {
            if(trans.rxBuf == 0)
            {
                return 0;
            }
            else
            {
                return rx;
            }
        }
        else
            return 0;
    }

    void SPI_begin() {
          SPI_Params  spiParams;

          SPI_init();

          SPI_Params_init(&spiParams);
          spiParams.transferMode    = SPI_MODE_BLOCKING;
          spiParams.mode            = SPI_MASTER;
          spiParams.transferTimeout = 500000; // timeout 5s
          spiParams.bitRate         = 12000000;
          spiParams.frameFormat     = 0;
          spiParams.dataSize        = 8;

          spi = SPI_open(CONFIG_SPI_0, &spiParams);

          if (spi == NULL) {
             while(1);
          }


    }

    void initSS() {
        GPIO_setConfig(CONFIG_GPIO_W5500_CS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
        GPIO_setConfig(CONFIG_GPIO_W5500_RESET, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
        GPIO_write(CONFIG_GPIO_W5500_RESET, 0); // Chip reset
        uint16_t xxx = 9999;
        while(xxx--);
        GPIO_write(CONFIG_GPIO_W5500_RESET, 1);
    }

    inline void setSS()
    {
        GPIO_write(CONFIG_GPIO_W5500_CS, 0);
    }


    inline void resetSS()
    {
        GPIO_write(CONFIG_GPIO_W5500_CS, 1);
    }
