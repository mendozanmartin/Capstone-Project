const { MqttClient } = require('mqtt')
const mqtt = require('mqtt')
const adafruitClient = mqtt.connect('mqtt://io.adafruit.com', { username: "mendozanmartin", password: "aio_HpFX47bwUsrNTWi7Ba2JgnRYFWhe" })

const hivemqClient = mqtt.connect('mqtt://broker.hivemq.com')

console.log("Starting mqtt connections...")
adafruitClient.on('connect', function () {
    adafruitClient.subscribe('mendozanmartin/feeds/outlet-valve', function (err) {
        if (err) {
            console.log("There is an error with the adafruit mendozanmartin/feeds/outlet-valve feed")
        }
    })
    adafruitClient.subscribe('mendozanmartin/feeds/inlet-valve', function (err) {
        if (err) {
            console.log("There is an error with the adafruit mendozanmartin/feeds/inlet-valve")
        }
    })

})

adafruitClient.on('message', function (topic, message) {
    let str = topic.split("/");


    console.log(topic + " " + message)
    hivemqClient.publish(str[str.length -1], message)
})

hivemqClient.on('connect', function() {
    hivemqClient.subscribe('TDS-collection', function (err) {
        if (err) {
            console.log("There is an error with the hivemq TDS ")
        } 
    })
})

hivemqClient.on('message', function(topic, message) {
    console.log(topic + " " + message)
    if (topic === "TDS-collection") {
        adafruitClient.publish("mendozanmartin/feeds/collection-tank-tds", message)
    }
})