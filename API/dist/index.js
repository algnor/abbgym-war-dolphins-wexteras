"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const fs_1 = __importDefault(require("fs"));
const app = (0, express_1.default)();
const port = 3000;
app.get('/hej', (req, res) => {
    res.send('hej!');
});
app.get('/gettime', (req, res) => {
    res.send(new Date().toISOString());
});
app.get('/savetemp', (req, res) => {
    let temp = String(req.query.data);
    let time = Date.now();
    if (temp === 'undefined') {
        res.send(400);
        return;
    }
    fs_1.default.appendFileSync('data/tempData.txt', temp + ", " + time + '\n');
    res.send(200);
});
app.get('/saveHum', (req, res) => {
    let hum = String(req.query.data);
    let time = Date.now();
    if (hum === 'undefined') {
        res.send(400);
        return;
    }
    fs_1.default.appendFileSync('data/humData.txt', hum + ", " + time + '\n');
    res.send(200);
});
app.use('/data', express_1.default.static('data'));
app.listen(port, () => {
    console.log(`listening till port ${port}`);
});
