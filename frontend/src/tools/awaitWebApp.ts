export const awaitWebApp = async (): Promise<boolean> => (
  new Promise((resolve) => {
    const handle = window.setTimeout(() => {
      resolve(false);

      window.clearTimeout(handle);
    }, 100);
  })
);
