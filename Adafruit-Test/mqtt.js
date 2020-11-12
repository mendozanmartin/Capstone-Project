const mqtt = require('mqtt')
const client = mqtt.connect('mqtt://io.adafruit.com', { username: "mendozanmartin", password: "aio_wwQH757UghxuMx3TXIfjEUICj0DU" })

client.on('connect', function () {
    client.subscribe('mendozanmartin/feeds/outlet-flowrate', function (err) {

    })

    client.publish('mendozanmartin/feeds/outlet-flowrate', "21")
})