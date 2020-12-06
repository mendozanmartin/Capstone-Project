const mqtt = require('mqtt')
const adafruitClient = mqtt.connect('mqtt://io.adafruit.com', { username: "mendozamartin", password: "" })

const hivemqClient = mqtt.connect('mqtt://broker.hivemq.com')
let sent = false;

console.log("Starting mqtt connections...")
adafruitClient.on('connect', function () {
    adafruitClient.subscribe('mendozamartin/feeds/inlet-valve', function (err) {
        if (err) {
            console.log("There is an error with the adafruit mendozamartin/feeds/inlet-valve feed")
        }
    })
})

adafruitClient.on('message', function (topic, message) {
    console.log(topic + " " + message)
    if (sent == false) {
        hivemqClient.publish(topic, message)
    }
    sent = !sent;
})

hivemqClient.on('connect', function () {
    hivemqClient.subscribe('mendozamartin/feeds/filter-level', function (err) {
    })

    hivemqClient.subscribe("mendozamartin/feeds/final-tank-tds", function (err) {
    })
    hivemqClient.subscribe("mendozamartin/feeds/final-tank-turbidity", function (err) {
    })
    hivemqClient.subscribe("mendozamartin/feeds/outlet-flowrate", function (err) {
    })
    hivemqClient.subscribe("mendozamartin/feeds/inlet-valve", function (err) {
    })
})

hivemqClient.on('message', function (topic, message) {
    console.log(topic + " " + message)
    if (sent == false) {
        adafruitClient.publish(topic, message)
    }
    sent = !sent;
})