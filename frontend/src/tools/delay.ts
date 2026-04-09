export const delay = async (duration: number) => (
  new Promise((resolve) => {
    self.setTimeout(resolve, duration);
  })
);
