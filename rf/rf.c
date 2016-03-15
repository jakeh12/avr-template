#include "rf.h"
#include "../oled/oled.h"

uint8_t rf_buffer[RF_BUFFER_SIZE];

uint8_t rf_length = 0;
uint8_t rf_rssi = 0;
uint8_t rf_crc = 0;


uint8_t map(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max)
{
    return (uint8_t)(((int)x - (int)in_min) * ((int)out_max - (int)out_min) / ((int)in_max - (int)in_min) + (int)out_min);
}


void rf_init()
{
    DDRB |= _BV(PB4); // LED for status indication, ignore anything with "PB4"
    PORTB |= _BV(PB4); // LED OFF (HIGH = OFF, LOW = ON...)

    for (int i = 0; i < RF_BUFFER_SIZE; i++)
    {
        rf_buffer[i] = 0x00; // clear buffer
    }

    TRXPR |= _BV(TRXRST);     // reset transceiver
    IRQ_MASK = 0x00; // disable interrupts
    TRX_STATE = _BV(TRX_CMD3); // turn off the transceiver

    // wait certain time
    _delay_ms(5);


    PHY_TX_PWR = 0x00;    // max tx power (lowest = 0x0B)

    TRX_CTRL_1 |= _BV(TX_AUTO_CRC_ON); // auto crc calculation

    //RX_CTRL |= _BV(PDT_THRES1) | _BV(PDT_THRES0);

    TRX_CTRL_2 |= _BV(OQPSK_DATA_RATE1) | _BV(OQPSK_DATA_RATE1);     // 2Mbit/s data rate

    // enable antenna diversity
    //ANT_DIV |= _BV(ANT_DIV_EN) | _BV(ANT_EXT_SW_EN) | _BV(ANT_CTRL0);

    IRQ_MASK = _BV(RX_START_EN) | _BV(RX_END_EN) | _BV(TX_END_EN); // enable interrupts

    PHY_CC_CCA |=  _BV(CHANNEL3) | _BV(CHANNEL1) | _BV(CHANNEL0); // channel  (1011)


    TRX_STATE = (TRX_STATE & ~_BV(TRX_CMD3)) | _BV(TRX_CMD2) | _BV(TRX_CMD1); // state = RX_ON

    // done initializing
}

void rf_transmit(uint8_t* frame, uint8_t length)
{
    if (length > 127) { length = 127; } // make sure length is always less than 128

    TRX_STATE = (TRX_STATE & ~(_BV(TRX_CMD2) | _BV(TRX_CMD1))) | _BV(TRX_CMD3) | _BV(TRX_CMD0); // state = PLL_ON (TX_ON)

    while(TRX_STATUS3 & TRX_STATUS0); // wait for PLL to lock on the right frequency

    PORTB &= ~_BV(PB4); // turn status led on

    TRXFBST = length + 2; // save the length of the frame in first byte of the transceiver buffer
    memcpy((void *)(&TRXFBST+1), frame, length); // copy content of the frame into the transceiver buffer

    TRXPR |= _BV(SLPTR); // send transceiver to sleep -> it will initiate the tx procedure
    TRXPR &= ~_BV(SLPTR); // it needs to be high for a little bit so lets pull it back down

    TRX_STATE = (TRX_STATE & ~(_BV(TRX_CMD3) | _BV(TRX_CMD0))) | _BV(TRX_CMD2) | _BV(TRX_CMD1); // switch back to receiver mode RX_ON
}

ISR(TRX24_TX_END_vect) // interrupt for the end of transmission
{
    PORTB |= _BV(PB4);
    //display_cnprintf("TRANSMITTED!");
}

ISR(TRX24_RX_START_vect) //interrupt for beginning receiving frame
{
    PORTB &= ~_BV(PB4);
    rf_rssi = PHY_RSSI & 0x1F; // save signal strength 4:0RSSI rx strength (0 min, 28 max)
}

ISR(TRX24_RX_END_vect) // interrupt for finished receiving frame
{
    rf_crc =  (PHY_RSSI & 0x80) >> 7;

    if (rf_rssi && rf_crc) // if signal strength is more than -90 dBm and CRC is valid
    {
        rf_length = TST_RX_LENGTH;

        memcpy(&rf_buffer, (void*)&TRXFBST, rf_length); // copy content of the buffer to the variable
        //display_cnprintf("RSSI:%d%% CRC: %d", map(rssi, 0, 28, 0, 100), crc);

    }
    else // corrupted package
    {
        //display_ciprintf("RSSI:%d%% CRC: %d", map(rssi, 0, 28, 0, 100), crc);
    }

    PORTB |= _BV(PB4);

}

uint8_t rf_available()
{
    if (rf_length)
    {
        return rf_length - 2;

    }
    return rf_length;
}

void rf_read(uint8_t* to)
{
    memcpy(to, rf_buffer, rf_length-2);
    rf_length = 0;
}

uint8_t link_power()
{
    return map(rf_rssi, 0, 28, 0, 100);
}

uint8_t crc_res()
{
    return rf_crc;
}
