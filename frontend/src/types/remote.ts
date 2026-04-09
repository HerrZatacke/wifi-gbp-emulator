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
  height: number,
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

export interface RemoteEnv {
  targetWindow?: Window,
  isIframe: boolean,
  isPopup: boolean,
  isRemote: boolean,
}

export interface PrinterStatusCommand {
  name: PrinterFunction.CLEARPRINTER | PrinterFunction.CHECKPRINTER,
  fn: () => Promise<CheckPrinterStatus>,
}

export interface PrinterFetchImagesCommand {
  name: PrinterFunction.FETCHIMAGES,
  fn: (targetWindow: Window, params: PrinterParams, remoteParams: URLSearchParams) => Promise<PrinterImages>,
}

export type PrinterCommand = PrinterStatusCommand | PrinterFetchImagesCommand;
