import {
  type PrinterImages,
  type PrinterParams,
  type BlobResponse,
  type RemotePrinterEvent,
} from '../../types/remote.ts';
import fetchDumpRetry from '../fetchDumpRetry.ts';


const fetchImages = async (
  targetWindow: Window,
  { dumps }: PrinterParams,
): Promise<PrinterImages> => {
  const blobsdone: BlobResponse[] = [];
  let index = 0;

  for (const dump of dumps) {
    try {
      const blobResponse: BlobResponse = await fetchDumpRetry(dump, 3);

      targetWindow.postMessage({
        fromRemotePrinter: {
          progress: (index + 1) / dumps.length,
        },
      } as RemotePrinterEvent, '*');

      blobsdone.push(blobResponse);
    } catch (error) {
      console.warn(error);
      blobsdone.push({
        ok: false,
        blob: new Blob(),
      });
    }
    index += 1;
  }

  return {
    blobsdone,
  };
};

export default fetchImages;
