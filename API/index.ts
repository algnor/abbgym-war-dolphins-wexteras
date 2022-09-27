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

app.get('/saveData', (req: Request, res: Response) => {
  let temp = String(req.query.temp)
  let hum = String(req.query.hum)
  let time = Date.now()

  console.log("saving temperature: ", temp, time)

  if (temp !== 'undefined') {
    fs.appendFileSync('data/tempData.txt', temp + ", " + time + '\n')
  }



  console.log("saving humidity: ", hum, time)

  if (hum !== 'undefined') {
    fs.appendFileSync('data/humData.txt', hum + ", " + time + '\n')
  }
  res.sendStatus(200)
})


app.get('/settings', (req: Request, res: Response) => {
  let data = req.query

  console.log("updating settings: ", data)

  let rawSettings = fs.readFileSync("data/settings.json")
  let settings = JSON.parse(String(rawSettings))
  for (let x in data) {
    settings[x] = data[x]
    if (data[x] === "true" || data[x] === "false") {
      settings[x] = data[x] ==="true"
      continue
    }
    if (!isNaN(Number(data[x]))) {
      settings[x] = Number(data[x])
    }
  }
  console.log(settings)
  fs.writeFileSync("data/settings.json", JSON.stringify(settings))
  res.send(settings)
})

app.use('/data', express.static('data'))
app.use('/', express.static("../hemsida"))

app.listen(port, () => {
  console.log(`listening till port ${port}`)
})

