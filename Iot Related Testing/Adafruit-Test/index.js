const ADAFRUIT_IO_USERNAME = "mendozanmartin"
const ADAFRUIT_IO_KEY = "aio_xYpn92R9hsfHwDcEAFckkMD07VpX"

const axios = require('axios')


setInterval(() => {
    axios({
        method: 'post',
        url: `https://io.adafruit.com/api/v2/${ADAFRUIT_IO_USERNAME}/feeds/outlet-flowrate/data`,
        headers: { 'Content-Type': 'application/json', 'X-AIO-Key': ADAFRUIT_IO_KEY },
        data: { 'value': Math.random() * 100 }
    }).then(() => {
        console.log('data sent')
    }).catch((e) => {
        console.log(e)
    })
}, 3000)

setInterval(() => {
    let value = 'ON'

    if (value == 'ON') {
        value = 'OFF'
    } else {
        value = 'ON'
    }
    axios({
        method: 'post',
        url: `https://io.adafruit.com/api/v2/${ADAFRUIT_IO_USERNAME}/feeds/outlet-valve/data`,
        headers: { 'Content-Type': 'application/json', 'X-AIO-Key': ADAFRUIT_IO_KEY },
        data: { 'value': value }
    })
}, 3000)
