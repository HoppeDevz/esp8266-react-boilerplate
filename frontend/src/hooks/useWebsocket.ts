import { useEffect } from "react";

export function useWebsocketServer(onMessageHandle: (websocket: WebSocket, event: MessageEvent<any>) => void ) {

    useEffect(() => {

        const websocketGateway = `ws://${window.location.host}/ws`;
        const websocket = new WebSocket(websocketGateway);

        websocket.addEventListener("message", (event) => onMessageHandle(websocket, event));

        return () => {

            websocket.close();
        }

    }, []);

}