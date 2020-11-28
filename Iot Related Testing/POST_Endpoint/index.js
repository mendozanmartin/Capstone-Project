const express = require('express');
const ngrok = require('ngrok');

const app = express();
app.use(express.json());

(async function () {
    const url = await ngrok.connect();
    console.log(url);
})();

app.post('/data', (req, res) => {
    console.log(req.body)
    res.status(200).send(req.body)
})

app.get('/', (req, res) => {
    res.send('Hello from Martin, Ernest and Javaughn!')
})

app.listen(80, function () {
    console.log('Example app listening on port 3000.');
});