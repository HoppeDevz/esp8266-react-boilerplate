import { useWebsocketServer } from "./hooks/useWebsocket";

function App() {

    useWebsocketServer((ws, event) => {

      if (event.data === "ping") {

        ws.send("pong!");
      }

    });

    return(
        <div
          className="w-screen h-screen bg-[#080808] flex justify-center items-center"
        >
            <p
              className="text-[13px] text-[#BABABA]"
            >
              ESP8266 and React!
            </p>
        </div>
    )
}

export default App;
