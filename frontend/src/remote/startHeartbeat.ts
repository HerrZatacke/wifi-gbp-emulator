import { type PrinterFunction } from '../consts.ts';
import { type RemotePrinterEvent } from '../types/remote.ts';

type cleanupFn = () => void;

const startHeartbeat = (targetWindow: Window, commands: PrinterFunction[]): cleanupFn => {

  const heartBeat = () => {
    if (!targetWindow) { return; }

    targetWindow.postMessage({
      fromRemotePrinter: {
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
