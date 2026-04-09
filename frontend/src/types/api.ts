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

export interface NetworkResponse {
  ssid: string;
}

export interface NetworkDeleteRequest extends NetworkResponse {
  delete: boolean;
}

export interface NetworkUpdateRequest extends NetworkResponse {
  psk: string;
}

export interface AccesspointResponse {
  ssid: string;
}

export interface AccespointUpdateRequest extends AccesspointResponse {
  ssid: string;
  psk: string;
}

export interface WiFiConfigResponse {
  mdns: string;
  networks: NetworkResponse[];
  ap: AccesspointResponse;
}

export interface WifiConfigUpdateRequest {
  mdns?: string;
  networks?: (NetworkUpdateRequest | NetworkDeleteRequest)[];
  ap?: AccespointUpdateRequest;
}

export type UpdateFn = (request: NetworkUpdateRequest | NetworkDeleteRequest) => Promise<void>;

export interface DetectRemote {
  isRemote: boolean,
  targetWindow: Window | null,
}
