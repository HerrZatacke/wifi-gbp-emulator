import { $fetch } from 'ofetch';
import { URL_LIST_DUMPS } from '../../consts.ts';
import  { type ListDumpsResponse } from '../../types/api.ts';
import { type CheckPrinterStatus } from '../../types/remote.ts';

const checkPrinter = async (): Promise<CheckPrinterStatus> => {
  const dumpsInfo = await $fetch<ListDumpsResponse>(URL_LIST_DUMPS);

  return {
    printerData: {
      ...dumpsInfo,
      dumps: [...dumpsInfo.dumps].sort(),
    },
  };
};

export default checkPrinter;
