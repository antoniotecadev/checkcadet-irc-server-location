const net = require("net"); // módulo para conexões TCP
const WebSocket = require("ws"); // módulo para WebSocket

// Criando um servidor WebSocket na porta 3000
const wss = new WebSocket.Server({ port: 3000 });

wss.on("connection", (ws) => {

    console.log("WebSocket client connected", ws._socket.remoteAddress);

    // Conectando ao servidor IRC
    const irc = net.createConnection({
        host: "irc-server", // nome do serviço
        port: 6667
    });

    // buffer para evitar problemas de fragmentação
    let buffer = "";

    // Quando receber uma mensagem do WebSocket, envie para o servidor IRC
    ws.on("message", (msg) => {
        const data = msg.toString().trim();
        irc.write(data + "\r\n"); // RFC obrigatório
    });

    // Quando receber dados do servidor IRC, envie para o WebSocket
    irc.on("data", (data) => {
        buffer += data.toString();

        let lines = buffer.split("\r\n");
        buffer = lines.pop(); // última linha pode estar incompleta, mantenha no buffer

        // Envie cada linha completa para o WebSocket
        for (const line of lines) {
            if (line.length > 0) {
                ws.send(line);
            }
        }
    });

    // Quando o WebSocket for fechado, feche a conexão com o servidor IRC
    ws.on("close", () => {
        console.log("WebSocket disconnected", ws._socket.remoteAddress);
        irc.end();
    });

    // Tratamento de erros
    irc.on("error", (err) => {
        console.log("IRC error:", err.message);
        ws.close();
    });
});