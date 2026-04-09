import { type RemoteEnv, type RemotePrinterEvent } from '../types/remote.ts';

type cleanupFn = () => void;

const startHeartbeat = ({ targetWindow }: RemoteEnv, commands: string[]): cleanupFn => {

  const heartBeat = () => {
    if (!targetWindow) { return; }

    targetWindow.postMessage({
      fromRemotePrinter: {
        height: document.body.getBoundingClientRect().height,
        commands,
      },
    } as RemotePrinterEvent, '*');
  };

  const interval = window.setInterval(heartBeat, 500);

  return () => {
    window.clearInterval(interval);
  };
};

export default startHeartbeat;
