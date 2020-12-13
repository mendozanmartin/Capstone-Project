const mqtt = require('mqtt')
const adafruitClient = mqtt.connect('mqtt://io.adafruit.com', { username: "mendozamartin", password: "aio_UUTB761C9ljFBPDBSkTP5v7GKZQh" })

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
    let feed = topic.split('/')
    if (sent == false) {
        hivemqClient.publish(feed[2], message)
    }
    sent = !sent;
})

hivemqClient.on('connect', function () {
    hivemqClient.subscribe('filter-level', function (err) {
    })

    hivemqClient.subscribe("final-tank-tds", function (err) {
    })
    hivemqClient.subscribe("final-tank-turbidity", function (err) {
    })
    hivemqClient.subscribe("outlet-flowrate", function (err) {
    })
    hivemqClient.subscribe("inlet-valve", function (err) {
    })
})

hivemqClient.on('message', function (topic, message) {
    let modifiedTopic = "mendozamartin/feeds/" + topic
    console.log(modifiedTopic + " " + topic)
    // if (sent == false) {
    adafruitClient.publish("mendozamartin/feeds/" + topic, message)
    // }
    // sent = !sent;
})