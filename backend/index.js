const express = require('express')
const app = express()
const port = 3000
const fs = require('fs')

const tempLogger = fs.createWriteStream('data/temp.csv', {
    flags: 'a'
})

const lightLogger = fs.createWriteStream('data/light.csv', {
    flags: 'a'
})

app.post('/', (req, res) => {
    const temp = req.header('temperature')
    const light = req.header('light')
    console.log(`~[${Date.now()}] temp: ${temp}c, light: ${light}`)
    tempLogger.write(`${temp},${Date.now()}\n`)
    lightLogger.write(`${light},${Date.now()}\n`)
    res.send('Got a POST request')
})

app.use('/data', express.static('data'))

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})