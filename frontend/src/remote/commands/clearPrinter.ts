import { $fetch } from 'ofetch';
import { URL_CLEAR_DUMPS } from '../../consts.ts';
import { type ClearDumpsResponse } from '../../types/api.ts';
import { type CheckPrinterStatus } from '../../types/remote.ts';
import checkPrinter from './checkPrinter';

const clearPrinter = async (): Promise<CheckPrinterStatus> => {
  const clearDumpsResponse = await $fetch<ClearDumpsResponse>(URL_CLEAR_DUMPS);
  if (clearDumpsResponse.deleted !== undefined) {
    return checkPrinter();
  }

  throw new Error('error while deleting images');
};

export default clearPrinter;
