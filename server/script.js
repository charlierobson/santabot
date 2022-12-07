const express = require('express')
const app = express();
const http = require('http');
const server = http.createServer(app);
var bodyParser = require('body-parser')
const { Server } = require("socket.io");
const io = new Server(server, {
    cors: {
        origin: "*"
    }
});
const port = 8081

app.post('/santabot', bodyParser.text(), (req, res) => {
    console.log('req body ', req.body);
    io.emit('stateChange', req.body);
    res.sendStatus(200)
});

io.on('connection', (socket) => {
    console.log('a user connected');
});

server.listen(port, () => {
    console.log('listening on *:8081');
});