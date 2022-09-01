import express, { Express, Request, Response } from 'express';
import fs from "fs"
const app = express()
const port = 3000

app.get('/hej', (req: Request, res: Response) => {
    res.send('hej!')
})

app.get('/gettime', (req: Request, res: Response) => {
    res.send(new Date().toISOString())
})

app.get('/savetemp', (req: Request, res: Response) => {
    let temp = String(req.query.data)
    let time = Date.now()

    if (temp === 'undefined') {
        res.send(400)
        return
    }
    fs.appendFileSync('data/tempData.txt', temp + ", " + time + '\n')
    res.send(200)

})

app.get('/saveHum', (req: Request, res: Response) => {
    let hum = String(req.query.data)
    let time = Date.now()

    if (hum === 'undefined') {
        res.send(400)
        return
    }
    fs.appendFileSync('data/humData.txt', hum + ", " + time + '\n')
    res.send(200)

})

app.get('/setData', (req: Request, res: Response) => {
    let rawSettings = fs.readFileSync("data/settings.json")
    let settings = JSON.parse(String(rawSettings))
    res.send(settings)
})

app.use('/data', express.static('data'))

app.listen(port, () => {
    console.log(`listening till port ${port}`)
})

