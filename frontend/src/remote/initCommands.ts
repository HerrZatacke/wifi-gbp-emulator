import { PrinterFunction } from '../consts.ts';
import {
  type CheckPrinterStatus,
  type PrinterCommand,
  type PrinterFetchImagesCommand,
  type PrinterImages,
  type PrinterParams,
  type PrinterStatusCommand,
  type RemotePrinterEvent,
} from '../types/remote.ts';
import checkPrinter from './commands/checkPrinter';
import clearPrinter from './commands/clearPrinter';
import fetchImages from './commands/fetchImages';

const initCommands = (targetWindow: Window): PrinterCommand[] => {
  const commands: PrinterCommand[] = [
    {
      name: PrinterFunction.CHECKPRINTER,
      fn: checkPrinter,
    },
    {
      name: PrinterFunction.FETCHIMAGES,
      fn: fetchImages,
    },
    {
      name: PrinterFunction.CLEARPRINTER,
      fn: clearPrinter,
    },
  ];

  window.addEventListener('message', async (event: MessageEvent<RemotePrinterEvent>) => {
    if (event.source !== targetWindow) {
      return;
    }

    const printerCommand = event.data.toRemotePrinter;
    if (!printerCommand) {
      return;
    }

    let fromRemotePrinter: CheckPrinterStatus | PrinterImages | null;

    switch (printerCommand.command) {
      case PrinterFunction.FETCHIMAGES: {
        const commandFn = commands.find(({ name }) => name === printerCommand.command) as PrinterFetchImagesCommand;
        fromRemotePrinter = await commandFn.fn(targetWindow, (printerCommand.params as PrinterParams).dumps);
        break;
      }

      case PrinterFunction.CLEARPRINTER:
      case PrinterFunction.CHECKPRINTER: {
        const commandFn = commands.find(({ name }) => name === printerCommand.command) as PrinterStatusCommand;
        fromRemotePrinter = await commandFn.fn();
        break;
      }

      default:
        fromRemotePrinter = null;
    }

    if (fromRemotePrinter) {
      targetWindow.postMessage({
        fromRemotePrinter,
      } as RemotePrinterEvent, '*');
    }
  });


  return commands;
};

export default initCommands;
