import { $fetch, type FetchResponse } from 'ofetch';
import { type BlobResponse } from '../types/remote.ts';

const fetchDumpRetry = async (url: string, retries: number): Promise<BlobResponse> => {

  try {
    const res: FetchResponse<Blob> = await $fetch.raw(url, { responseType: 'blob' });
    const headers: Record<string, string> = {};
    res.headers.forEach((value: string, key: string) => {
      headers[key] = value;
    });

    if (!res._data) {
      throw new Error(`Could not fetch ${url}`);
    }

    return {
      blob: res._data,
      contentType: res.headers.get('content-type') || undefined,
      meta: {
        headers,
      },
      status: res.status,
      ok: res.ok,
    };
  } catch (error) {
    if (retries <= 1) {
      throw error;
    }

    return fetchDumpRetry(url, retries - 1);
  }

};

export default fetchDumpRetry;
