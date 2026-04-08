export interface ListDumpsResponse {
  dumps: string[],
  fs: {
    total: number,
    used: number,
    available: number,
    maximages: number,
    dumpcount: number,
  }
}

export interface ClearDumpsResponse {
  deleted: number;
}
