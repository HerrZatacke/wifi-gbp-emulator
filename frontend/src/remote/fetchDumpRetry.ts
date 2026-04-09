import { $fetch } from 'ofetch';
import { type BlobResponse } from '../types/remote.ts';

const fetchDumpRetry = async (url: string, retries: number): Promise<BlobResponse> => {
  let res: Response;

  try {
    res = await $fetch.raw(url, { responseType: 'arrayBuffer' });
  } catch (error) {
    if (retries <= 1) {
      throw error;
    }

    return fetchDumpRetry(url, retries - 1);
  }

  const headers: Record<string, string> = {};
  res.headers.forEach((value: string, key: string) => {
    headers[key] = value;
  });

  return (
    res.blob()
      .then((blob: Blob): BlobResponse => ({
        blob,
        contentType: res.headers.get('content-type') || undefined,
        meta: {
          headers,
        },
        status: res.status,
        ok: res.ok,
      }))
  );
};

export default fetchDumpRetry;
