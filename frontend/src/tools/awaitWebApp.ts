import { DetectRemote } from '../types/api.ts';

export const awaitWebApp = async (): Promise<DetectRemote> => (
  new Promise((resolve) => {
    if (window.opener !== window.self) {
      resolve({
        isRemote: true,
        targetWindow: window.opener,
      });
    } else {
      resolve({
        isRemote: false,
        targetWindow: null,
      });
    }
  })
);
