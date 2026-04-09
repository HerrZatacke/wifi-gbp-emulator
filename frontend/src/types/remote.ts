import { type PrinterFunction } from '../consts.ts';
import { ListDumpsResponse } from './api.ts';

export interface BlobResponse {
  ok: boolean
  blob: Blob
  contentType?: string
  status?: number
  meta?: {
    headers: Record<string, string>,
  },
}

export interface CheckPrinterStatus {
  printerData: ListDumpsResponse,
}

export interface PrinterImages {
  blobsdone: BlobResponse[],
}

export type FromPrinterEvent = Partial<CheckPrinterStatus & PrinterImages & {
  progress: number,
  commands: PrinterFunction[],
}>

export interface PrinterParams {
  dumps: string[],
}

export interface ToPrinterEvent {
  command: string,
  params?: PrinterParams,
}

export type RemotePrinterEvent = Partial<{
  fromRemotePrinter: FromPrinterEvent,
  toRemotePrinter: ToPrinterEvent,
}>

export interface PrinterStatusCommand {
  name: PrinterFunction.CLEARPRINTER | PrinterFunction.CHECKPRINTER,
  fn: () => Promise<CheckPrinterStatus>,
}

export interface PrinterFetchImagesCommand {
  name: PrinterFunction.FETCHIMAGES,
  fn: (targetWindow: Window, dumps: string[]) => Promise<PrinterImages>,
}

export type PrinterCommand = PrinterStatusCommand | PrinterFetchImagesCommand;
